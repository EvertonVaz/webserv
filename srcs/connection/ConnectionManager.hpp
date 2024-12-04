/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectionManager.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 12:37:07 by Everton           #+#    #+#             */
/*   Updated: 2024/12/04 08:56:05 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <vector>
#include <poll.h>
#include <string>
#include "../server/Server.hpp"
#include "../router/Router.hpp"
#include "../logger/Logger.hpp"
#include "../http/HTTPRequest.hpp"
#include "../interfaces/ISocket.hpp"

class ConnectionManager {
	private:
		ISocket *socketInterface;
		std::set<int> listenSockets;
		std::vector<struct pollfd> pollFds;
		std::map<int, HTTPRequest> requests;
		std::map<int, std::string> clientBuffers;
		std::vector<ServerConfig> serverConfigs;
		Router router;
		Logger* logger;

		void handleReadError(int clientSockFd, const HTTPRequest& request);

	public:
		ConnectionManager(Server &servers);
		~ConnectionManager();
		void run();
		void handleEvents();
		void acceptNewConnection(int listenSocketFd);
		void readFromClient(int clientSocketFd);
		void processRequest(int clientSockFd, HTTPRequest& request);
		void sendResponse(int clientSockFd, HTTPResponse& response);
		void closeConnection(int clientSocketFd);

		std::set<int> getListenSockets() const;
        std::vector<struct pollfd> getPollFds() const;
        std::map<int, std::string> getClientBuffers() const;

		void setPollFds(std::vector<struct pollfd> pollFds);
		void setPollRevents(int clientSockFd, short revents);
		void setClientBuffers(std::map<int, std::string> clientBuffers);
};
