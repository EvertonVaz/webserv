/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 20:05:01 by Everton           #+#    #+#             */
/*   Updated: 2024/10/04 20:16:19 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include "ServerConfig.hpp"

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

void ServerConfig::setListen(std::string value) {
	if (value.find(":") != std::string::npos) {
		std::string port_str = value.substr(value.find(":") + 1);
		port = std::atoi(port_str.c_str());
		host = value.substr(0, value.find(":"));
	} else {
		port = std::atoi(value.c_str());
	}
};

void ServerConfig::setHost(const std::string& host) {
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

void ServerConfig::setErrorPage(std::string error_page) {
	std::istringstream iss(error_page);
	iss >> this->error_page.first;
	iss >> this->error_page.second;
};

void ServerConfig::setRoot(const std::string& root) {
	this->root = root;
};

void ServerConfig::setMaxBodySize(int max_body_size) {
	this->max_body_size = max_body_size;
};

void ServerConfig::addRoute(const std::string& path, const RouteConfig& route) {
	routes[path] = route;
};