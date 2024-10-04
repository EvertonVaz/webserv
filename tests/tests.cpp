/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 18:32:14 by Everton           #+#    #+#             */
/*   Updated: 2024/10/04 11:15:27 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "basic.hpp"
#include "../srcs/configs/ConfigParser.hpp"

std::string path = "/home/etovaz/nave/webserver/tests/";

bool testValidConfigParsing() {
    ConfigParser parser;
    try {
        parser.loadConfig(path + "valid_config.conf");
    } catch (const std::exception& e) {
        ASSERT_TRUE(false, "Exceção lançada durante o parsing: " + std::string(e.what()));
    }

    std::vector<ServerConfig> servers = parser.getServers();
    ASSERT_TRUE(servers.size() == 1, "Deve haver uma configuração de servidor");

    ServerConfig server = servers[0];
    ASSERT_TRUE(server.getPort() == 8080, "A porta do servidor deve ser 8080");
    ASSERT_TRUE(server.getHost() == "127.0.0.1", "O host do servidor deve ser 127.0.0.1");

    std::string serverName = server.getServerName();
    std::cout << "Valor de serverName: '" << serverName << "'" << std::endl;
    ASSERT_TRUE(serverName == "localhost", "O nome do servidor deve ser localhost");

    std::map<std::string, RouteConfig> routes = server.getRoutes();
    ASSERT_TRUE(routes.size() == 1, "Deve haver uma configuração de rota");

    RouteConfig route = routes["/"];
    ASSERT_TRUE(route.getRoot() == "/var/www/html", "A raiz da rota deve ser /var/www/html");
    ASSERT_TRUE(route.getMethods().size() == 2, "A rota deve aceitar dois métodos");
    ASSERT_TRUE(route.getMethods().count("GET") == 1, "A rota deve aceitar o método GET");
    ASSERT_TRUE(route.getMethods().count("POST") == 1, "A rota deve aceitar o método POST");

    return true;
}

bool testMissingSemicolon() {
    ConfigParser parser;
    try {
        parser.loadConfig(path + "missing_semicolon.conf");
        ASSERT_TRUE(false, "O parser deveria ter lançado uma exceção devido à falta de ponto e vírgula");
    } catch (const std::exception& e) {
        std::string errorMsg = e.what();
        ASSERT_TRUE(errorMsg.find("Missing semicolon in directive:") != std::string::npos, "A mensagem de erro deve indicar falta de ponto e vírgula");
    }

    return true;
}

bool testUnmatchedBraces() {
    ConfigParser parser;
    try {
        parser.loadConfig(path + "unmatched_braces.conf");
        ASSERT_TRUE(false, "O parser deveria ter lançado uma exceção devido a chaves não balanceadas");
    } catch (const std::exception& e) {
        std::string errorMsg = e.what();
        ASSERT_TRUE(errorMsg.find("Mismatched braces in configuration file.") != std::string::npos, "A mensagem de erro deve indicar chaves não balanceadas");
    }

    return true;
}

bool testInvalidDirective() {
    ConfigParser parser;
    try {
        parser.loadConfig(path + "invalid_directive.conf");
        ASSERT_TRUE(false, "O parser deveria ter lançado uma exceção devido a diretiva inválida");
    } catch (const std::exception& e) {
        std::string errorMsg = e.what();
        ASSERT_TRUE(errorMsg.find("Diretiva desconhecida") != std::string::npos, "A mensagem de erro deve indicar diretiva desconhecida");
    }

    return true;
}

bool testMissingRequiredDirective() {
    ConfigParser parser;
    try {
        parser.loadConfig(path + "missing_required_directive.conf");
        ASSERT_TRUE(false, "O parser deveria ter lançado uma exceção devido à diretiva obrigatória ausente");
    } catch (const std::exception& e) {
        std::string errorMsg = e.what();
        ASSERT_TRUE(errorMsg.find("Diretiva obrigatória ausente") != std::string::npos, "A mensagem de erro deve indicar diretiva obrigatória ausente");
    }

    return true;
}
