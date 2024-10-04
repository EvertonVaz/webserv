/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 19:06:31 by Everton           #+#    #+#             */
/*   Updated: 2024/10/04 16:05:50 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <vector>
#include <string>
#include <cstdlib>
#include "RouteConfig.hpp"

class ServerConfig {
    private:
        int port;
		std::string host;
		std::string root;
        std::map<int, std::string> error_page;
        std::vector<std::string> server_names;
		int max_body_size;
        std::map<std::string, RouteConfig> routes;

    public:
		ServerConfig() {
			host = "127.0.0.1";
			root = "./";
		};
        int getPort() const {
			return port;
		};
        std::string getHost() const {
			return host;
		};
        std::vector<std::string> getServerName() const {
			return server_names;
		};
		std::map<int, std::string> getErrorPage() const {
			return error_page;
		};
		std::string getRoot() const {
			return root;
		};
		std::map<std::string, RouteConfig> getRoutes() const {
			return routes;
		};
		void fillListen(std::string value) {
			if (value.find(":") != std::string::npos) {
				std::string port_str = value.substr(value.find(":") + 1);
				port = std::atoi(port_str.c_str());
				host = value.substr(0, value.find(":"));
			} else {
				port = std::atoi(value.c_str());
			}
		};
		void setHost(const std::string& host) {
			this->host = host;
		};
		void fillServerName(const std::string& server_name) {
			std::istringstream iss(server_name);
			while (iss) {
				std::string links;
				iss >> links;
				if (!links.empty())
					server_names.push_back(links);
			}
		};
		void setErrorPage(std::string error_page) {
			std::istringstream iss(error_page);
			int error_code;
			iss >> error_code;
			iss >> error_page;
			this->error_page[error_code] = error_page;
		};
		void setRoot(const std::string& root) {
			this->root = root;
		};
		void setMaxBodySize(int max_body_size) {
			this->max_body_size = max_body_size;
		};
		void addRoute(const std::string& path, const RouteConfig& route) {
			routes[path] = route;
		};
};