/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 18:01:22 by Everton           #+#    #+#             */
/*   Updated: 2024/11/06 23:06:32 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Router.hpp"
#include <sys/stat.h>
#include "../aux.hpp"
#include "../handlers/StaticFileHandler.hpp"
#include "../handlers/ErrorHandler.hpp"

Router::Router() {}

Router::Router(const ServerConfig& config) {
    serverConfig = config;
}

Router::Router(const Router& other) {
    serverConfig = other.serverConfig;
}

Router& Router::operator=(const Router& other) {
    serverConfig = other.serverConfig;
    return *this;
}

Router::~Router() {
}

const RouteConfig Router::routeRequest(const HTTPRequest& request) {
    std::string requestPath = request.getURI();
    std::map<std::string, RouteConfig> routes = serverConfig.getRoutes();

    RouteConfig bestMatch;
    size_t bestMatchLength = 0;

    for (std::map<std::string, RouteConfig>::iterator it = routes.begin(); it != routes.end(); ++it) {
        std::string routePath = it->first;
        if (routePath == "/") {
            if (requestPath == "/") {
                return it->second;
            }
        } else if (requestPath.find(routePath) == 0) {
            size_t matchLength = routePath.length();
            if (matchLength > bestMatchLength) {
                bestMatch = (it->second);
                bestMatchLength = matchLength;
            }
        }
    }
    return bestMatch;
}

void Router::handleRequest(const HTTPRequest& request, HTTPResponse& response) {
    const RouteConfig routeConfig = routeRequest(request);
    ErrorHandler errorHandler;

    if (routeConfig.getRoot().empty()) {
        return errorHandler.handleError(404, response);
    }

    std::set<std::string> allowedMethods = routeConfig.getMethods();
    if (allowedMethods.find(request.getMethod()) == allowedMethods.end()) {
        response.addHeader("Allow", joinMethods(allowedMethods));
        return errorHandler.handleError(405, response);;
    }

    std::map<int, std::string> redirectPath = routeConfig.getReturnCodes();
    if (!redirectPath.empty()) {
        response.setStatusCode(301);
        response.addHeader("Location", redirectPath[301]);
        response.addHeader("Content-Type", "text/html");
        response.setBody("<html><body><h1>301 Moved Permanently</h1></body></html>");
        return;
    }

    std::string root = routeConfig.getRoot();
    if (root.empty()) {
        root = serverConfig.getRoot();
    }

    StaticFileHandler staticHandler;
    staticHandler.setUri(request.getURI());
    staticHandler.setRootDirectory(root);
    staticHandler.setDirectoryListingEnabled(routeConfig.getAutoindex());
    staticHandler.setIndexFiles(routeConfig.getIndex());
    staticHandler.handleRequest(response);
}
