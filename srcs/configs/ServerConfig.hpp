/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 19:06:31 by Everton           #+#    #+#             */
/*   Updated: 2024/10/10 21:46:20 by Everton          ###   ########.fr       */
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

		void initializeDirectiveMap();
		struct DirectiveHandler {
			void (ServerConfig::*handler)(const std::string&);
		};
		std::map<std::string, DirectiveHandler> directiveMap;

    public:
		ServerConfig();

        int getPort() const;
        std::string getHost() const;
		std::string getRoot() const;
        std::vector<std::string> getServerName() const;
		std::pair<int, std::string> getErrorPage() const;
		std::map<std::string, RouteConfig> getRoutes() const;

		void setListen(const std::string& value);
		void setHost(const std::string& host);
		void setRoot(const std::string& root);
		void setMaxBodySize(const std::string& max_body_size);
		void setErrorPage(const std::string& error_page);
		void setServerName(const std::string& server_name);
		void addRoute(const std::string& path, const RouteConfig& route);

		void applyDirective(const std::string& key, const std::string& value);
};