/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 09:24:07 by Everton           #+#    #+#             */
/*   Updated: 2024/10/25 14:25:42 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parse_config/ConfigParser.hpp"
#include "SocketImp.hpp"
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <cstring>
#include <arpa/inet.h>
#include <sys/poll.h>

class Server
{
	private:
		std::vector<int> listenSockets;
		std::vector<ServerConfig> servers;
		ISocket *socketInterface;

	public:
		Server(const ConfigParser &parser, ISocket *socket = new SocketImp());
		~Server();
		void configureSocket(int sockfd, int &opt);
		void setupSocket(int sockfd, const ServerConfig &serverConfig, struct sockaddr_in &addr);
		void initServer(const ServerConfig &serverConfig);
		bool isListening() const;
		void run(void);
};
