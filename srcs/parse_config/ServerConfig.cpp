/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 20:05:01 by Everton           #+#    #+#             */
/*   Updated: 2024/10/23 10:54:08 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include "ServerConfig.hpp"
#include "../aux.hpp"


ServerConfig::ServerConfig() {
	host = "127.0.0.1";
	root = "./";
	port = -1;

	initializeDirectiveMap();
};

int ServerConfig::getPort() const {
	return port;
};

std::string ServerConfig::getHost() const {
	return host;
};

std::vector<std::string> ServerConfig::getServerName() const {
	return server_names;
};

std::pair<int, std::string> ServerConfig::getErrorPage() const {
	return error_page;
};

std::string ServerConfig::getRoot() const {
	return root;
};

std::map<std::string, RouteConfig> ServerConfig::getRoutes() const {
	return routes;
};

void ServerConfig::setListen(const std::string& value) {
	std::string port_str = value.substr(value.find(":") + 1);
	if (!isNumber(port_str)) {
		throw std::runtime_error("Invalid port: " + port_str);
	}
	if (value.find(":") != std::string::npos) {
		port = std::atoi(port_str.c_str());
		host = value.substr(0, value.find(":"));
	} else {
		port = std::atoi(value.c_str());
	}
	if (port < 0 || port > 65535) {
		throw std::runtime_error("Invalid port: " + value);
	}
};

void ServerConfig::setHost(const std::string& host) {
	if (!isValidIPv4(host)) {
		throw std::runtime_error("Invalid host: " + host);
	}
	this->host = host;
};

void ServerConfig::setServerName(const std::string& server_name) {
	std::istringstream iss(server_name);
	while (iss) {
		std::string links;
		iss >> links;
		if (!links.empty())
			server_names.push_back(links);
	}
};

void ServerConfig::setErrorPage(const std::string& error_page) {
	std::istringstream iss(error_page);
	iss >> this->error_page.first;
	iss >> this->error_page.second;
};

void ServerConfig::setRoot(const std::string& root) {
	this->root = root;
};

void ServerConfig::setMaxBodySize(const std::string& max_body_size) {
	this->max_body_size = std::atoi(max_body_size.c_str());
};

void ServerConfig::addRoute(const std::string& path, const RouteConfig& route) {
	routes[path] = route;
};

void ServerConfig::initializeDirectiveMap() {
	directiveMap["host"].handler = &ServerConfig::setHost;
	directiveMap["listen"].handler = &ServerConfig::setListen;
	directiveMap["server_name"].handler = &ServerConfig::setServerName;
	directiveMap["error_page"].handler = &ServerConfig::setErrorPage;
	directiveMap["root"].handler = &ServerConfig::setRoot;
	directiveMap["client_max_body_size"].handler = &ServerConfig::setMaxBodySize;
}

void ServerConfig::applyDirective(const std::string& key, const std::string& value) {
    std::map<std::string, DirectiveHandler>::const_iterator it = directiveMap.find(key);
    if (it != directiveMap.end()) {
        (this->*(it->second.handler))(value);
    } else {
        throw std::runtime_error("Unknown directive: " + key);
    }
}