/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleMultiPart.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 08:30:54 by Everton           #+#    #+#             */
/*   Updated: 2024/11/27 12:43:54 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PostHandler.hpp"
#include <algorithm>
#include <cstddef>
#include <fstream>
#include <string>

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

bool PostHandler::boundaryCreate() {
    std::string boundaryPrefix = "boundary=";
    size_t boundaryPos = _contentType.find(boundaryPrefix);
    if (boundaryPos == std::string::npos) {
        return false;
    }
    _boundary = "--" + _contentType.substr(boundaryPos + boundaryPrefix.length());
    _endBoundary = _boundary + "--";
    if (_boundary.empty() || _endBoundary.empty())
        return false;
    _body = _body.substr(_body.find(_boundary) + _boundary.length()+2);
    return true;
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

bool PostHandler::handleMultiPart() {
    bool hasBoundary = boundaryCreate();
    size_t boundaryPos = 0;
    size_t finalPos = 1;

    while (hasBoundary && boundaryPos < finalPos) {
        if (!handleHeaders()) {
            return false;
        }
        boundaryPos = _body.find(_boundary);
        finalPos = _body.find(_endBoundary);
        std::string content = _body.substr(0, boundaryPos);
        if (!_headers["filename"].empty()) {
            std::string filePath = _request.getUploadPath() + "/" + _headers["filename"];
            std::ofstream outFile(filePath.c_str(), std::ios::binary);
            if (!outFile.is_open()) {
                return false;
            }
            outFile.write(content.c_str(), content.size());
            outFile.close();
            _body = _body.substr(boundaryPos + _boundary.length());
        } else if (!_headers["name"].empty()) {
            std::string key = _headers["name"];
            std::string value = content.substr(0, content.length() - 2);
        }
    }
    return true;
}
