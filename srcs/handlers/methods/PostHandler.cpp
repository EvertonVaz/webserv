/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostHandler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 09:08:56 by Everton           #+#    #+#             */
/*   Updated: 2024/12/07 16:03:18 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include <sys/stat.h>
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

bool PostHandler::directoryExists(const std::string& path) {
    struct stat st;
    if (stat(path.c_str(), &st) != 0) {
        return false;
    }
    return (st.st_mode & S_IFDIR) != 0;
}

bool find(std::string str, std::string sep1, std::string sep2, size_t& pos) {
    pos = 0;
    if (str.find(sep1, pos) < str.find(sep2, pos))
        pos = str.find(sep1, pos);
    else
        pos = str.find(sep2, pos);
    if (str.find('\n', 0) == 0)
        pos += 1;
    return pos != std::string::npos;
}

bool PostHandler::handleHeaders() {
    size_t pos = 0;
    size_t headersEnd = _body.find("\r\n\r\n");
    std::string header = _body.substr(0, headersEnd);

    while (find(header, ":", "=", pos)) {
        std::string key = header.substr(0, pos);
        header = header.substr(pos + 1);
        find(header, ";", "\r\n", pos);

        std::string value = header.substr(0, pos);
        key.erase(std::remove(key.begin(), key.end(), ' '), key.end());
        value.erase(0, value.find_first_not_of(" "));
        std::transform(key.begin(), key.end(), key.begin(), ::tolower);
        value.erase(std::remove(value.begin(), value.end(), '\"'), value.end());
        _headers[key] = value;
        header = header.substr(pos + 1);
    }
    if (_headers.empty()) {
        _logger->log(Logger::ERROR, "Failed to parse headers");
        return false;
    }
    _body = _body.substr(headersEnd + 4);
    return true;
}

bool PostHandler::saveFileNoName(const std::string& content) {
    std::string filePath = _request.getUploadPath() + "/uploaded_file";
    std::ofstream outFile(filePath.c_str(), std::ios::binary);
    if (!outFile.is_open()) {
        return false;
    }
    outFile.write(content.c_str(), content.size());
    outFile.close();
    return true;
}

int PostHandler::handlePost() {
    if (!directoryExists(_request.getUploadPath())) {
        return 404;
    }
    if (_request.getHeaders().find("content-type") == _request.getHeaders().end()) {
        return 400;
    }
    if (_request.getHeaders()["content-type"].find("multipart/form-data") != std::string::npos) {
        return handleMultiPart();
    }
    else if (!saveFileNoName(_request.getBody()))
        return 500;
    return 201;
}