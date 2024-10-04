/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 14:14:57 by Everton           #+#    #+#             */
/*   Updated: 2024/10/03 23:07:23 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./configs/ConfigParser.hpp"
#include <iostream>

int main() {
    std::vector<ServerConfig> servers;
    ConfigParser parser;
    parser.loadConfig("../config.web");

    servers = parser.getServers();

    for (size_t i = 0; i < servers.size(); ++i) {
        const ServerConfig& server = servers[i];
        std::cout << "Server " << i + 1 << ":" << std::endl;
        std::cout << "  Listen: " << server.getPort() << std::endl;
        std::cout << "  Server Name: " << server.getServerName() << std::endl;
        std::cout << "  Root: " << server.getRoot() << std::endl;
        std::cout << "  Error Page: " << server.getErrorPage() << std::endl;

        std::map<std::string, RouteConfig> routes = server.getRoutes();
        size_t j = 0;
        for (std::map<std::string, RouteConfig>::const_iterator it = routes.begin(); it != routes.end(); ++it, ++j) {
            const RouteConfig& route = it->second;
            std::cout << "  Route " << j + 1 << ":" << std::endl;
            std::cout << "    Path: " << it->first << std::endl;
            std::cout << "    Methods: ";
            std::set<std::string> methods = route.getMethods();
            for (std::set<std::string>::const_iterator mit = methods.begin(); mit != methods.end(); ++mit) {
                std::cout << *mit << " ";
            }
            std::cout << std::endl;
            std::cout << "    Root: " << route.getRoot() << std::endl;
        }
    }

    return 0;
}