/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 18:01:22 by Everton           #+#    #+#             */
/*   Updated: 2024/11/08 16:57:11 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Router.hpp"
#include <sys/stat.h>
#include "../aux.hpp"
#include "../handlers/StaticFileHandler.hpp"
#include "../handlers/ErrorHandler.hpp"
#include "../utils/FilePath.hpp"
// #include "../handlers/CGIHandler.hpp"

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

static inline std::string setRoot(std::string routeRoot, std::string serverRoot) {
    if (routeRoot.empty()) {
        return serverRoot;
    }
    return routeRoot;
}

bool Router::isCGIRequest(std::string uri) const {
    // std::string uri = request.getURI();
    return uri.find(".php") != std::string::npos;
}

void Router::handleRequest(const HTTPRequest& request, HTTPResponse& response) {
    const RouteConfig routeConfig = routeRequest(request);
    ErrorHandler errorHandler(serverConfig.getErrorPage());
    std::string root = setRoot(routeConfig.getRoot(), serverConfig.getRoot());

    if (root.empty()) {
        return errorHandler.handleError(404, response);
    }

    FilePath filePath(root, request.getURI(), routeConfig.getIndex(), routeConfig.getAutoindex());

    std::set<std::string> allowedMethods = routeConfig.getMethods();
    if (allowedMethods.find(request.getMethod()) == allowedMethods.end()) {
        response.addHeader("Allow", joinMethods(allowedMethods));
        return errorHandler.handleError(405, response);
    }

    std::map<int, std::string> redirectPath = routeConfig.getReturnCodes();
    if (!redirectPath.empty()) {
        response.addHeader("Location", redirectPath[301]);
        return errorHandler.handleError(301, response);
    }

    if (routeConfig.getCgiExtensions().size() > 0){
        // TODO: Implement CGIHandler
        // CGIHandler cgiHandler(request, routeConfig);
    } else {
        StaticFileHandler staticHandler(serverConfig.getErrorPage(), filePath);
        staticHandler.setDirectoryListingEnabled(routeConfig.getAutoindex());
        return staticHandler.handleResponse(response);
    }
}
