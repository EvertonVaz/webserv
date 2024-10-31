/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectionManager.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 11:05:35 by Everton           #+#    #+#             */
/*   Updated: 2024/10/31 11:43:41 by Everton          ###   ########.fr       */
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
        if (bytesRead == 0 || errno != EWOULDBLOCK) {
            closeConnection(clientSockFd);
        }
        return;
    }

    std::string data(buffer, bytesRead);
    requests[clientSockFd].appendData(data);

    if (requests[clientSockFd].hasError()) {
        closeConnection(clientSockFd);
    } else if (requests[clientSockFd].isComplete()) {
        processRequest(clientSockFd, requests[clientSockFd]);
    }
}

std::string getContentType(const std::string& extension) {
    if (extension == ".html") return "text/html";
    if (extension == ".css") return "text/css";
    if (extension == ".js") return "application/javascript";
    if (extension == ".png") return "image/png";
    if (extension == ".jpg" || extension == ".jpeg") return "image/jpeg";
    return "application/octet-stream";
}

void serveStaticFile(const std::string& filePath, HTTPResponse& response) {
    std::ifstream file(filePath.c_str(), std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        response.setStatusCode(404);
        response.setBody("<html><body><h1>404 Not Found</h1></body></html>");
        response.addHeader("Content-Type", getContentType(response.getHeaders()["Content-Type"]));
        return;
    }
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    response.setBody(content);

    // Definir o Content-Type (usar uma função para mapear extensões para MIME types)
    response.addHeader("Content-Type", "text/html");

    file.close();
}

void ConnectionManager::processRequest(int clientSockFd, const HTTPRequest& request) {
    HTTPResponse response;

    if (request.hasError()) {
        response.setStatusCode(400);
        response.setBody("<html><body><h1>400 Bad Request</h1></body></html>");
        response.addHeader("Content-Type", "text/html");
    } else {
        std::string method = request.getMethod();
        std::string uri = request.getURI();

        if (method == "GET") {
            std::string path = "/home/etovaz/nave/webserver/srcs/connection/test.html";
            serveStaticFile(path, response);
        } else if (method == "POST") {
            response.setStatusCode(200);
            response.setBody("<html><body><h1>POST recebido</h1></body></html>");
            response.addHeader("Content-Type", "text/html");
        } else if (method == "DELETE") {
            response.setStatusCode(200);
            response.setBody("<html><body><h1>Recurso deletado</h1></body></html>");
            response.addHeader("Content-Type", "text/html");
        } else {
            response.setStatusCode(405); // Method Not Allowed
            response.addHeader("Allow", "GET, POST, DELETE");
            response.setBody("<html><body><h1>405 Method Not Allowed</h1></body></html>");
            response.addHeader("Content-Type", "text/html");
        }
    }

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