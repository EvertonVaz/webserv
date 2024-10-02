/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 14:14:57 by Everton           #+#    #+#             */
/*   Updated: 2024/10/02 14:56:09 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"
#include <iostream>

int main() {
    std::vector<ServerConfig> servers;
    ConfigParser parser("config.web");

    if (parser.parse(servers)) {
        for (size_t i = 0; i < servers.size(); ++i) {
            const ServerConfig& server = servers[i];
            std::cout << "Server " << i + 1 << ":" << std::endl;
            std::cout << "  Listen: " << server.listen << std::endl;
            std::cout << "  Server Name: " << server.server_name << std::endl;
            std::cout << "  Root: " << server.root << std::endl;
            std::cout << "  Error Page: " << server.error_page << std::endl;

            for (size_t j = 0; j < server.routes.size(); ++j) {
                const RouteConfig& route = server.routes[j];
                std::cout << "  Route " << j + 1 << ":" << std::endl;
                std::cout << "    Path: " << route.path << std::endl;
                std::cout << "    Methods: " << route.methods << std::endl;
                std::cout << "    Upload Directory: " << route.upload_directory << std::endl;
                std::cout << "    Index: " << route.index << std::endl;
            }
        }
    } else {
        std::cerr << "Failed to parse configuration file." << std::endl;
    }

    return 0;
}