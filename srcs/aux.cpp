/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 10:48:25 by Everton           #+#    #+#             */
/*   Updated: 2024/11/06 20:27:45 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "aux.hpp"
#include <sstream>
#include <cstdlib>
#include <sstream>
#include <stdexcept>

ServerConfig selectConfig(HTTPRequest request, std::vector<ServerConfig> serverConfigs) {
    for (size_t i = 0; i < serverConfigs.size(); i++) {
        if (request.getHeaders()["host"].find(serverConfigs[i].getHost()) != std::string::npos)
            return serverConfigs[i];
    }
    throw std::runtime_error("No server config found for request");
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

std::string getContentType(const std::string& extension) {
    if (extension == ".html") return "text/html";
    if (extension == ".css") return "text/css";
    if (extension == ".png") return "image/png";
    if (extension == ".js") return "application/javascript";
    if (extension == ".json") return "application/json";
    if (extension == ".jpg" || extension == ".jpeg") return "image/jpeg";
    return "application/octet-stream";
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
