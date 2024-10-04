/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 19:06:31 by Everton           #+#    #+#             */
/*   Updated: 2024/10/04 20:09:58 by Everton          ###   ########.fr       */
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
		int max_body_size;
        std::vector<std::string> server_names;
        std::pair<int, std::string> error_page;
        std::map<std::string, RouteConfig> routes;

    public:
		ServerConfig() {
			host = "127.0.0.1";
			root = "./";
		};

        int getPort() const;
        std::string getHost() const;
		std::string getRoot() const;
        std::vector<std::string> getServerName() const;
		std::pair<int, std::string> getErrorPage() const;
		std::map<std::string, RouteConfig> getRoutes() const;

		void setListen(std::string value);
		void setHost(const std::string& host);
		void setRoot(const std::string& root);
		void setMaxBodySize(int max_body_size);
		void setErrorPage(std::string error_page);
		void setServerName(const std::string& server_name);
		void addRoute(const std::string& path, const RouteConfig& route);
};