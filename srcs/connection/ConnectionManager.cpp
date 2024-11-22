/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectionManager.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 11:05:35 by Everton           #+#    #+#             */
/*   Updated: 2024/11/22 15:03:48 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>
#include <poll.h>
#include <sys/stat.h>
#include "../aux.hpp"
#include "ConnectionManager.hpp"
#include "../handlers/ErrorHandler.hpp"
#include "../logger/Logger.hpp"

ConnectionManager::ConnectionManager(Server &server) {
    this->logger = &Logger::getInstance();
    this->serverConfigs = server.getServers();
	clientBuffers = std::map<int, std::string>();
	this->socketInterface = server.getSocketInterface();
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
			logger->log(Logger::ERROR, "poll() falhou");
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
        logger->log(Logger::ERROR, "Falha ao aceitar nova conexão");
        return;
    }

    if (socketInterface->fcntl(clientSockFd, F_SETFL, O_NONBLOCK) < 0) {
        socketInterface->close(clientSockFd);
        logger->log(Logger::ERROR, "Fail to set client socket to non-blocking");
    }

    struct pollfd clientPfd;
    clientPfd.fd = clientSockFd;
    clientPfd.events = POLLIN;
    clientPfd.revents = 0;
    pollFds.push_back(clientPfd);
    clientBuffers[clientSockFd] = "";

    logger->log(Logger::INFO, "Nova conexão aceita: socket fd " + itostr(clientSockFd));
}

void ConnectionManager::readFromClient(int clientSockFd) {
    char buffer[65535];
    clientBuffers[clientSockFd] = "";
    socketInterface->recv(clientSockFd, buffer, sizeof(buffer), 0);
    clientBuffers[clientSockFd].append(buffer);
    requests[clientSockFd].appendData(clientBuffers[clientSockFd], serverConfigs);

    if (requests[clientSockFd].hasError()) {
        HTTPResponse response;
        ErrorHandler errorHandler("");
        errorHandler.handleError(400, response);
        sendResponse(clientSockFd, response);
        logger->log(Logger::WARNING, "Erro na requisição do cliente socket fd " + itostr(clientSockFd));
        closeConnection(clientSockFd);
    } else if (requests[clientSockFd].isComplete()) {
        processRequest(clientSockFd, requests[clientSockFd]);
    }
}

void ConnectionManager::processRequest(int clientSockFd, HTTPRequest& request) {
    HTTPResponse response;
    router = selectConfig(request, serverConfigs);
    std::string uploadPath = router.getServerConfig().getRoutes()[request.getURI()].getUploadPath();
    request.setUploadPath(uploadPath);
    router.handleRequest(request, response);

    std::map<std::string, std::string> reqHeaders = request.getHeaders();
    if (reqHeaders.find("Connection") != reqHeaders.end() && reqHeaders["Connection"] == "close") {
        response.setCloseConnection(true);
    }
    logger->logRoute(
        "Requisição processada",
        request.getMethod(),
        request.getURI(),
        response.getStatusCode());
    sendResponse(clientSockFd, response);

    if (response.shouldCloseConnection()) {
        closeConnection(clientSockFd);
    }
}

void ConnectionManager::sendResponse(int clientSockFd, HTTPResponse& response) {
    std::string responseStr = response.generateResponse();
    int totalSent = 0;
    int responseLength = responseStr.size();
    const char* responseData = responseStr.c_str();
    requests.erase(clientSockFd);

    while (totalSent < responseLength) {
        int sent = socketInterface->send(clientSockFd, responseData + totalSent, responseLength - totalSent, 0);
        if (sent < 0) {
            closeConnection(clientSockFd);
            logger->log(Logger::ERROR, "Erro ao enviar resposta para o cliente socket fd " + itostr(clientSockFd));
            return;
        }
        totalSent += sent;
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
    logger->log(Logger::INFO, "Conexão fechada: socket fd " + itostr(sockFd));
}
