/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 14:14:57 by Everton           #+#    #+#             */
/*   Updated: 2024/11/18 10:53:41 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdexcept>
#include "./server/Server.hpp"
#include "./logger/Logger.hpp"
#include "./parser/ConfigParser.hpp"
#include "./connection/ConnectionManager.hpp"

int main(int argc, char **argv) {
    Logger& logger = Logger::getInstance();
    try {
        if (argc < 2 && argc > 2) {
            std::string msg = "Usage: " + std::string(argv[0]) + " <config_file>";
            throw std::invalid_argument(msg);
        }
        // TODO: Criar uma logica para salvar o log em um arquivo
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
        logger.log(Logger::ERROR, e.what());
        return 1;
    }
    return 0;
}

