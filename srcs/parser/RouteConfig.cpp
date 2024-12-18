/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RouteConfig.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 19:58:50 by Everton           #+#    #+#             */
/*   Updated: 2024/12/03 09:56:33 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RouteConfig.hpp"
#include <iostream>
#include <sstream>

RouteConfig::RouteConfig() {
    path = "/";
    autoindex = false;
    root = "";
    upload_enable = false;
    upload_path = "";
    index.push_back("index.html");
    methods.insert("GET");
    cgi_extensions = std::set<std::string>();
    allowed_methods.insert("GET");
    return_codes = std::map<int, std::string>();

    initializeDirectiveMap();
}

RouteConfig::RouteConfig(const RouteConfig& other) {
    *this = other;
}

RouteConfig& RouteConfig::operator=(const RouteConfig& other) {
    path = other.path;
    autoindex = other.autoindex;
    root = other.root;
    upload_enable = other.upload_enable;
    upload_path = other.upload_path;
    index = other.index;
    methods = other.methods;
    cgi_extensions = other.cgi_extensions;
    allowed_methods = other.allowed_methods;
    return_codes = other.return_codes;

    return *this;
}

RouteConfig::~RouteConfig() {}

bool RouteConfig::getAutoindex() const {
	return autoindex;
}

bool RouteConfig::getUploadEnable() const {
	return upload_enable;
}

std::string RouteConfig::getPath() const {
    return path;
}

std::string RouteConfig::getRoot() const {
	return root;
}

std::string RouteConfig::getUploadPath() const {
	return upload_path;
}

std::list<std::string> RouteConfig::getIndex() const {
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

void RouteConfig::setMethods(const std::string& method) {
	std::istringstream iss(method);
	while (iss) {
		std::string word;
		iss >> word;
		if (!word.empty())
			methods.insert(word);
	}
}

void RouteConfig::setPath(const std::string& path) {
    if (path[path.length() - 1] != '/')
        this->path = path + "/";
    else
        this->path = path;
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
			this->index.push_front(word);
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
    directiveMap["methods"].handler = &RouteConfig::setMethods;
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
        return (this->*(it->second.handler))(value);
    } else {
        throw std::runtime_error("Unknown directive: " + key);
    }
}