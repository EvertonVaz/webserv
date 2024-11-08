/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operators.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 14:40:07 by Everton           #+#    #+#             */
/*   Updated: 2024/11/08 14:43:06 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"
#include "RouteConfig.hpp"

bool ServerConfig::operator!=(const ServerConfig &other) const {
    return !(*this == other);
}

bool ServerConfig::operator==(const ServerConfig &other) const {
    return (this->port == other.port &&
            this->host == other.host &&
            this->root == other.root &&
            this->maxBodySize == other.maxBodySize &&
            this->serverNames == other.serverNames &&
            this->errorPage == other.errorPage &&
            this->routes == other.routes);
}

bool ServerConfig::operator<(const ServerConfig &other) const {
    return this->port < other.port;
}

bool ServerConfig::operator>(const ServerConfig &other) const {
    return this->port > other.port;
}

bool RouteConfig::operator!=(const RouteConfig &other) const {
    return !(*this == other);
}

bool RouteConfig::operator==(const RouteConfig &other) const {
    return (this->getAutoindex() == other.getAutoindex() &&
            this->getRoot() == other.getRoot() &&
            this->getUploadEnable() == other.getUploadEnable() &&
            this->getUploadPath() == other.getUploadPath() &&
            this->getIndex() == other.getIndex() &&
            this->getMethods() == other.getMethods() &&
            this->getCgiExtensions() == other.getCgiExtensions() &&
            this->getAllowedMethods() == other.getAllowedMethods() &&
            this->getReturnCodes() == other.getReturnCodes());
}

bool RouteConfig::operator<(const RouteConfig &other) const {
    return this->root < other.root;
}

bool RouteConfig::operator>(const RouteConfig &other) const {
    return this->root > other.root;
}