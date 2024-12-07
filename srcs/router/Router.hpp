/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 18:01:41 by Everton           #+#    #+#             */
/*   Updated: 2024/12/07 15:54:46 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../utils/FilePath.hpp"
#include "../http/HTTPRequest.hpp"
#include "../http/HTTPResponse.hpp"
#include "../parser/RouteConfig.hpp"
#include "../parser/ServerConfig.hpp"
#include "../handlers/ErrorHandler.hpp"
#include "../handlers/methods/PostHandler.hpp"

class Router {
private:
    ServerConfig serverConfig;
    PostHandler postHandler;

public:
    Router();
    Router(const ServerConfig& config);
    Router(const Router& other);
    Router& operator=(const Router& other);
    ~Router();

    bool isCgiRequest(const std::string& path, const std::set<std::string>& cgiExtensions);
    const RouteConfig routeRequest(const HTTPRequest& request);
    void handleRequest(const HTTPRequest& request, HTTPResponse& response);
    void resolvePath(std::string path, const RouteConfig routeConfig, HTTPResponse& response);
    ServerConfig getServerConfig() const;
    bool getAutoIndex(std::string requestURI);
    int handleMethods(const HTTPRequest& request, FilePath filePath);
    int handleDelete(const FilePath& filePath);
};
