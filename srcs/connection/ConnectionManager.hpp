/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectionManager.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 12:37:07 by Everton           #+#    #+#             */
/*   Updated: 2024/11/01 09:24:28 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <vector>
#include <poll.h>
#include <string>
#include "../interfaces/ISocket.hpp"
#include "../server/Server.hpp"
#include "../http/HTTPRequest.hpp"
#include "../http/HTTPResponse.hpp"

class ConnectionManager {
	private:
		ISocket *socketInterface;
		std::set<int> listenSockets;
		std::vector<struct pollfd> pollFds;
		std::map<int, HTTPRequest> requests;
		std::map<int, std::string> clientBuffers;
		std::vector<ServerConfig> serverConfigs;

	public:
		ConnectionManager(Server &servers);
		~ConnectionManager();
		void run();
		void handleEvents();
		void acceptNewConnection(int listenSocketFd);
		void readFromClient(int clientSocketFd);
		void processRequest(int clientSockFd, const HTTPRequest& request);
		void writeToClient(int clientSocketFd);
		void closeConnection(int clientSocketFd);

		std::set<int> getListenSockets() const;
        std::vector<struct pollfd> getPollFds() const;
        std::map<int, std::string> getClientBuffers() const;

		void setPollFds(std::vector<struct pollfd> pollFds);
		void setClientBuffers(std::map<int, std::string> clientBuffers);

		const RouteConfig routeRequest(const HTTPRequest& request, ServerConfig& serverConfig);
    	void handleRequest(const HTTPRequest& request, HTTPResponse& response, ServerConfig& serverConfig);
		void resolvePath(std::string path, const RouteConfig routeConfig, HTTPResponse& response);
};