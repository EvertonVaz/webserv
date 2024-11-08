/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RouterTests.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 16:10:10 by Everton           #+#    #+#             */
/*   Updated: 2024/11/08 17:21:58 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/basic.hpp"
#include "../headers/MockServer.hpp"
#include "../headers/MockConfigParser.hpp"
#include "../../srcs/router/Router.hpp"
#include "../../srcs/parser/RouteConfig.hpp"
#include "../../srcs/http/HTTPRequest.hpp"


bool test_route_request() {
	MockConfigParser parser;
    ServerConfig serverConfig;
	RouteConfig routeConfig;

	routeConfig.setRoot("./var/www/html");
	routeConfig.setMethods({"GET"});
	routeConfig.setIndex({"index.html"});
    serverConfig.setHost("127.0.0.1");
    serverConfig.setListen("8080");
    serverConfig.addRoute("/", routeConfig);
    parser.addServer(serverConfig);

    MockSocket mockSocket;
    MockServer server(parser, &mockSocket);

    Router router(serverConfig);
	HTTPRequest request;

	std::string requestStr = "GET / HTTP/1.1\r\nHost: 127.0.0.1:8080\r\nConnection: keep-alive\r\n\r\n";
	request.appendData(requestStr, parser.getServers());

	RouteConfig config = router.routeRequest(request);
	ASSERT_TRUE(config.getRoot() == "./var/www/html", "Root should be ./var/www/html");
	ASSERT_TRUE(*config.getIndex().begin() == "index.html", "Index should be index.html");
	ASSERT_TRUE(*config.getMethods().begin() == "GET", "Methods should be GET");

	return true;
}

bool test_handle_request_serve_static_file() {
    MockConfigParser parser;
    ServerConfig serverConfig;
	RouteConfig routeConfig;

	routeConfig.setRoot("./var/www/html");
	routeConfig.setMethods({"GET"});
	routeConfig.setIndex({"index.html"});
    serverConfig.setHost("127.0.0.1");
    serverConfig.setListen("8080");
    serverConfig.addRoute("/", routeConfig);
    parser.addServer(serverConfig);

    MockSocket mockSocket;
    MockServer server(parser, &mockSocket);

    Router router(serverConfig);
	HTTPRequest request;

	std::string requestStr = "GET / HTTP/1.1\r\nHost: 127.0.0.1:8080\r\nConnection: keep-alive\r\n\r\n";
	request.appendData(requestStr, parser.getServers());

    HTTPResponse response;
    router.handleRequest(request, response);
    ASSERT_TRUE(response.getStatusCode() == 200, "Status code deve ser 200 OK");

    ASSERT_TRUE(response.getHeaders().find("Content-Type") != response.getHeaders().end(), "Header 'Content-Type' deve existir");
    ASSERT_TRUE(response.getHeaders().at("Content-Type") == "text/html", "Content-Type deve ser 'text/html'");

    ASSERT_TRUE(response.getBody().find("Hello World") != std::string::npos, "Corpo da resposta deve conter 'Hello World'");

    return true;
}

// handleRequest - Requisição com método não permitido
bool test_handle_request_method_not_allowed() {
    MockConfigParser parser;
    ServerConfig serverConfig;
	RouteConfig routeConfig;

	routeConfig.setRoot("./var/www/html");
	routeConfig.setMethods({"GET"});
	routeConfig.setIndex({"index.html"});
    serverConfig.setHost("127.0.0.1");
    serverConfig.setListen("8080");
    serverConfig.addRoute("/", routeConfig);
    parser.addServer(serverConfig);

    MockSocket mockSocket;
    MockServer server(parser, &mockSocket);
    Router router(serverConfig);

    HTTPRequest request;
    std::string requestStr = "POST / HTTP/1.1\r\nHost: 127.0.0.1:8080\r\nConnection: keep-alive\r\n\r\n";
    request.appendData(requestStr, parser.getServers());

    // Criação da resposta HTTP
    HTTPResponse response;

    // ====== Execução do handleRequest ======
    router.handleRequest(request, response);

    // ====== Verificações ======
    // Verificar se o status code é 405
    ASSERT_TRUE(response.getStatusCode() == 405, "Status code deve ser 405 Method Not Allowed");

    // Verificar se o header 'Allow' está correto
    ASSERT_TRUE(response.getHeaders().find("Allow") != response.getHeaders().end(), "Header 'Allow' deve existir");
    ASSERT_TRUE(response.getHeaders().at("Allow") == "GET", "Allow deve ser 'GET'");

    // Verificar se o corpo da resposta contém a mensagem de erro
    ASSERT_TRUE(response.getBody().find("405") != std::string::npos, "Corpo da resposta deve conter '405 Method Not Allowed'");

    return true;
}

// handleRequest - Requisição que deve resultar em redirecionamento 301
bool test_handle_request_redirect() {
    // ====== Configuração Inicial ======
    // Configuração do servidor com rota para "/old" que redireciona para "/new"
    MockConfigParser parser;
    ServerConfig config;
	RouteConfig routeConfig;

	routeConfig.setRoot("./var/www/html");
	routeConfig.setMethods({"GET"});
	routeConfig.setIndex({"index.html"});
	routeConfig.setReturnCodes("301 /new");
    config.setHost("127.0.0.1");
    config.setListen("8080");
    config.addRoute("/old", routeConfig);
    parser.addServer(config);

    // Inicialização do MockSocket e MockServer
    MockSocket mockSocket;
    MockServer server(parser, &mockSocket);

    // Inicialização do Router
    Router router(config);

    // Criação da requisição HTTP GET para "/old"
    HTTPRequest request;
    std::string requestStr = "GET /old HTTP/1.1\r\nHost: 127.0.0.1:8080\r\nConnection: keep-alive\r\n\r\n";
    request.appendData(requestStr, parser.getServers());

    // Criação da resposta HTTP
    HTTPResponse response;

    // ====== Execução do handleRequest ======
    router.handleRequest(request, response);

    // ====== Verificações ======
    // Verificar se o status code é 301
    ASSERT_TRUE(response.getStatusCode() == 301, "Status code deve ser 301 Moved Permanently");

    // Verificar se o header 'Location' está correto
    ASSERT_TRUE(response.getHeaders().find("Location") != response.getHeaders().end(), "Header 'Location' deve existir");
    ASSERT_TRUE(response.getHeaders().at("Location") == "/new", "Location deve ser '/new'");

    // Verificar se o corpo da resposta contém a mensagem de redirecionamento
    ASSERT_TRUE(response.getBody().find("301") != std::string::npos, "Corpo da resposta deve conter '301 Moved Permanently'");

    return true;
}

// handleRequest - Requisição para diretório com autoindex ativado
bool test_handle_request_autoindex_enabled() {
    // ====== Configuração Inicial ======
    // Configuração do servidor com rota para "/files" que ativa autoindex
    MockConfigParser parser;
    ServerConfig config;
	RouteConfig routeConfig;

	routeConfig.setRoot("./var/www");
	routeConfig.setMethods({"GET"});
	routeConfig.setAutoindex("on");
    config.setHost("127.0.0.1");
    config.setListen("8080");
    config.addRoute("/files", routeConfig);
    parser.addServer(config);

    // Inicialização do MockSocket e MockServer
    MockSocket mockSocket;
    MockServer server(parser, &mockSocket);

    // Inicialização do Router
    Router router(config);

    // Criação da requisição HTTP GET para "/files"
    HTTPRequest request;
    std::string requestStr = "GET /files HTTP/1.1\r\nHost: 127.0.0.1:8080\r\nConnection: keep-alive\r\n\r\n";
    request.appendData(requestStr, parser.getServers());

    // Criação da resposta HTTP
    HTTPResponse response;

    // ====== Execução do handleRequest ======
    router.handleRequest(request, response);

    // ====== Verificações ======
    // Verificar se o status code é 200
    ASSERT_TRUE(response.getStatusCode() == 200, "Status code deve ser 200 OK");

    // Verificar se o Content-Type está correto
    ASSERT_TRUE(response.getHeaders().find("Content-Type") != response.getHeaders().end(), "Header 'Content-Type' deve existir");
    ASSERT_TRUE(response.getHeaders().at("Content-Type") == "text/html", "Content-Type deve ser 'text/html'");

    // Verificar se o corpo da resposta contém a mensagem de autoindex
    ASSERT_TRUE(response.getBody().find("Index of") != std::string::npos, "Corpo da resposta deve conter mensagem de autoindex");

    return true;
}

// handleRequest - Requisição para diretório sem autoindex e sem arquivo index
bool test_handle_request_autoindex_disabled_no_index() {
    // ====== Configuração Inicial ======
    // Configuração do servidor com rota para "/secure" que desativa autoindex
    MockConfigParser parser;
    ServerConfig config;
	RouteConfig routeConfig;

	routeConfig.setRoot("./var/www");
	routeConfig.setMethods({"GET"});
	routeConfig.setIndex({"index.html"});
	routeConfig.setAutoindex("off");
    config.setHost("127.0.0.1");
    config.setListen("8080");
    config.addRoute("/secure", routeConfig);
    parser.addServer(config);

    // Inicialização do MockSocket e MockServer
    MockSocket mockSocket;
    MockServer server(parser, &mockSocket);

    // Inicialização do Router
    Router router(config);

    // Criação da requisição HTTP GET para "/secure"
    HTTPRequest request;
    std::string requestStr = "GET /secure HTTP/1.1\r\nHost: 127.0.0.1:8080\r\nConnection: keep-alive\r\n\r\n";
    request.appendData(requestStr, parser.getServers());

    // Criação da resposta HTTP
    HTTPResponse response;

    // ====== Execução do handleRequest ======
    router.handleRequest(request, response);

    // ====== Verificações ======
    // Verificar se o status code é 404
    ASSERT_TRUE(response.getStatusCode() == 404, "Status code deve ser 404");

    // Verificar se o corpo da resposta contém a mensagem de erro
    ASSERT_TRUE(response.getBody().find("404") != std::string::npos, "Corpo da resposta deve conter '404 Not Found'");

    // Verificar se o Content-Type está correto
    ASSERT_TRUE(response.getHeaders().find("Content-Type") != response.getHeaders().end(), "Header 'Content-Type' deve existir");
    ASSERT_TRUE(response.getHeaders().at("Content-Type") == "text/html", "Content-Type deve ser 'text/html'");

    return true;
}

bool test_handle_no_root() {
    // ====== Configuração Inicial ======
    // Configuração do servidor com rota para "/files" que ativa autoindex
    MockConfigParser parser;
    ServerConfig config;
    RouteConfig routeConfig;

    routeConfig.setMethods({"GET"});
    routeConfig.setAutoindex("on");
    // config.setHost("

    return true;
}
