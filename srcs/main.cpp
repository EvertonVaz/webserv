/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 14:14:57 by Everton           #+#    #+#             */
/*   Updated: 2024/12/08 10:53:50 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdexcept>
#include <csignal>
#include "./server/Server.hpp"
#include "./logger/Logger.hpp"
#include "./parser/ConfigParser.hpp"
#include "./connection/ConnectionManager.hpp"
#include "aux.hpp"

void ignoreSigPipe() {
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGPIPE, &sa, NULL);
    return ;
}

void signalHandler(int signal) {
    (void)signal;
    throw std::runtime_error("Bye Bye :)");
}

void printLogs(const Server& server) {
    Logger& logger = Logger::getInstance();
    std::string log = server.getServers()[0].getHost() + ":";
    log += itostr(server.getServers()[0].getPort());
    logger.log(Logger::INFO, "Servidor iniciado com sucesso.");
    logger.log(Logger::INFO, "Acesse: http://" + log);
}

int main(int argc, char **argv){
    ignoreSigPipe();
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    Logger& logger = Logger::getInstance();
    try {
        if (argc > 3) {
            std::string msg = "Usage: " + std::string(argv[0]) + " <config_file>";
            throw std::invalid_argument(msg + " [log_file]");
        }
        if (argc == 3)
            logger.setLogFile(argv[2]);
        ConfigParser parser;
        parser.loadConfig(argv[1]);
        Server server(parser, NULL);
        if (!server.isListening()) {
            throw std::runtime_error("Erro ao iniciar o servidor.");
        }
        printLogs(server);
        ConnectionManager connManager(server);
        connManager.run();
    } catch (const std::exception& e) {
        logger.log(Logger::ERROR, e.what());
        return 1;
    }
    return 0;
}

