/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostHandler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 09:08:56 by Everton           #+#    #+#             */
/*   Updated: 2024/11/27 12:44:23 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PostHandler.hpp"
#include <sys/stat.h>

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
    if (!directoryExists(_request.getUploadPath())) {
        return false;
    }
    if (_request.getHeaders().find("content-type") == _request.getHeaders().end()) {
        return false;
    }
    if (_request.getHeaders()["content-type"].find("multipart/form-data") != std::string::npos) {
        return handleMultiPart();
    }
    return false;
}

bool PostHandler::directoryExists(const std::string& path) {
    struct stat st;
    if (stat(path.c_str(), &st) != 0) {
        return false;
    }
    return (st.st_mode & S_IFDIR) != 0;
}
