/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testConfig.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 18:32:14 by Everton           #+#    #+#             */
/*   Updated: 2024/10/10 20:18:24 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "basic.hpp"
#include "../srcs/configs/ConfigParser.hpp"

std::string path = "/home/etovaz/nave/webserver/tests/configs/";

bool testConfigValidConfigParsing() {
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

    std::string serverName = server.getServerName()[0];
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

bool testConfigMissingSemicolon() {
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

bool testConfigUnmatchedBraces() {
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

bool testConfigInvalidDirective() {
    ConfigParser parser;
    try {
        parser.loadConfig(path + "invalid_directive.conf");
        ASSERT_TRUE(false, "O parser deveria ter lançado uma exceção devido a diretiva inválida");
    } catch (const std::exception& e) {
        std::string errorMsg = e.what();
        ASSERT_TRUE(errorMsg.find("Unknown directive") != std::string::npos, "A mensagem de erro deve indicar diretiva desconhecida");
    }

    return true;
}

bool testConfigMissingRequiredDirective() {
    ConfigParser parser;
    try {
        parser.loadConfig(path + "missing_required_directive.conf");
        ASSERT_TRUE(false, "O parser deveria ter lançado uma exceção devido à diretiva obrigatória ausente");
    } catch (const std::exception& e) {
        std::string errorMsg = e.what();
        ASSERT_TRUE(errorMsg.find("Server block missing listen directive") != std::string::npos, "A mensagem de erro deve indicar diretiva obrigatória ausente");
    }

    return true;
}

bool testConfigInvalidValue() {
    ConfigParser parser;
    try {
        parser.loadConfig(path + "invalid_value.conf");
        ASSERT_TRUE(false, "O parser deveria ter lançado uma exceção devido a um valor inválido");
    } catch (const std::exception& e) {
        std::string errorMsg = e.what();
        ASSERT_TRUE(errorMsg.find("Invalid value") != std::string::npos, "A mensagem de erro deve indicar valor inválido");
    }

    return true;
}

bool testConfigEmptyConfigFile() {
    ConfigParser parser;
    try {
        parser.loadConfig(path + "empty_config.conf");
        ASSERT_TRUE(false, "O parser deveria ter lançado uma exceção devido ao arquivo de configuração vazio");
    } catch (const std::exception& e) {
        std::string errorMsg = e.what();
        ASSERT_TRUE(errorMsg.find("Empty configuration file") != std::string::npos, "A mensagem de erro deve indicar arquivo de configuração vazio");
    }

    return true;
}

bool testConfigMultipleServerBlocks() {
    ConfigParser parser;
    try {
        parser.loadConfig(path + "multiple_server_block.conf");
    } catch (const std::exception& e) {
        ASSERT_TRUE(false, "Exceção lançada durante o parsing: " + std::string(e.what()));
    }

    std::vector<ServerConfig> servers = parser.getServers();
    ASSERT_TRUE(servers.size() == 2, "Deve haver duas configurações de servidor");

    ServerConfig server1 = servers[0];
    ASSERT_TRUE(server1.getPort() == 8080, "A porta do primeiro servidor deve ser 8080");
    ASSERT_TRUE(server1.getHost() == "168.127.1.1", "O host do primeiro servidor deve ser 168.127.1.1");

    std::string serverName1 = server1.getServerName()[0];
    ASSERT_TRUE(serverName1 == "server.io", "O nome do primeiro servidor deve ser localhost");

    std::map<std::string, RouteConfig> routes1 = server1.getRoutes();
    ASSERT_TRUE(routes1.size() == 1, "Deve haver uma configuração de rota");

    RouteConfig route1 = routes1["/"];
    ASSERT_TRUE(route1.getRoot() == "/var/www/html", "A raiz da rota deve ser /var/www/html");
    ASSERT_TRUE(route1.getMethods().size() == 2, "A rota deve aceitar dois métodos");
    ASSERT_TRUE(route1.getMethods().count("GET") == 1, "A rota deve aceitar o método GET");
    ASSERT_TRUE(route1.getMethods().count("POST") == 1, "A rota deve aceitar o método POST");

    ServerConfig server2 = servers[1];
    ASSERT_TRUE(server2.getPort() == 8080, "A porta do primeiro servidor deve ser 8080");
    ASSERT_TRUE(server2.getHost() == "127.0.0.1", "O host do primeiro servidor deve ser 127.0.0.1");

    std::string serverName2 = server2.getServerName()[0];
    ASSERT_TRUE(serverName2 == "localhost", "O nome do primeiro servidor deve ser localhost");

    std::map<std::string, RouteConfig> routes2 = server2.getRoutes();
    ASSERT_TRUE(routes2.size() == 1, "Deve haver uma configuração de rota");

    RouteConfig route2 = routes2["/"];
    ASSERT_TRUE(route2.getRoot() == "/var/www/html", "A raiz da rota deve ser /var/www/html");
    ASSERT_TRUE(route2.getMethods().size() == 2, "A rota deve aceitar dois métodos");
    ASSERT_TRUE(route2.getMethods().count("GET") == 1, "A rota deve aceitar o método GET");
    ASSERT_TRUE(route2.getMethods().count("POST") == 1, "A rota deve aceitar o método POST");
    return true;
}