/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requestGettersSetters.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 20:26:19 by Everton           #+#    #+#             */
/*   Updated: 2024/11/24 12:40:24 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPRequest.hpp"
#include <cstddef>
#include <ostream>

bool HTTPRequest::isComplete() const {
    return state == COMPLETE;
}

bool HTTPRequest::hasError() const {
    return state == ERROR;
}

void HTTPRequest::setMaxBodySize(size_t size) {
    maxBodySize = size;
}

std::string HTTPRequest::getMethod() const {
    return method;
}

std::string HTTPRequest::getURI() const {
    return uri;
}

std::string HTTPRequest::getQueryString() const {
    return queryString;
}

std::string HTTPRequest::getHTTPVersion() const {
    return httpVersion;
}

std::map<std::string, std::string> HTTPRequest::getHeaders() const {
    return headers;
}

std::string HTTPRequest::getBody() const {
    return body;
}

size_t HTTPRequest::getContentLength() const {
    return contentLength;
}

std::string HTTPRequest::getContentType() const {
    return contentType;
}

void HTTPRequest::setState(ParseState newState) {
    state = newState;
}

HTTPRequest::ParseState HTTPRequest::getState() const {
    return state;
}

void HTTPRequest::setUploadPath(const std::string& path) {
    uploadPath = path;
}

std::string HTTPRequest::getUploadPath() const {
    return uploadPath;
}

size_t HTTPRequest::getMaxBodySize() const {
    return maxBodySize;
}


std::ostream& operator<<(std::ostream& os, const HTTPRequest& request) {
    os  << "Method: " << request.getMethod() << "\n"
        << "URI: " << request.getURI() << "\n"
        << "Query String: " << request.getQueryString() << "\n"
        << "HTTP Version: " << request.getHTTPVersion() << "\n"
        << "Headers: \n";
    std::map<std::string, std::string> headers = request.getHeaders();
    for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it) {
        os << it->first << ": " << it->second << "\n";
    }
    os  << "Content Type: " << request.getContentType() << "\n"
        << "Content Length: " << request.getContentLength() << "\n"
        << "Max Body Size: " << request.getMaxBodySize() << "\n"
        << "Body: " << request.getBody() << "\n"
        << "State: " << request.getState() << "\n"
        << "Upload Path: " << request.getUploadPath() << "\n";
    return os;
}