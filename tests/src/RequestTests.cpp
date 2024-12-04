/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestTests.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:36:21 by Everton           #+#    #+#             */
/*   Updated: 2024/12/03 15:09:26 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/basic.hpp"
#include "../headers/main.hpp"
#include "../../srcs/http/HTTPRequest.hpp"
#include "../../srcs/parser/ServerConfig.hpp"
#include "../headers/MockConfigParser.hpp"
#include <string>
#include <vector>

bool testHTTPRequestParsingWithBody() {
    HTTPRequest request;
    std::string rawRequest =
        "POST /submit HTTP/1.1\r\n"
        "Host: 127.0.0.1\r\n"
        "Content-Length: 11\r\n"
        "\r\n"
        "Hello World";

    std::vector<ServerConfig> serverConfigs; // Mock ou configure conforme necessário
    request.appendData(rawRequest, serverConfigs);

    ASSERT_TRUE(request.isComplete(), "Request deve estar completa.");
    ASSERT_TRUE(request.getMethod() == "POST", "Método deve ser POST.");
    ASSERT_TRUE(request.getURI() == "/submit", "URI deve ser /submit.");
    ASSERT_TRUE(request.getBody() == "Hello World", "Corpo deve ser 'Hello World'.");
    return true;
}

bool testHTTPRequestParsingChunkedEncoding() {
    HTTPRequest request;
    std::string rawRequest =
        "POST /upload HTTP/1.1\r\n"
        "Host: 127.0.0.1\r\n"
        "Transfer-Encoding: chunked\r\n"
        "\r\n"
        "7\r\n"
        "Mozilla\r\n"
        "9\r\n"
        "Developer\r\n"
        "7\r\n"
        "Network\r\n"
        "0\r\n"
        "\r\n";

    std::vector<ServerConfig> serverConfigs; // Mock ou configure conforme necessário
    request.appendData(rawRequest, serverConfigs);

    ASSERT_TRUE(request.isComplete(), "Request deve estar completa.");
    ASSERT_TRUE(request.getBody() == "MozillaDeveloperNetwork", "Corpo deve ser 'MozillaDeveloperNetwork'.");
    return true;
}

bool testHTTPRequestParsingLargeRequest() {
    MockConfigParser parser;
    HTTPRequest request;
    std::string body(1024 * 1024, 'a'); // 1 MB de 'a's
    std::string rawRequest =
        "POST /large HTTP/1.1\r\n"
        "Host: 127.0.0.1\r\n"
        "Content-Length: " + std::to_string(body.size()) + "\r\n"
        "\r\n" +
        body;

    MockConfigParser *p = parser.createDefaultParser();
    std::vector<ServerConfig> serverConfigs = p->getServers();
    serverConfigs[0].setMaxBodySize("104857600");
    request.appendData(rawRequest, serverConfigs);

    ASSERT_TRUE(request.isComplete(), "Request deve estar completa.");
    ASSERT_TRUE(request.getBody().size() == body.size(), "Tamanho do corpo deve corresponder.");
    return true;
}

bool testHTTPRequestParsingInvalidVersion() {
    HTTPRequest request;
    std::string rawRequest =
        "GET / HTTP/1.0\r\n"
        "Host: 127.0.0.1\r\n"
        "\r\n";

    std::vector<ServerConfig> serverConfigs;
    request.appendData(rawRequest, serverConfigs);

    ASSERT_TRUE(request.hasError(), "Request deve apresentar erro.");
    ASSERT_TRUE(request.getState() == HTTPRequest::ERROR505, "Estado deve ser ERROR505.");
    return true;
}

bool testHTTPRequestParsingUnsupportedMethod() {
    HTTPRequest request;
    std::string rawRequest =
        "PUT /resource HTTP/1.1\r\n"
        "Host: 127.0.0.1\r\n"
        "\r\n";

    std::vector<ServerConfig> serverConfigs;
    request.appendData(rawRequest, serverConfigs);

    ASSERT_TRUE(request.hasError(), "Request deve apresentar erro.");
    ASSERT_TRUE(request.getState() == HTTPRequest::ERROR501, "Estado deve ser ERROR501.");
    return true;
}
