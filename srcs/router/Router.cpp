/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 18:01:22 by Everton           #+#    #+#             */
/*   Updated: 2024/11/05 15:43:25 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Router.cpp
#include "Router.hpp"
#include <sys/stat.h>
#include "../aux.hpp"

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

    if (routeConfig.getRoot().empty()) {
        response.setStatusCode(404);
        response.setBody("<html><body><h1>404 Not Found</h1></body></html>");
        response.addHeader("Content-Type", "text/html");
        return;
    }

    // TODO: preciso arrumar o allowedMethods do RouteConfig
    // erro: se nao tiver nenhum método permitido
    // ele nao aceita nenhuma requisição, obrigando a ter um método permitido
    std::set<std::string> allowedMethods = routeConfig.getMethods();
    if (allowedMethods.find(request.getMethod()) == allowedMethods.end()) {
        response.setStatusCode(405);
        response.addHeader("Allow", joinMethods(allowedMethods));
        response.setBody("<html><body><h1>405 Method Not Allowed</h1></body></html>");
        response.addHeader("Content-Type", "text/html");
        return;
    }

    std::map<int, std::string> redirectPath = routeConfig.getReturnCodes();
    if (!redirectPath.empty()) {
        response.setStatusCode(301);
        response.addHeader("Location", redirectPath[301]);
        response.setBody("<html><body><h1>301 Moved Permanently</h1></body></html>");
        response.addHeader("Content-Type", "text/html");
        return;
    }

    std::string root = routeConfig.getRoot();
    if (root.empty()) {
        root = serverConfig.getRoot();
    }
    std::string filePath = root + request.getURI();
    resolvePath(filePath, routeConfig, response);
}

void Router::resolvePath(std::string path, const RouteConfig routeConfig, HTTPResponse& response) {
    struct stat pathStat;

    std::set<std::string> indexFiles = routeConfig.getIndex();
    int st = stat(path.c_str(), &pathStat);
    if (st == 0 && S_ISDIR(pathStat.st_mode)) {
        if (indexFiles.size() > 0) {
            for (std::set<std::string>::iterator it = indexFiles.begin(); it != indexFiles.end(); ++it) {
                if (path[path.length() - 1] != '/')
                    path += "/";
                std::string indexPath = path + *it;
                if (stat(indexPath.c_str(), &pathStat) == 0 && S_ISREG(pathStat.st_mode)) {
                    return serveStaticFile(indexPath, response);
                }
            }
        }

        if (routeConfig.getAutoindex()) {
            // generateDirectoryListing(path, response); função para listar o diretório
            response.setStatusCode(200);
            response.setBody("<html><body><h1>200 OK - era para listar arquivos</h1></body></html>");
            response.addHeader("Content-Type", "text/html");
            return;
        } else {
            response.setStatusCode(403);
            response.setBody("<html><body><h1>403 Forbidden</h1></body></html>");
            response.addHeader("Content-Type", "text/html");
            return;
        }
    }
}
