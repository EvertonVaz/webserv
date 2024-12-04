/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MockConfigParser.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 19:16:16 by Everton           #+#    #+#             */
/*   Updated: 2024/12/03 16:24:12 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../../srcs/parser/ConfigParser.hpp"


class MockConfigParser : public ConfigParser {
    public:
        MockConfigParser* parser;

        std::vector<ServerConfig> getServers() const {
            return servers;
        }

        void addServer(const ServerConfig &config) {
            servers.push_back(config);
        }

        MockConfigParser* createDefaultParser(
            std::string root = "./var/www/html",
            std::string host = "127.0.0.1",
            std::string listen = "8080",
            std::string route = "/",
            std::string index = "index.html",
            std::string cgiExtensions = ".cgi",
            std::vector<std::string> methods = {"GET", "POST", "DELETE"}

        ) {
            parser = new MockConfigParser();
            ServerConfig serverConfig;
            RouteConfig routeConfig;

            routeConfig.setRoot(root);
            routeConfig.setMethods(methods[0]);
            routeConfig.setMethods(methods[1]);
            routeConfig.setMethods(methods[2]);
            routeConfig.setIndex(index);
            routeConfig.setCgiExtension(cgiExtensions);
            serverConfig.setHost(host);
            serverConfig.setListen(listen);
            serverConfig.addRoute(route, routeConfig);
            parser->addServer(serverConfig);

            return parser;
        }

        ~MockConfigParser() {}
    private:
        std::vector<ServerConfig> servers;
};
