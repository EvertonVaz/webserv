/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleMultiPart.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 08:30:54 by Everton           #+#    #+#             */
/*   Updated: 2024/11/28 10:16:04 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PostHandler.hpp"
#include <cstddef>
#include <fstream>
#include <string>



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
