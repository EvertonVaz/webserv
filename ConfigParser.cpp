/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 14:10:43 by Everton           #+#    #+#             */
/*   Updated: 2024/10/03 12:04:09 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include "ConfigParser.hpp"

ConfigParser::ConfigParser(const std::string& filename) : filename(filename) {}

std::string ConfigParser::getFilename() const {
	return filename;
}

bool newServerBlock(bool inServerBlock, ServerConfig& currentServer) {
	if (inServerBlock) {
		std::cerr << "Nested server blocks are not allowed." << std::endl;
		return false;
	}
	currentServer = ServerConfig();
	return true;
}

bool newLocationBlock(bool inLocationBlock, RouteConfig& currentRoute) {
	if (inLocationBlock) {
		std::cerr << "Nested location blocks are not allowed." << std::endl;
		return false;
	}
	currentRoute = RouteConfig();
	return true;
}

bool ConfigParser::parse(std::vector<ServerConfig>& servers){
    std::ifstream file(filename.c_str());
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return false;
    }

    std::string line;
    ServerConfig currentServer;
    RouteConfig currentRoute;
    bool inServerBlock = false;
    bool inLocationBlock = false;
    int openBraces = 0;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key;
        iss >> key;
		if (line.find("{") != std::string::npos && line.find("}") == std::string::npos)
			openBraces++;
        if (key == "server") {
            inServerBlock = newServerBlock(inServerBlock, currentServer);
        } else if (key == "location") {
            inLocationBlock = newLocationBlock(inLocationBlock, currentRoute);
        } else if (key == "}") {
            openBraces--;
            if (inLocationBlock) {
                currentServer.routes.push_back(currentRoute);
                inLocationBlock = false;
            } else if (inServerBlock) {
                servers.push_back(currentServer);
                inServerBlock = false;
            }
        } else {
            if (inServerBlock && !inLocationBlock) {
                parseServerDirective(currentServer, key, iss);
            } else if (inServerBlock && inLocationBlock) {
                parseLocationDirective(currentRoute, key, iss);
            }
        }
    }

    if (openBraces != 0) {
        std::cerr << "Mismatched braces in configuration file." << std::endl;
        return false;
    }

    file.close();
    return true;
}

void ConfigParser::parseServerDirective(ServerConfig& server, const std::string& key, std::istringstream& iss) {
	if (key == "listen") {
		iss >> server.listen;
	} else if (key == "server_name") {
		iss >> server.server_name;
	} else if (key == "root") {
		iss >> server.root;
	} else if (key == "error_page") {
		iss >> server.error_page;
	}
}

void ConfigParser::parseLocationDirective(RouteConfig& route, const std::string& key, std::istringstream& iss) {
	if (key == "methods") {
		iss >> route.methods;
	} else if (key == "upload_directory") {
		iss >> route.upload_directory;
	} else if (key == "index") {
		iss >> route.index;
	}
}