/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 09:24:07 by Everton           #+#    #+#             */
/*   Updated: 2024/10/28 19:46:14 by Everton          ###   ########.fr       */
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
};
