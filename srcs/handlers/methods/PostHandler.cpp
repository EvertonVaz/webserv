/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostHandler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 09:08:56 by Everton           #+#    #+#             */
/*   Updated: 2024/11/26 09:40:12 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include <sys/types.h>
#include "PostHandler.hpp"

PostHandler::PostHandler() {
    _logger = &Logger::getInstance();
}

PostHandler::~PostHandler() {}

void PostHandler::setRequest(const HTTPRequest& request) {
    _request = request;
    _contentType = _request.getContentType();
    _body = _request.getBody();
}

bool PostHandler::handlePost() {
    if (_request.getHeaders().find("content-type") == _request.getHeaders().end()) {
        return false;
    }
    if (_request.getHeaders()["content-type"].find("multipart/form-data") != std::string::npos) {
        return handleMultiPart();
    }
    return false;
}

bool PostHandler::createDirectory(const std::string& path) {
    size_t pos = 0;
    std::string dir;
    int mkdirRet;

    while ((pos = path.find('/', pos)) != std::string::npos) {
        dir = path.substr(0, pos++);
        if (dir.empty()) continue;
        mkdirRet = mkdir(dir.c_str(), 0755);
        if (mkdirRet && errno != EEXIST) {
            return false;
        }
    }
    mkdirRet = mkdir(path.c_str(), 0755);
    if (mkdirRet && errno != EEXIST) {
        return false;
    }
    return true;
}
