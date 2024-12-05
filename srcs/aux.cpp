/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 10:48:25 by Everton           #+#    #+#             */
/*   Updated: 2024/12/05 13:31:59 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "aux.hpp"
#include <linux/limits.h>
#include <map>
#include <sstream>
#include <cstdlib>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include "./logger/Logger.hpp"

ServerConfig selectConfig(HTTPRequest request, std::vector<ServerConfig> serverConfigs) {
    std::string requestHost = request.getHeaders()["host"];
    std::string requestPort = requestHost.substr(requestHost.find(":") + 1);
    requestHost = requestHost.substr(0, requestHost.find(":"));
    for (size_t i = 0; i < serverConfigs.size(); i++) {
        std::string host = serverConfigs[i].getHost();
        int port = serverConfigs[i].getPort();

        bool findHost = host == requestHost || host == "0.0.0.0";
        bool findPort = port == std::atoi(requestPort.c_str());
        if ((findHost && findPort) || findHost)
            return serverConfigs[i];
        else {
            std::vector<std::string> serverName = serverConfigs[i].getServerName();
            for (size_t j = 0; j < serverName.size(); j++) {
                bool findServerName = serverName[j] == requestHost;
                if ((findServerName && findPort) || findServerName)
                    return serverConfigs[i];
            }
        }
    }
    Logger::getInstance()
        .log(Logger::ERROR, "No server config found for request");
    return ServerConfig();
}

std::string joinMethods(const std::set<std::string>& methods) {
    std::ostringstream oss;
    for (std::set<std::string>::const_iterator it = methods.begin(); it != methods.end(); ++it) {
        if (it != methods.begin()) {
            oss << ", ";
        }
        oss << *it;
    }
    return oss.str();
}

bool removeTrailingSemicolon(std::string &s, const std::string &key) {
    if (s.find("#") != std::string::npos || key.find("#") != std::string::npos)
        return true;
    if (s.empty() || s.find("{") != std::string::npos || s.find("}") != std::string::npos)
        return true;
    if (s[s.size() - 1] == ';') {
        s.erase(s.size() - 1);
        return false;
    }
    throw std::runtime_error("Missing semicolon in directive: " + s);
}

std::string& trim(std::string& s) {
    std::string::iterator it = s.begin();
    while (it != s.end() && std::isspace(static_cast<unsigned char>(*it))) {
        ++it;
    }
    s.erase(s.begin(), it);

    if (!s.empty()) {
        std::string::reverse_iterator rit = s.rbegin();
        while (rit != s.rend() && std::isspace(static_cast<unsigned char>(*rit))) {
            ++rit;
        }
        s.erase(rit.base(), s.end());
    }
    return s;
}

std::vector<std::string> split(const std::string& s, char delimiter) {
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter)) {
		tokens.push_back(token);
	}
	return tokens;
}

bool isNumber(const std::string& s) {
    for (size_t i = 0; i < s.size(); ++i) {
        if (!std::isdigit(s[i])) {
            return false;
        }
    }
    return true;
}

bool isValidIPv4(const std::string& ip) {
	std::vector<std::string> parts = split(ip, '.');
	if (parts.size() != 4) {
		return false;
	}
	for (size_t i = 0; i < parts.size(); ++i) {
		if (!isNumber(parts[i])) {
			return false;
		}
		int num = std::atoi(parts[i].c_str());
		if (num < 0 || num > 255) {
			return false;
		}
	}
	return true;
}

const std::map<int, std::string> httpReasonPhrase() {
    std::map<int, std::string> reasonPrases;
    reasonPrases[100] = "Continue";
    reasonPrases[101] = "Switching Protocols";
    reasonPrases[102] = "Processing";
    reasonPrases[103] = "Early Hints";
    reasonPrases[104] = "Unknown";
    reasonPrases[200] = "OK";
    reasonPrases[201] = "Created";
    reasonPrases[202] = "Accepted";
    reasonPrases[203] = "Non-Authoritative Information";
    reasonPrases[204] = "No Content";
    reasonPrases[300] = "Multiple Choices";
    reasonPrases[301] = "Moved Permanently";
    reasonPrases[302] = "Found";
    reasonPrases[303] = "See Other";
    reasonPrases[304] = "Not Modified";
    reasonPrases[400] = "Bad Request";
    reasonPrases[401] = "Unauthorized";
    reasonPrases[403] = "Forbidden";
    reasonPrases[404] = "Not Found";
    reasonPrases[405] = "Method Not Allowed";
    reasonPrases[500] = "Internal Server Error";
    reasonPrases[501] = "Not Implemented";
    reasonPrases[502] = "Bad Gateway";
    reasonPrases[503] = "Service Unavailable";
    reasonPrases[504] = "Gateway Timeout";
    reasonPrases[505] = "HTTP Version Not Supported";

    return reasonPrases;
}

std::string getContentType(const std::string& extension) {
    if (extension == ".html") return "text/html";
    if (extension == ".css") return "text/css";
    if (extension == ".js") return "application/javascript";
    if (extension == ".json") return "application/json";
    if (extension == ".png") return "image/png";
    if (extension == ".jpg" || extension == ".jpeg") return "image/jpeg";
    if (extension == ".gif") return "image/gif";
    return "application/octet-stream";
}

std::string itostr(const int value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

std::string getProgram(const std::string& cgiExtension) {
    if (cgiExtension.find(".py") != std::string::npos) {
        //TODO: change to the correct path for the 42 VM
        return "/home/etovaz/nave/webserver/venv/bin/python3";
    }
    if (cgiExtension.find(".php") != std::string::npos) {
        return "/usr/bin/php";
    }
    return "";
}