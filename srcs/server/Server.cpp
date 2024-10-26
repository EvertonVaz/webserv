/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 09:27:24 by Everton           #+#    #+#             */
/*   Updated: 2024/10/26 11:48:11 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Server.hpp"
#include "SocketImp.hpp"

Server::Server(const ConfigParser &parser, ISocket *socket) {
	if (!socket) {
		socketInterface = new SocketImp();
		ownSocket = true;
	}
	else {
		socketInterface = socket;
		ownSocket = false;
	}
	servers = parser.getServers();
	for (size_t i = 0; i < servers.size(); i++)
		initServer(servers[i]);
}

Server::~Server() {
	for (size_t i = 0; i < listenSockets.size(); i++)
		socketInterface->close(listenSockets[i]);
	if (ownSocket)
		delete socketInterface;
}

void Server::configureSocket(int sockfd, int &opt) {
    if (sockfd < 0) {
        throw std::runtime_error("Failed to create socket");
    }

    if (socketInterface->setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, (char *)&opt, sizeof(opt)) < 0) {
        socketInterface->close(sockfd);
        throw std::runtime_error("Failed to set SO_REUSEADDR");
    }

    if (fcntl(sockfd, F_SETFL, O_NONBLOCK) < 0) {
        socketInterface->close(sockfd);
        throw std::runtime_error("Failed to make socket non-blocking");
    }
}

void Server::setupSocket(int sockfd, const ServerConfig &serverConfig, struct sockaddr_in &addr) {
	if (inet_pton(AF_INET, serverConfig.getHost().c_str(), &addr.sin_addr) <= 0) {
		socketInterface->close(sockfd);
		throw std::runtime_error("inet_pton failed");

	}

	if (socketInterface->bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		socketInterface->close(sockfd);
		throw std::runtime_error("bind failed");

	}

	if (socketInterface->listen(sockfd, SOMAXCONN) < 0) {
		socketInterface->close(sockfd);
		throw std::runtime_error("listen failed");

	}
}

void Server::initServer(const ServerConfig &serverConfig) {
	int opt = 1;
	struct sockaddr_in addr;
	int sockfd = socketInterface->socket(AF_INET, SOCK_STREAM, 0);

	configureSocket(sockfd, opt);

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(serverConfig.getPort());

	setupSocket(sockfd, serverConfig, addr);

	listenSockets.push_back(sockfd);
	std::cout << "Servidor escutando em " << serverConfig.getHost() << ":" << serverConfig.getPort() << std::endl;
}

bool Server::isListening() const {
	return !listenSockets.empty();
}

std::vector<int> Server::getListenSockets() const {
	return listenSockets;
}

std::vector<ServerConfig> Server::getServers() const {
	return servers;
}

ISocket *Server::getSocketInterface() const {
	return socketInterface;
}