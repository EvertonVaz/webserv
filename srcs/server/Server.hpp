/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 09:24:07 by Everton           #+#    #+#             */
/*   Updated: 2024/11/08 14:31:11 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../parser/ConfigParser.hpp"
#include "../interfaces/ISocket.hpp"
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <cstring>
#include <arpa/inet.h>
#include <sys/poll.h>

class Server
{
	protected:
		bool ownSocket;
		std::vector<int> listenSockets;
		std::vector<ServerConfig> servers;
		ISocket *socketInterface;

	public:
		Server(const ConfigParser &parser, ISocket *socket = NULL);
		virtual ~Server();

		void configureSocket(int sockfd, int &opt);
		void setupSocket(int sockfd, const ServerConfig &serverConfig, struct sockaddr_in &addr);
		void initServer(const ServerConfig &serverConfig);
		bool isListening() const;
		virtual std::vector<int> getListenSockets() const;
		virtual std::vector<ServerConfig> getServers() const;
		virtual ISocket *getSocketInterface() const;

		bool operator==(const Server &other) const;
		bool operator!=(const Server &other) const;
		Server &operator=(const Server &other);
		bool operator<(const Server &other) const;
		bool operator>(const Server &other) const;
};
