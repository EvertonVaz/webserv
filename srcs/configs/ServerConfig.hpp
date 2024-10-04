/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 19:06:31 by Everton           #+#    #+#             */
/*   Updated: 2024/10/03 22:35:53 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <string>
#include "RouteConfig.hpp"

class ServerConfig {
    private:
        int listen;
		std::string root;
		std::string host;
        std::string error_page;
        std::string server_name;
        std::map<std::string, RouteConfig> routes;

    public:
		ServerConfig() {
			host = "127.0.0.1";
			root = "./";
		};
        int getPort() const {
			return listen;
		};
        std::string getHost() const {
			return host;
		};
        std::string getServerName() const {
			return server_name;
		};
		std::string getErrorPage() const {
			return error_page;
		};
		std::string getRoot() const {
			return root;
		};
		std::map<std::string, RouteConfig> getRoutes() const {
			return routes;
		};
		void setPort(int port) {
			listen = port;
		};
		void setHost(const std::string& host) {
			this->host = host;
		};
		void setServerName(const std::string& server_name) {
			this->server_name = server_name;
		};
		void setErrorPage(const std::string& error_page) {
			this->error_page = error_page;
		};
		void setRoot(const std::string& root) {
			this->root = root;
		};
		void addRoute(const std::string& path, const RouteConfig& route) {
			routes[path] = route;
		};
	};