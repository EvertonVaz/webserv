/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testConfig.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 18:32:14 by Everton           #+#    #+#             */
/*   Updated: 2024/10/28 22:29:28 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/basic.hpp"
#include "../../srcs/parser/ConfigParser.hpp"

std::string path_configs = "/home/etovaz/nave/webserver/tests/configs/";

bool testConfigValidConfigParsing() {
    ConfigParser parser;
    try {
        parser.loadConfig(path_configs + "valid_config.conf");
    } catch (const std::exception& e) {
        ASSERT_TRUE(false, "Exceção lançada durante o parsing: " + std::string(e.what()));
    }

    std::vector<ServerConfig> servers = parser.getServers();
    ASSERT_TRUE(servers.size() >= 1, "Deve haver uma configuração de servidor");

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
        parser.loadConfig(path_configs + "missing_semicolon.conf");
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
        parser.loadConfig(path_configs + "unmatched_braces.conf");
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
        parser.loadConfig(path_configs + "invalid_directive.conf");
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
        parser.loadConfig(path_configs + "missing_required_directive.conf");
        ASSERT_TRUE(false, "O parser deveria ter lançado uma exceção devido à diretiva obrigatória ausente");
    } catch (const std::exception& e) {
        std::string errorMsg = e.what();
        ASSERT_TRUE(errorMsg.find("Server block missing listen directive") != std::string::npos, "A mensagem de erro deve indicar diretiva obrigatória ausente");
    }

    return true;
}

bool testConfigEmptyConfigFile() {
    ConfigParser parser;
    try {
        parser.loadConfig(path_configs + "empty_config.conf");
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
        parser.loadConfig(path_configs + "multiple_server_block.conf");
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

bool testConfigAutoindexOnOff() {
    ConfigParser parser;
    try {
        parser.loadConfig(path_configs + "autoindex_on_off.conf");
    } catch (const std::exception& e) {
        ASSERT_TRUE(false, "Exceção lançada durante o parsing: " + std::string(e.what()));
    }

    std::vector<ServerConfig> servers = parser.getServers();
    ASSERT_TRUE(servers.size() == 1, "Deve haver uma configuração de servidor");

    ServerConfig server = servers[0];
    std::map<std::string, RouteConfig> routes = server.getRoutes();

    RouteConfig routeOn = routes["/autoindex_on"];
    ASSERT_TRUE(routeOn.getAutoindex() == true, "Autoindex deve estar ativado na rota /autoindex_on");

    RouteConfig routeOff = routes["/autoindex_off"];
    ASSERT_TRUE(routeOff.getAutoindex() == false, "Autoindex deve estar desativado na rota /autoindex_off");

    return true;
}

bool testConfigUploadEnable() {
    ConfigParser parser;
    try {
        parser.loadConfig(path_configs + "upload_enable.conf");
    } catch (const std::exception& e) {
        ASSERT_TRUE(false, "Exceção lançada durante o parsing: " + std::string(e.what()));
    }

    std::vector<ServerConfig> servers = parser.getServers();
    ASSERT_TRUE(servers.size() == 1, "Deve haver uma configuração de servidor");

    ServerConfig server = servers[0];
    std::map<std::string, RouteConfig> routes = server.getRoutes();

    RouteConfig route = routes["/upload"];
    ASSERT_TRUE(route.getUploadEnable() == true, "Upload deve estar ativado na rota /upload");
    ASSERT_TRUE(route.getUploadPath() == "/var/www/uploads", "Caminho de upload deve ser /var/www/uploads");

    return true;
}

bool testConfigCgiExtensions() {
    ConfigParser parser;
    try {
        parser.loadConfig(path_configs + "cgi_extensions.conf");
    } catch (const std::exception& e) {
        ASSERT_TRUE(false, "Exceção lançada durante o parsing: " + std::string(e.what()));
    }

    std::vector<ServerConfig> servers = parser.getServers();
    ASSERT_TRUE(servers.size() == 1, "Deve haver uma configuração de servidor");

    ServerConfig server = servers[0];
    std::map<std::string, RouteConfig> routes = server.getRoutes();

    RouteConfig route = routes["/cgi"];
    std::set<std::string> cgiExtensions = route.getCgiExtensions();
    ASSERT_TRUE(cgiExtensions.size() == 2, "Deve suportar duas extensões CGI");
    ASSERT_TRUE(cgiExtensions.count(".php") == 1, "Deve suportar extensão .php");
    ASSERT_TRUE(cgiExtensions.count(".py") == 1, "Deve suportar extensão .py");

    return true;
}

bool testConfigReturnCodes() {
    ConfigParser parser;
    try {
        parser.loadConfig(path_configs + "return_codes.conf");
    } catch (const std::exception& e) {
        ASSERT_TRUE(false, "Exceção lançada durante o parsing: " + std::string(e.what()));
    }

    std::vector<ServerConfig> servers = parser.getServers();
    ASSERT_TRUE(servers.size() == 1, "Deve haver uma configuração de servidor");

    ServerConfig server = servers[0];
    std::map<std::string, RouteConfig> routes = server.getRoutes();

    RouteConfig route = routes["/errors"];
    std::map<int, std::string> returnCodes = route.getReturnCodes();
    ASSERT_TRUE(returnCodes.size() == 2, "Deve haver dois códigos de retorno");

    ASSERT_TRUE(returnCodes.count(404) == 1, "Deve conter código de retorno 404");
    ASSERT_TRUE(returnCodes[404] == "/404.html", "Código 404 deve redirecionar para /404.html");

    ASSERT_TRUE(returnCodes.count(500) == 1, "Deve conter código de retorno 500");
    ASSERT_TRUE(returnCodes[500] == "/500.html", "Código 500 deve redirecionar para /500.html");

    return true;
}

bool testConfigInvalidAutoindexValue() {
    ConfigParser parser;
    try {
        parser.loadConfig(path_configs + "invalid_autoindex.conf");
        ASSERT_TRUE(false, "O parser deveria ter lançado uma exceção devido a um valor inválido para autoindex");
    } catch (const std::exception& e) {
        std::string errorMsg = e.what();
        ASSERT_TRUE(errorMsg.find("Invalid value for autoindex") != std::string::npos, "A mensagem de erro deve indicar valor inválido para autoindex");
    }

    return true;
}

bool testConfigInvalidUploadEnableValue() {
    ConfigParser parser;
    try {
        parser.loadConfig(path_configs + "invalid_upload_enable.conf");
        ASSERT_TRUE(false, "O parser deveria ter lançado uma exceção devido a um valor inválido para upload_enable");
    } catch (const std::exception& e) {
        std::string errorMsg = e.what();
        ASSERT_TRUE(errorMsg.find("Invalid value for upload_enable") != std::string::npos, "A mensagem de erro deve indicar valor inválido para upload_enable");
    }

    return true;
}

bool testConfigMissingUploadPath() {
    ConfigParser parser;
    try {
        parser.loadConfig(path_configs + "missing_upload_path.conf");
        ASSERT_TRUE(false, "O parser deveria ter lançado uma exceção devido ao upload_path ausente");
    } catch (const std::exception& e) {
        std::string errorMsg = e.what();
        ASSERT_TRUE(errorMsg.find("Location block missing upload_path directive") != std::string::npos, "A mensagem de erro deve indicar upload_path sem upload_enable ativado");
    }

    return true;
}

bool testConfigInvalidReturnCode() {
    ConfigParser parser;
    try {
        parser.loadConfig(path_configs + "invalid_return_code.conf");
        ASSERT_TRUE(false, "O parser deveria ter lançado uma exceção devido a um código de retorno inválido");
    } catch (const std::exception& e) {
        std::string errorMsg = e.what();
        ASSERT_TRUE(errorMsg.find("Invalid return code") != std::string::npos, "A mensagem de erro deve indicar código de retorno inválido");
    }

    return true;
}

bool testConfigDuplicateServerNames() {
    ConfigParser parser;
    try {
        parser.loadConfig(path_configs + "duplicate_server_names.conf");
    } catch (const std::exception& e) {
        ASSERT_TRUE(false, "Exceção lançada durante o parsing: " + std::string(e.what()));
    }

    std::vector<ServerConfig> servers = parser.getServers();
    ASSERT_TRUE(servers.size() == 2, "Deve haver duas configurações de servidor");

    ServerConfig server1 = servers[0];
    ServerConfig server2 = servers[1];

    ASSERT_TRUE(server1.getServerName()[0] == "example.com", "O primeiro servidor deve ter o nome example.com");
    ASSERT_TRUE(server2.getServerName()[0] == "example.com", "O segundo servidor deve ter o nome example.com");

    return true;
}

bool testConfigAllDirectives() {
    ConfigParser parser;
    try {
        parser.loadConfig(path_configs + "all_directives.conf");
    } catch (const std::exception& e) {
        ASSERT_TRUE(false, "Exceção lançada durante o parsing: " + std::string(e.what()));
    }

    std::vector<ServerConfig> servers = parser.getServers();
    ASSERT_TRUE(servers.size() == 1, "Deve haver uma configuração de servidor");

    ServerConfig server = servers[0];
    ASSERT_TRUE(server.getPort() == 80, "A porta do servidor deve ser 80");
    ASSERT_TRUE(server.getHost() == "0.0.0.0", "O host do servidor deve ser 0.0.0.0");

    std::vector<std::string> serverNames = server.getServerName();
    ASSERT_TRUE(serverNames.size() == 2, "Deve haver dois nomes de servidor");
    ASSERT_TRUE(serverNames[0] == "example.com", "O primeiro nome do servidor deve ser example.com");
    ASSERT_TRUE(serverNames[1] == "www.example.com", "O segundo nome do servidor deve ser www.example.com");

    std::map<std::string, RouteConfig> routes = server.getRoutes();
    ASSERT_TRUE(routes.size() == 2, "Deve haver duas configurações de rota");

    RouteConfig route1 = routes["/"];
    ASSERT_TRUE(route1.getRoot() == "/var/www/html", "A raiz da rota / deve ser /var/www/html");
    ASSERT_TRUE(route1.getAutoindex() == false, "Autoindex deve estar desativado na rota /");

    RouteConfig route2 = routes["/images"];
    ASSERT_TRUE(route2.getRoot() == "/var/www/images", "A raiz da rota /images deve ser /var/www/images");
    ASSERT_TRUE(route2.getAutoindex() == true, "Autoindex deve estar ativado na rota /images");

    return true;
}

bool testConfigInvalidHost() {
    ConfigParser parser;
    try {
        parser.loadConfig(path_configs + "invalid_host.conf");
        ASSERT_TRUE(false, "O parser deveria ter lançado uma exceção devido a um host inválido");
    } catch (const std::exception& e) {
        std::string errorMsg = e.what();
        ASSERT_TRUE(errorMsg.find("Invalid host") != std::string::npos, "A mensagem de erro deve indicar host inválido");
    }

    return true;
}

bool testConfigInvalidPort() {
    ConfigParser parser;
    try {
        parser.loadConfig(path_configs + "invalid_port.conf");
        ASSERT_TRUE(false, "O parser deveria ter lançado uma exceção devido a uma porta inválida");
    } catch (const std::exception& e) {
        std::string errorMsg = e.what();
        ASSERT_TRUE(errorMsg.find("Invalid port") != std::string::npos, "A mensagem de erro deve indicar porta inválida\nesperado: Invalid port\nrecebido: " + errorMsg);
    }

    return true;
}

bool testErrorPage() {
    ConfigParser parser;
    try {
        parser.loadConfig(path_configs + "error_page.conf");
        ServerConfig server = parser.getServers()[0];
        std::map<int, std::string> errorPages = server.getErrorPage();
        ASSERT_TRUE(errorPages.size() == 5, "Deve haver cinco páginas de erro configuradas");
        ASSERT_TRUE(errorPages[404] == "/404.html", "esperado: /404.html - real: " + errorPages[404]);
        ASSERT_TRUE(errorPages[504] == "/504.html", "esperado: /504.html - real: " + errorPages[504]);
    } catch (const std::exception& e) {
        ASSERT_TRUE(false, "Exceção lançada durante o parsing: " + std::string(e.what()));
    }
    return true;
}
