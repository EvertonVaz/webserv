/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RouteConfig.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 19:58:50 by Everton           #+#    #+#             */
/*   Updated: 2024/10/11 14:09:17 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RouteConfig.hpp"
#include <sstream>

RouteConfig::RouteConfig() {
	initializeDirectiveMap();
}

bool RouteConfig::getAutoindex() const {
	return autoindex;
}

bool RouteConfig::getUploadEnable() const {
	return upload_enable;
}

std::string RouteConfig::getRoot() const {
	return root;
}

std::string RouteConfig::getUploadPath() const {
	return upload_path;
}

std::set<std::string> RouteConfig::getIndex() const {
	return index;
}

std::set<std::string> RouteConfig::getCgiExtensions() const {
	return cgi_extensions;
}

std::set<std::string> RouteConfig::getAllowedMethods() const {
	return allowed_methods;
}

std::map<int, std::string> RouteConfig::getReturnCodes() const {
	return return_codes;
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

void RouteConfig::setAutoindex(const std::string& autoindex) {
    if (autoindex == "on")
        this->autoindex = true;
    else if (autoindex == "off")
        this->autoindex = false;
    else
        throw std::runtime_error("Invalid value for autoindex: " + autoindex);
}

void RouteConfig::setUploadPath(const std::string& upload_path) {
    this->upload_path = upload_path;
}

void RouteConfig::setCgiExtension(const std::string& cgi_extension) {
    std::istringstream iss(cgi_extension);
    while (iss) {
        std::string word;
        iss >> word;
        if (!word.empty())
            this->cgi_extensions.insert(word);
    }
}

void RouteConfig::setUploadEnable(const std::string& upload_enable) {
    if (upload_enable == "on")
        this->upload_enable = true;
    else if (upload_enable == "off")
        this->upload_enable = false;
    else
        throw std::runtime_error("Invalid value for upload_enable: " + upload_enable);
}

void RouteConfig::setReturnCodes(const std::string& return_codes) {
    std::istringstream iss(return_codes);
    while (iss) {
        std::string code_str;
        iss >> code_str;
        if (code_str.empty())
            continue;

        int code;
        std::istringstream code_iss(code_str);
        code_iss >> code;
        if (code_iss.fail())
            throw std::runtime_error("Invalid return code: " + code_str);

        std::string uri;
        iss >> uri;
        if (uri.empty())
            throw std::runtime_error("Missing URI for return code: " + code_str);

        this->return_codes[code] = uri;
    }
}

void RouteConfig::initializeDirectiveMap() {
    directiveMap["root"].handler = &RouteConfig::setRoot;
    directiveMap["index"].handler = &RouteConfig::setIndex;
    directiveMap["methods"].handler = &RouteConfig::addMethod;
    directiveMap["allowed_methods"].handler = &RouteConfig::setAllowedMethods;
    directiveMap["autoindex"].handler = &RouteConfig::setAutoindex;
    directiveMap["upload_path"].handler = &RouteConfig::setUploadPath;
    directiveMap["cgi_extension"].handler = &RouteConfig::setCgiExtension;
    directiveMap["upload_enable"].handler = &RouteConfig::setUploadEnable;
    directiveMap["return_codes"].handler = &RouteConfig::setReturnCodes;
}

void RouteConfig::applyDirective(const std::string& key, const std::string& value) {
    std::map<std::string, DirectiveHandler>::const_iterator it = directiveMap.find(key);
    if (it != directiveMap.end()) {
        (this->*(it->second.handler))(value);
    } else {
        throw std::runtime_error("Unknown directive: " + key);
    }
}