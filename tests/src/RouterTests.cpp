/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RouterTests.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 16:10:10 by Everton           #+#    #+#             */
/*   Updated: 2024/11/05 16:48:40 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/basic.hpp"
#include "../headers/MockServer.hpp"
#include "../../srcs/router/Router.hpp"
#include "../../srcs/parser/ConfigParser.hpp"
#include "../../srcs/http/HTTPRequest.hpp"

std::string requestStr = "GET / HTTP/1.1\r\nHost: 127.0.0.1:8080\r\nConnection: keep-alive\r\n\r\n";
// const RouteConfig routeRequest(const HTTPRequest& request);
// void handleRequest(const HTTPRequest& request, HTTPResponse& response);
// void resolvePath(std::string path, const RouteConfig routeConfig, HTTPResponse& response);

bool test_route_request() {
	ConfigParser parser;
	parser.loadConfig(path);

	MockSocket mockSocket = MockSocket();
	MockServer server(parser, &mockSocket);
	HTTPRequest request;
	Router router(parser.getServers()[0]);
	request.appendData(requestStr, parser.getServers());

	RouteConfig config = router.routeRequest(request);

	return true;
}
