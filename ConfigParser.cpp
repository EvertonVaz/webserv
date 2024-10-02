/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 14:10:43 by Everton           #+#    #+#             */
/*   Updated: 2024/10/02 15:05:32 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include "ConfigParser.hpp"

ConfigParser::ConfigParser(const std::string& filename) : filename(filename) {}

std::string ConfigParser::getFilename() const {
	return filename;
}

bool ConfigParser::parse(std::vector<ServerConfig>& servers) {
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

	while (std::getline(file, line)) {
		std::istringstream iss(line);
		std::string key;
		std::string path;
		iss >> key;

		if (key == "server") {
			currentServer = ServerConfig();
			inServerBlock = true;
		} else if (key == "location") {
			currentRoute = RouteConfig();
			inLocationBlock = true;
			iss >> path;
			currentRoute.path = path;
		} else if (key == "}") {
			if (inLocationBlock) {
				currentServer.routes.push_back(currentRoute);
				inLocationBlock = false;
			} else if (inServerBlock) {
				servers.push_back(currentServer);
				inServerBlock = false;
			}
		} if (inServerBlock) {
			parseServerDirective(currentServer, key, iss);
		} if (inLocationBlock) {
			parseLocationDirective(currentRoute, key, iss);
		}
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