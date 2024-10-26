/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 14:14:57 by Everton           #+#    #+#             */
/*   Updated: 2024/10/26 10:41:44 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./parse_config/ConfigParser.hpp"
#include <iostream>
#include "./server/Server.hpp"

void run(Server &servers) {
    std::vector<int> listenSockets = servers.getListenSockets();
    ISocket *socketInterface = servers.getSocketInterface();
    std::vector<struct pollfd> pollFds(listenSockets.size());
    for (size_t i = 0; i < listenSockets.size(); i++) {
        pollFds[i].fd = listenSockets[i];
        pollFds[i].events = POLLIN;
    }

    while (true) {
        int ret = poll(pollFds.data(), pollFds.size(), -1);
        if (ret < 0) {
            throw std::runtime_error("poll failed");
            break;
        }

        for (size_t i = 0; i < pollFds.size(); i++) {
            if (pollFds[i].revents & POLLIN) {
                int newSocket;
                struct sockaddr_in clientAddr;
                socklen_t clientLen = sizeof(clientAddr);
                newSocket = socketInterface->accept(
                    listenSockets[i],
                    (struct sockaddr *)&clientAddr,
                    &clientLen
                );
                if(newSocket < 0)
                    throw std::runtime_error("accept failed");

                std::cout << "New connection from " << inet_ntoa(clientAddr.sin_addr);
                std::cout << ":" << ntohs(clientAddr.sin_port) << std::endl;

                char buffer[1024];
                int bytesRead = socketInterface->recv(newSocket, buffer, sizeof(buffer) - 1, 0);
                if (bytesRead < 0) {
                    throw std::runtime_error("recv failed");
                }
                buffer[bytesRead] = '\0';
                std::cout << "Received: " << buffer << std::endl;
                if (std::string(buffer).find("down") != std::string::npos) {
                    std::cout << "Shutdown command received. Closing server." << std::endl;
                    socketInterface->close(newSocket);
                    return;
                }

                socketInterface->close(newSocket);
            }
        }
    }
}

int main(int argc, char **argv) {
    try {
        if (argc < 2) {
            std::cerr << "Usage: " << argv[0] << " <config_file>" << std::endl;
            exit(EXIT_FAILURE);
        }
        ConfigParser parser;
        parser.loadConfig(argv[1]);

        Server server(parser, NULL);

        if (server.isListening()) {
            std::cout << "Servidor iniciado com sucesso." << std::endl;
        } else {
            std::cout << "Falha ao iniciar o servidor." << std::endl;
            exit(EXIT_FAILURE);
        }

        run(server);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
    return 0;
}

