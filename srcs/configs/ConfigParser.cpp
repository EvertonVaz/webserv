/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 14:10:43 by Everton           #+#    #+#             */
/*   Updated: 2024/10/04 20:33:49 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include <fstream>
#include <iostream>
#include "ConfigParser.hpp"
#include <cctype>

ConfigParser::ConfigParser() {}

ConfigParser *ConfigParser::loadConfig(const std::string& filename) {
    std::ifstream file(filename.c_str());
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    this->file << file.rdbuf();
    file.close();
    return parse();
}

bool newServerBlock(bool inServerBlock, ServerConfig& currentServer) {
	if (inServerBlock) {
		throw std::runtime_error("Nested server blocks are not allowed.");
		return false;
	}
    currentServer = ServerConfig();
	return true;
}

bool newLocationBlock(bool inLocationBlock, RouteConfig& currentRoute) {
	if (inLocationBlock) {
		throw std::runtime_error("Nested location blocks are not allowed.");
		return false;
	}
    currentRoute = RouteConfig();
	return true;
}

ConfigParser *ConfigParser::parse() {
    std::string line;
    int openBraces = 0;
    std::string route_name;
    bool inServerBlock = false;
    bool inLocationBlock = false;

	while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key;
        iss >> key;
		if (line.find("{") != std::string::npos && line.find("}") == std::string::npos)
			openBraces++;
        if (key == "server") {
            inServerBlock = newServerBlock(inServerBlock, currentServer);
            continue;
        } else if (key == "location") {
			iss >> key;
            route_name = key;
            inLocationBlock = newLocationBlock(inLocationBlock, currentRoute);
            currentRoute.setRoot(currentServer.getRoot());
            continue;
        } else if (key == "}") {
            openBraces--;
            if (inLocationBlock) {
                currentServer.addRoute(route_name, currentRoute);
                inLocationBlock = false;
                continue;
            } else if (inServerBlock) {
                servers.push_back(currentServer);
                inServerBlock = false;
                continue;
            }
        } else {
            if (inServerBlock && !inLocationBlock) {
                parseServerDirective(currentServer, key, iss);
            } else if (inServerBlock && inLocationBlock) {
                parseLocationDirective(currentRoute, key, iss);
            }
        }
    }

    if (openBraces != 0)
        throw std::runtime_error("Mismatched braces in configuration file.");

    return this;
}

static inline bool removeTrailingSemicolon(std::string &s, const std::string &key) {
    if (s.find("#") != std::string::npos || key.find("#") != std::string::npos) {
        return true;
    }
    if (s.empty() || s.find("{") != std::string::npos || s.find("}") != std::string::npos) {
        return true;
    }
    if (s[s.size() - 1] == ';') {
        s.erase(s.size() - 1);
        return false;
    }
    throw std::runtime_error("Missing semicolon in directive: " + s);
}

static inline std::string& trim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return (s);
}

void ConfigParser::parseServerDirective(ServerConfig& server, const std::string& key, std::istringstream& iss) {
	std::string value;
    std::getline(iss, value);
	value = trim(value);
    if (removeTrailingSemicolon(value, key)) return;
	if (key == "listen") {
		server.setListen(value);
	} else if (key == "server_name") {
		server.setServerName(value);
	} else if (key == "error_page") {
		server.setErrorPage(value);
	} else if (key == "root") {
        server.setRoot(value);
    } else if (key == "client_max_body_size") {
        server.setMaxBodySize(std::atoi(value.c_str()));
    } else {
        throw std::runtime_error("Unknown directive: " + key);
    }
}

void ConfigParser::parseLocationDirective(RouteConfig& route, const std::string& key, std::istringstream& iss) {
	std::string value;
	std::getline(iss, value);
	value = trim(value);
    if (removeTrailingSemicolon(value, key)) return;
	if (key == "methods") {
        route.addMethod(value);
	} else if (key == "index") {
		route.setIndex(value);
	} else if (key == "root") {
        route.setRoot(value);
    } else {
        throw std::runtime_error("Unknown directive: " + key);
    }
}

std::vector<ServerConfig> ConfigParser::getServers() const {
    return servers;
}