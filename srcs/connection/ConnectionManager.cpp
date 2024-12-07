/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectionManager.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 11:05:35 by Everton           #+#    #+#             */
/*   Updated: 2024/12/07 17:47:47 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/poll.h>
#include <vector>
#include <poll.h>
#include <cstddef>
#include <sys/stat.h>
#include "../aux.hpp"
#include "../logger/Logger.hpp"
#include "ConnectionManager.hpp"
#include "../handlers/ErrorHandler.hpp"

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
                acceptNewConnection(sockFd);
            }
        } else {
            if (pollFds[i].revents & POLLIN) {
                readFromClient(sockFd);
            }
            if (pollFds[i].revents & POLLOUT) {
                processRequest(sockFd, requests[sockFd]);
            }
            if (pollFds[i].revents & (POLLHUP | POLLERR)) {
                closeConnection(sockFd);
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
        return;
    }

    struct pollfd clientPfd;
    clientPfd.fd = clientSockFd;
    clientPfd.events = POLLIN;
    clientPfd.revents = 0;
    pollFds.push_back(clientPfd);
    clientBuffers[clientSockFd] = "";
    std::string msg = "Nova conexão aceita: socket fd ";
    logger->log(Logger::INFO, msg + itostr(clientSockFd));
}

void ConnectionManager::handleReadError(int clientSockFd, const HTTPRequest& request) {
    HTTPResponse response;
    std::string path = selectConfig(request, serverConfigs).getErrorPage();
    ErrorHandler errorHandler(path);
    errorHandler.handleError(request.getState(), response);
    sendResponse(clientSockFd, response);
    logger->logRoute(
        "Erro na requisição ",
        request.getMethod(),
        request.getURI(),
        response.getStatusCode());
    return setPollRevents(clientSockFd, POLLERR);
}

void ConnectionManager::readFromClient(int clientSockFd) {
    char buffer[65535];
    clientBuffers[clientSockFd] = "";
    long int bytesRead = socketInterface->recv(clientSockFd, buffer, sizeof(buffer), 0);
    if (bytesRead == 0)
        return setPollRevents(clientSockFd, POLLHUP);
    if (bytesRead < 0) {
        logger->log(Logger::ERROR, "Erro ao ler dados do cliente socket fd " + itostr(clientSockFd));
        return setPollRevents(clientSockFd, POLLERR);
    }
    clientBuffers[clientSockFd] += std::string(buffer, bytesRead);
    requests[clientSockFd].appendData(clientBuffers[clientSockFd], serverConfigs);

    if (requests[clientSockFd].hasError()) {
        return handleReadError(clientSockFd, requests[clientSockFd]);
    }
    if (requests[clientSockFd].isComplete()) {
        return setPollRevents(clientSockFd, POLLOUT);
    }
}

void ConnectionManager::processRequest(int clientSockFd, HTTPRequest& request) {
    HTTPResponse response;
    router = selectConfig(request, serverConfigs);
    std::string uploadPath = router.getServerConfig().getRoutes()[request.getURI()].getUploadPath();
    request.setUploadPath(uploadPath);
    router.handleRequest(request, response);

    std::map<std::string, std::string> reqHeaders = request.getHeaders();
    if (reqHeaders.find("connection") != reqHeaders.end() && reqHeaders["connection"] == "close") {
        response.setCloseConnection(true);
    }
    logger->logRoute(
        "Requisição processada",
        request.getMethod(),
        request.getURI(),
        response.getStatusCode());
    sendResponse(clientSockFd, response);

    if (response.shouldCloseConnection())
        return setPollRevents(clientSockFd, POLLHUP);
    if (response.getStatusCode() >= 400 && response.getStatusCode() < 600)
        return setPollRevents(clientSockFd, POLLERR);
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
            logger->log(Logger::ERROR, "Erro ao enviar resposta para o cliente socket fd " + itostr(clientSockFd));
            return setPollRevents(clientSockFd, POLLERR);
        }
        if (sent == 0)
            return setPollRevents(clientSockFd, POLLHUP);
        totalSent += sent;
    }
    return setPollRevents(clientSockFd, POLLIN);
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
