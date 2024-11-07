#include "../headers/basic.hpp"
#include "../headers/MockServer.hpp"
#include "../headers/MockConfigParser.hpp"
#include "../../srcs/router/Router.hpp"
#include "../../srcs/parser/RouteConfig.hpp"
#include "../../srcs/http/HTTPRequest.hpp"

bool test_error_404() {
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
    std::string requestStr = "POST / HTTP/1.1\r\nHost: 127.0.0.1:8080/error\r\nConnection: keep-alive\r\n\r\n";
    request.appendData(requestStr, parser.getServers());

    HTTPResponse response;

    router.handleRequest(request, response);
    ASSERT_TRUE(response.getStatusCode() == 404, "Status code should be 404");
    ASSERT_TRUE(response.getHeaders().find("Content-Type")->second == "text/html", "Content-Type should be text/html");
    ASSERT_TRUE(response.getBody().find("<title>404 Not Found</title>") != std::string::npos, "Body should contain 404 Not Found");
    return true;
}

bool test_error_400() {
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
    request.appendData("GET / HTTP/1.1\r\nHost: \r\n\r\n", parser.getServers()); // Malformed request

    HTTPResponse response;

    router.handleRequest(request, response);
    ASSERT_TRUE(response.getStatusCode() == 400, "Status code should be 400");
    ASSERT_TRUE(response.getBody().find("<title>400 Bad Request</title>") != std::string::npos, "Body should contain 400 Bad Request");
    return true;
}

bool test_error_403() {
    MockConfigParser parser;
    ServerConfig serverConfig;
	RouteConfig routeConfig;

	routeConfig.setRoot("/restricted");
	routeConfig.setMethods({"GET"});
	routeConfig.setIndex({"index.html"});
    serverConfig.setHost("127.0.0.1");
    serverConfig.setListen("8080");
    serverConfig.addRoute("/forbidden", routeConfig);
    parser.addServer(serverConfig);

    MockSocket mockSocket;
    MockServer server(parser, &mockSocket);
    Router router(serverConfig);

    HTTPRequest request;
    request.appendData("GET /forbidden HTTP/1.1\r\nHost: 127.0.0.1:8080\r\n\r\n", parser.getServers());

    HTTPResponse response;

    router.handleRequest(request, response);
    ASSERT_TRUE(response.getStatusCode() == 403, "Status code should be 403");
    ASSERT_TRUE(response.getBody().find("<title>403 Forbidden</title>") != std::string::npos, "Body should contain 403 Forbidden");
    return true;
}

bool test_error_405() {
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
    request.appendData("DELETE / HTTP/1.1\r\nHost: 127.0.0.1:8080\r\n\r\n", parser.getServers());

    HTTPResponse response;

    router.handleRequest(request, response);
    ASSERT_TRUE(response.getStatusCode() == 405, "Status code should be 405");
    ASSERT_TRUE(response.getBody().find("<title>405 Method Not Allowed</title>") != std::string::npos, "Body should contain 405 Method Not Allowed");
    return true;
}

bool test_error_500() {
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
    request.appendData("GET / HTTP/1.1\r\nHost: 127.0.0.1:8080\r\n\r\n", parser.getServers());

    HTTPResponse response;

    router.handleRequest(request, response);
    ASSERT_TRUE(response.getStatusCode() == 500, "Status code should be 500");
    ASSERT_TRUE(response.getBody().find("<title>500 Internal Server Error</title>") != std::string::npos, "Body should contain 500 Internal Server Error");
    return true;
}