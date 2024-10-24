/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 09:27:24 by Everton           #+#    #+#             */
/*   Updated: 2024/10/24 11:31:17 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>



Server::Server(const ConfigParser &parser)
{
	servers = parser.getServers();
	for (size_t i = 0; i < servers.size(); i++)
		initServer(servers[i]);
}

Server::~Server()
{
	for (size_t i = 0; i < listenSockets.size(); i++)
		close(listenSockets[i]);
}

static inline void configureSocket(int sockfd, int &opt) {
    if (sockfd < 0) {
        throw std::runtime_error("Failed to create socket");
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, (char *)&opt, sizeof(opt)) < 0) {
        close(sockfd);
        throw std::runtime_error("Failed to set SO_REUSEADDR");
    }

    if (fcntl(sockfd, F_SETFL, O_NONBLOCK) < 0) {
        close(sockfd);
        throw std::runtime_error("Failed to make socket non-blocking");
    }
}

static inline void setupSocket(int sockfd, const ServerConfig &serverConfig, struct sockaddr_in &addr) {
	if (inet_pton(AF_INET, serverConfig.getHost().c_str(), &addr.sin_addr) <= 0) {
		close(sockfd);
		throw std::runtime_error("inet_pton failed");

	}

	if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		close(sockfd);
		throw std::runtime_error("bind failed");

	}

	if (listen(sockfd, SOMAXCONN) < 0) {
		close(sockfd);
		throw std::runtime_error("listen failed");

	}
}

void Server::initServer(const ServerConfig &serverConfig) {
	int opt = 1;
	struct sockaddr_in addr;
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

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

void Server::run(void) {
	std::vector<struct pollfd> pollFds(listenSockets.size());
	for (size_t i = 0; i < listenSockets.size(); i++) {
		pollFds[i].fd = listenSockets[i];
		pollFds[i].events = POLLIN;
	}

	while (true) {
		int ret = poll(pollFds.data(), pollFds.size(), -1);
		if (ret < 0) {
			throw std::runtime_error("poll failed");
			break;
		}

		for (size_t i = 0; i < pollFds.size(); i++) {
			if (pollFds[i].revents & POLLIN) {
				int newSocket;
				struct sockaddr_in clientAddr;
				socklen_t clientLen = sizeof(clientAddr);
				newSocket = accept(listenSockets[i], (struct sockaddr *)&clientAddr, &clientLen);
				if(newSocket < 0)
					throw std::runtime_error("accept failed");

				std::cout << "New connection from " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << std::endl;
				close(newSocket);
			}
		}
	}
}