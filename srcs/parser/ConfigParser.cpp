/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 14:10:43 by Everton           #+#    #+#             */
/*   Updated: 2024/10/29 10:01:33 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <fstream>
#include <iostream>
#include "ConfigParser.hpp"
#include <cctype>
#include "../aux.hpp"

ConfigParser::ConfigParser() {
    state = STATE_GLOBAL;
    initializeGlobalKeywords();
}

ConfigParser *ConfigParser::loadConfig(const std::string& filename) {
    std::ifstream file(filename.c_str());
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    this->file << file.rdbuf();
    file.close();
    if (this->file.str().empty()) {
        throw std::runtime_error("Empty configuration file.");
    }
    return parse();
}

ConfigParser *ConfigParser::parse(){
    std::string line;
    int openBraces = 0;

	while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key;
        iss >> key;
        if (line.find("{") != std::string::npos && line.find("}") == std::string::npos)
            openBraces++;
        bool handled = false;
        std::map<std::string, KeywordHandler>::const_iterator it = globalKeywords.find(key);
        if (it != globalKeywords.end()) {
            (this->*(it->second.handler))(iss);
            handled = true;
        }
        if (!handled)
            handleDirective(iss, key);
        if (line.find("}") != std::string::npos)
            openBraces--;
    }
    if (openBraces != 0)
        throw std::runtime_error("Mismatched braces in configuration file.");

    return this;
}

void ConfigParser::parseServerDirective(ServerConfig& server, const std::string& key, std::istringstream& iss) {
	std::string value;
    std::getline(iss, value);
	value = trim(value);
    if (removeTrailingSemicolon(value, key)) return;
	server.applyDirective(key, value);
}

void ConfigParser::parseLocationDirective(RouteConfig& route, const std::string& key, std::istringstream& iss) {
	std::string value;
	std::getline(iss, value);
	value = trim(value);
    if (removeTrailingSemicolon(value, key)) return;
	route.applyDirective(key, value);
}

std::vector<ServerConfig> ConfigParser::getServers() const {
    return servers;
}

void ConfigParser::initializeGlobalKeywords() {
	globalKeywords["server"].handler = &ConfigParser::handleServer;
    globalKeywords["location"].handler = &ConfigParser::handleLocation;
    globalKeywords["}"].handler = &ConfigParser::handleClosingBrace;
}

void ConfigParser::handleServer(std::istringstream& iss) {
    (void)iss;
    if (state != STATE_GLOBAL)
        throw std::runtime_error("Unexpected 'server' block.");

    state = STATE_SERVER;
    currentServer = ServerConfig();
}

void ConfigParser::handleLocation(std::istringstream& iss) {
    if (state != STATE_SERVER)
        throw std::runtime_error("Unexpected 'location' block outside of 'server' block.");

    std::string path;
    iss >> path;
    if (path.empty())
        throw std::runtime_error("Location path is missing.");

    route_name = path;
    state = STATE_LOCATION;
    currentRoute = RouteConfig();
    currentRoute.setRoot(currentServer.getRoot());
}

void ConfigParser::handleClosingBrace(std::istringstream& iss) {
    (void)iss;
    if (state == STATE_LOCATION) {
        if (currentRoute.getUploadEnable() && currentRoute.getUploadPath().empty())
            throw std::runtime_error("Location block missing upload_path directive.");
        currentServer.addRoute(route_name, currentRoute);
        state = STATE_SERVER;
    } else if (state == STATE_SERVER) {
        if (currentServer.getPort() < 0)
            throw std::runtime_error("Server block missing listen directive.");
        setServer(currentServer);
        state = STATE_GLOBAL;
    } else {
        throw std::runtime_error("Unexpected closing brace.");
    }
}

void ConfigParser::handleDirective(std::istringstream& iss, const std::string& key) {
    if (state == STATE_GLOBAL)
        return;
    if (state == STATE_SERVER)
        return (parseServerDirective(currentServer, key, iss));
    if (state == STATE_LOCATION) {
        return (parseLocationDirective(currentRoute, key, iss));
    }
    throw std::runtime_error("Directive outside of server block: " + key);
}

void ConfigParser::setServer(ServerConfig server) {
    servers.push_back(server);
}