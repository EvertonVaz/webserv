/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 14:10:43 by Everton           #+#    #+#             */
/*   Updated: 2024/10/03 22:57:09 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>
#include <cstdlib>
#include <iostream>
#include "ConfigParser.hpp"

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

bool newServerBlock(bool inServerBlock) {
	if (inServerBlock) {
		throw std::runtime_error("Nested server blocks are not allowed.");
		return false;
	}

	return true;
}

bool newLocationBlock(bool inLocationBlock) {
	if (inLocationBlock) {
		throw std::runtime_error("Nested location blocks are not allowed.");
		return false;
	}
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
            inServerBlock = newServerBlock(inServerBlock);
            continue;
        } else if (key == "location") {
			iss >> key;
            route_name = key;
            inLocationBlock = newLocationBlock(inLocationBlock);
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

static inline void removeTrailingSemicolon(std::string &s) {
    if (!s.empty() && s[s.size() - 1] == ';') {
        s.erase(s.size() - 1);
    }
}

void ConfigParser::parseServerDirective(ServerConfig& server, const std::string& key, std::istringstream& iss) {
	std::string value;
	iss >> value;
    removeTrailingSemicolon(value);
	if (key == "listen") {
		server.setPort(std::atoi(value.c_str()));
	} else if (key == "server_name") {
		server.setServerName(value);
	} else if (key == "error_page") {
		server.setErrorPage(value);
	} else if (key == "root") {
        server.setRoot(value);
    }
}

void ConfigParser::parseLocationDirective(RouteConfig& route, const std::string& key, std::istringstream& iss) {
	std::string value;
	iss >> value;
    removeTrailingSemicolon(value);
	if (key == "methods") {
        while (true) {
            removeTrailingSemicolon(value);
            route.addMethod(value);
            if (!(iss >> value))
                break;
        }
	} else if (key == "index") {
		route.setIndex(value);
	} else if (key == "root") {
        route.setRoot(value);
    }
}

std::vector<ServerConfig> ConfigParser::getServers() const {
    return servers;
}