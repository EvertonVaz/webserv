/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RouteConfig.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 19:58:50 by Everton           #+#    #+#             */
/*   Updated: 2024/10/10 21:57:35 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RouteConfig.hpp"
#include <sstream>

RouteConfig::RouteConfig() {
	initializeDirectiveMap();
}

std::string RouteConfig::getRoot() const {
	return root;
}

std::set<std::string> RouteConfig::getMethods() const {
	return methods;
}

void RouteConfig::addMethod(const std::string& method) {
	std::istringstream iss(method);
	while (iss) {
		std::string word;
		iss >> word;
		if (!word.empty())
			methods.insert(word);
	}
}

void RouteConfig::setRoot(const std::string& root) {
	this->root = root;
}

void RouteConfig::setIndex(const std::string& index) {
	std::istringstream iss(index);
	while (iss) {
		std::string word;
		iss >> word;
		if (!word.empty())
			this->index.insert(word);
	}
}

void RouteConfig::setAllowedMethods(const std::string& methods) {
	std::istringstream iss(methods);
	while (iss) {
		std::string word;
		iss >> word;
		if (!word.empty())
			allowed_methods.insert(word);
	}
}

void setAutoindex(const std::string& autoindex);
void setUploadPath(const std::string& upload_path);
void setCgiExtension(const std::string& cgi_extension);
void setUploadEnable(const std::string& upload_enable);
void setReturnCodes(const std::string& return_codes);


void RouteConfig::initializeDirectiveMap() {
	directiveMap["root"].handler = &RouteConfig::setRoot;
	directiveMap["index"].handler = &RouteConfig::setIndex;
	directiveMap["methods"].handler = &RouteConfig::addMethod;
	directiveMap["allowed_methods"].handler = &RouteConfig::setAllowedMethods;
}

void RouteConfig::applyDirective(const std::string& key, const std::string& value) {
    std::map<std::string, DirectiveHandler>::const_iterator it = directiveMap.find(key);
    if (it != directiveMap.end()) {
        (this->*(it->second.handler))(value);
    } else {
        throw std::runtime_error("Unknown directive: " + key);
    }
}