/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 14:14:57 by Everton           #+#    #+#             */
/*   Updated: 2024/11/17 21:23:01 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./parser/ConfigParser.hpp"
#include <iostream>
#include <stdexcept>
#include "./server/Server.hpp"
#include "./connection/ConnectionManager.hpp"
#include "./logger/Logger.hpp"

int main(int argc, char **argv) {
    try {
        if (argc < 2 && argc > 2) {
            std::string msg = "Usage: " + std::string(argv[0]) + " <config_file>";
            throw std::invalid_argument(msg);
        }
        Logger& logger = Logger::getInstance();
        // logger.setLogFile("server.log");

        ConfigParser parser;
        parser.loadConfig(argv[1]);

        Server server(parser, NULL);

        if (!server.isListening()) {
            throw std::runtime_error("Erro ao iniciar o servidor.");
        }
        logger.log(Logger::INFO, "Servidor iniciado com sucesso.");

        ConnectionManager connManager(server);
        connManager.run();
    } catch (const std::exception& e) {
        Logger::getInstance().log(Logger::ERROR, e.what());
        return 1;
    }
    return 0;
}

