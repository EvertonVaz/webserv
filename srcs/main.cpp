/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 14:14:57 by Everton           #+#    #+#             */
/*   Updated: 2024/11/01 09:06:43 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./parser/ConfigParser.hpp"
#include <iostream>
#include "./server/Server.hpp"
#include "./connection/ConnectionManager.hpp"

int main(int argc, char **argv) {
    try {
        if (argc < 2) {
            std::cerr << "Usage: " << argv[0] << " <config_file>" << std::endl;
            exit(EXIT_FAILURE);
        }
        ConfigParser parser;
        parser.loadConfig(argv[1]);

        Server server(parser, NULL);

        std::cout << *server.getServers()[0].getRoutes()["/"].getIndex().begin() << std::endl;
        if (server.isListening()) {
            std::cout << "Servidor iniciado com sucesso." << std::endl;
        } else {
            std::cout << "Falha ao iniciar o servidor." << std::endl;
            exit(EXIT_FAILURE);
        }

        ConnectionManager connManager(server);
        connManager.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
    return 0;
}

