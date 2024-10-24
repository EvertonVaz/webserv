/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 09:24:07 by Everton           #+#    #+#             */
/*   Updated: 2024/10/24 10:10:06 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parse_config/ConfigParser.hpp"
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

	public:
		Server(const ConfigParser &parser);
		~Server();
		void initServer(const ServerConfig &serverConfig);
		bool isListening() const;
		void run(void);
};
