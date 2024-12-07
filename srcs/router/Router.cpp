/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 18:01:22 by Everton           #+#    #+#             */
/*   Updated: 2024/12/07 16:29:50 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Router.hpp"
#include <sys/stat.h>
#include "../aux.hpp"
#include "../handlers/CGIHandler.hpp"
#include "../handlers/StaticFileHandler.hpp"

Router::Router() {}

Router::Router(const ServerConfig& config) : serverConfig(config) {
    PostHandler postHandler;
}

Router::Router(const Router& other) {
    serverConfig = other.serverConfig;
}

Router& Router::operator=(const Router& other) {
    serverConfig = other.serverConfig;
    return *this;
}

Router::~Router() {}

const RouteConfig Router::routeRequest(const HTTPRequest& request) {
    std::string requestPath = request.getURI();
    std::map<std::string, RouteConfig> routes = serverConfig.getRoutes();

    RouteConfig bestMatch;
    size_t bestMatchLength = 0;
    std::map<std::string, RouteConfig>::iterator it;
    for (it = routes.begin(); it != routes.end(); ++it) {
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

bool Router::getAutoIndex(std::string requestURI) {
    std::map<std::string, RouteConfig> routes = serverConfig.getRoutes();
    std::map<std::string, RouteConfig>::iterator it;
    for (it = routes.begin(); it != routes.end(); ++it) {
        if (requestURI.find(it->first) != std::string::npos && routes[it->first].getAutoindex()) {
            return it->second.getAutoindex();
        }
    }
    return false;
}

int Router::handleDelete(const FilePath& filePath) {
    if (filePath.getPathExist())  {
        if (filePath.getIsDirectory()) {
            return 403;
        } else if (filePath.getCanWrite()) {
            if (remove(filePath.getPath().c_str()) == 0) {
                return 204;
            } else {
                return 500;
            }
        }
    }
    return 404;
}

int Router::handleMethods(const HTTPRequest& request, FilePath filePath) {
    int code = 0;
    if (request.getMethod() == "POST") {
        postHandler.setRequest(request);
        code = postHandler.handlePost();
    } else if (request.getMethod() == "DELETE") {
        code = handleDelete(filePath);
    }
    return code;
}

void Router::handleRequest(const HTTPRequest& request, HTTPResponse& response) {
    const RouteConfig routeConfig = routeRequest(request);
    ErrorHandler errorHandler(serverConfig.getErrorPage());
    std::string root = setRoot(routeConfig.getRoot(), serverConfig.getRoot());

    if (root.empty()) {
        return errorHandler.handleError(404, response);
    }
    bool autoIndex = getAutoIndex(request.getURI());
    FilePath filePath(root, request, routeConfig);

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

    if (isCgiRequest(filePath.getPath(), routeConfig.getCgiExtensions())) {
        CGIHandler cgiHandler(errorHandler, filePath, request);
        return cgiHandler.handleResponse(response);
    } else {
        int code = handleMethods(request, filePath);
        if (code >= 400)
            return errorHandler.handleError(code, response);
        StaticFileHandler staticHandler(errorHandler, filePath);
        staticHandler.setDirectoryListingEnabled(autoIndex);
        staticHandler.setStatusCodeAndMethod(code, request.getMethod());
        return staticHandler.handleResponse(response);
    }
}

bool Router::isCgiRequest(const std::string& path, const std::set<std::string>& cgiExtensions) {
    size_t dotPos = path.find_last_of('.');
    if (dotPos == std::string::npos)
        return false;
    std::string extension = path.substr(dotPos);
    return cgiExtensions.find(extension) != cgiExtensions.end();
}

ServerConfig Router::getServerConfig() const {
    return serverConfig;
}