/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectionManager.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 11:05:35 by Everton           #+#    #+#             */
/*   Updated: 2024/10/30 17:47:47 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <poll.h>
#include <iostream>
#include <fstream>
#include "ConnectionManager.hpp"

ConnectionManager::ConnectionManager(Server &server) {
	this->socketInterface = server.getSocketInterface();
	clientBuffers = std::map<int, std::string>();
	std::vector<int> serverListenSockets = server.getListenSockets();

	for (size_t i = 0; i < serverListenSockets.size(); i++) {
		struct pollfd pfd;
		int sockFd = serverListenSockets[i];

		pfd.fd = serverListenSockets[i];
		pfd.events = POLLIN;
		pfd.revents = 0;
		this->pollFds.push_back(pfd);
		this->listenSockets.insert(sockFd);
	}
}

ConnectionManager::~ConnectionManager() {
	for (size_t i = 0; i < pollFds.size(); i++) {
		close(pollFds[i].fd);
	}
}

void ConnectionManager::run() {
	while (true) {
		int ret = socketInterface->poll(pollFds.data(), pollFds.size(), -1);

		if (ret == -1) {
			throw std::runtime_error("poll() failed");
			break ;
		}
		handleEvents();
	}
}

void ConnectionManager::handleEvents() {
    for (size_t i = 0; i < pollFds.size(); ++i) {
        if (pollFds[i].revents == 0) {
            continue;
        }

		int sockFd = pollFds[i].fd;
        if (listenSockets.count(sockFd) > 0) {
            if (pollFds[i].revents & POLLIN) {
                acceptNewConnection(pollFds[i].fd);
            }
        } else {
            if (pollFds[i].revents & POLLIN) {
                readFromClient(pollFds[i].fd);
            }
            if (pollFds[i].revents & POLLOUT) {
                writeToClient(pollFds[i].fd);
            }
            if (pollFds[i].revents & (POLLHUP | POLLERR)) {
                closeConnection(pollFds[i].fd);
            }
        }

        pollFds[i].revents = 0;
    }
}

void ConnectionManager::acceptNewConnection(int listenSockFd) {
    struct sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);

    int clientSockFd = socketInterface->accept(listenSockFd, (struct sockaddr*)&clientAddr, &clientLen);

    if (clientSockFd < 0) {
        throw std::runtime_error("Fail to accept new connection");
    }

    if (socketInterface->fcntl(clientSockFd, F_SETFL, O_NONBLOCK) < 0) {
        socketInterface->close(clientSockFd);
        throw std::runtime_error("Fail to set client socket to non-blocking");
    }

    struct pollfd clientPfd;
    clientPfd.fd = clientSockFd;
    clientPfd.events = POLLIN;
    clientPfd.revents = 0;
    pollFds.push_back(clientPfd);

    clientBuffers[clientSockFd] = "";

    std::cout << "New connect accept: socket fd " << clientSockFd << std::endl;
}

void ConnectionManager::readFromClient(int clientSockFd) {
    char buffer[1024];
    int bytesRead = socketInterface->recv(clientSockFd, buffer, sizeof(buffer) - 1, 0);

    if (bytesRead < 0) {
        closeConnection(clientSockFd);
        throw std::runtime_error("Error reading from client");
    } else if (bytesRead == 0) {
        std::cout << "Client disconnect: sock fd " << clientSockFd << std::endl;
        closeConnection(clientSockFd);
        return;
    }

    buffer[bytesRead] = '\0';
    clientBuffers[clientSockFd] += buffer;

    std::cout << "Receive data from client " << clientSockFd << ": " << buffer << std::endl;

    for (size_t i = 0; i < pollFds.size(); ++i) {
        if (pollFds[i].fd == clientSockFd) {
            pollFds[i].events |= POLLOUT;
            break;
        }
    }
}

// #TODO: REMOVER ESSA FUNÇÃO
static inline std::string arquivo() {
    std::ifstream file("/home/etovaz/nave/webserver/srcs/connection/test.html");
    if (!file.is_open()) {
        throw std::runtime_error("error ao ler arquivo html");
    }
    std::stringstream contentStream;
    contentStream << file.rdbuf();
    std::string content = contentStream.str();

    std::stringstream headerStream;
    headerStream << "HTTP/1.1 200 OK\r\n"
                << "Content-Type: text/html; charset=UTF-8\r\n"
                << "Content-Length: " << content.size() << "\r\n"
                << "Connection: close\r\n"
                << "\r\n";
    std::string header = headerStream.str();
    return header + content;
}

void ConnectionManager::writeToClient(int clientSockFd) {
    // #TODO: Remover esse exemplo simples de resposta HTTP
    std::string response = arquivo();
    int bytesSent = socketInterface->send(clientSockFd, response.c_str(), response.size(), 0);

    if (bytesSent < 0) {
        closeConnection(clientSockFd);
        throw std::runtime_error("Error sending response to client");
    }

    std::cout << "Response send to client " << clientSockFd << std::endl;

    // #TODO: verificar se a conexao e persistente, caso seja não devemos fechar ela
    closeConnection(clientSockFd);
}

void ConnectionManager::closeConnection(int sockFd) {
    socketInterface->close(sockFd);

    for (size_t i = 0; i < pollFds.size(); ++i) {
        if (pollFds[i].fd == sockFd) {
            pollFds.erase(pollFds.begin() + i);
            break;
        }
    }

    clientBuffers.erase(sockFd);

    std::cout << "Closed connection: socket fd " << sockFd << std::endl;
}

std::set<int> ConnectionManager::getListenSockets() const {
    return listenSockets;
}

std::vector<struct pollfd> ConnectionManager::getPollFds() const {
    return pollFds;
}

std::map<int, std::string> ConnectionManager::getClientBuffers() const {
    return clientBuffers;
}

void ConnectionManager::setPollFds(std::vector<struct pollfd> pollFds) {
    this->pollFds = pollFds;
}

void ConnectionManager::setClientBuffers(std::map<int, std::string> clientBuffers) {
    this->clientBuffers = clientBuffers;
}