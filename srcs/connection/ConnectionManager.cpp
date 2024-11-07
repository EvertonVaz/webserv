/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectionManager.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 11:05:35 by Everton           #+#    #+#             */
/*   Updated: 2024/11/06 23:01:56 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <poll.h>
#include <iostream>
#include <sys/stat.h>
#include "../aux.hpp"
#include "ConnectionManager.hpp"
#include "../handlers/ErrorHandler.hpp"

ConnectionManager::ConnectionManager(Server &server) {
	this->socketInterface = server.getSocketInterface();
	clientBuffers = std::map<int, std::string>();
	std::vector<int> serverListenSockets = server.getListenSockets();
    this->serverConfigs = server.getServers();

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
    int bytesRead = socketInterface->recv(clientSockFd, buffer, sizeof(buffer), 0);

    if (bytesRead <= 0) {
        if (bytesRead == 0) {
            closeConnection(clientSockFd);
        }
        return;
    }

    std::string data(buffer, bytesRead);
    requests[clientSockFd].appendData(data, serverConfigs);

    if (requests[clientSockFd].hasError()) {
        HTTPResponse response;
        ErrorHandler errorHandler;
        errorHandler.handleError(400, response);
        sendResponse(clientSockFd, response);
        closeConnection(clientSockFd);
    } else if (requests[clientSockFd].isComplete()) {
        processRequest(clientSockFd, requests[clientSockFd]);
    }
}

void ConnectionManager::sendResponse(int clientSockFd, HTTPResponse& response) {
    std::string responseStr = response.generateResponse();
    int totalSent = 0;
    int responseLength = responseStr.size();
    const char* responseData = responseStr.c_str();

    while (totalSent < responseLength) {
        int sent = socketInterface->send(clientSockFd, responseData + totalSent, responseLength - totalSent, 0);
        if (sent < 0) {
            closeConnection(clientSockFd);
            return;
        }
        totalSent += sent;
    }
}

void ConnectionManager::processRequest(int clientSockFd, const HTTPRequest& request) {
    HTTPResponse response;
    router = selectConfig(request, serverConfigs);

    router.handleRequest(request, response);

    std::map<std::string, std::string> reqHeaders = request.getHeaders();
    if (reqHeaders.find("Connection") != reqHeaders.end() && reqHeaders["Connection"] == "close") {
        response.setCloseConnection(true);
    }

    std::string responseStr = response.generateResponse();
    int totalSent = 0;
    int responseLength = responseStr.size();
    const char* responseData = responseStr.c_str();

    while (totalSent < responseLength) {
        int sent = socketInterface->send(clientSockFd, responseData + totalSent, responseLength - totalSent, 0);
        if (sent < 0) {
            closeConnection(clientSockFd);
            throw std::runtime_error("Error sending response to client");
            return;
        }
        totalSent += sent;
    }

    if (response.shouldCloseConnection()) {
        closeConnection(clientSockFd);
    } else {
        requests.erase(clientSockFd);
    }
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
