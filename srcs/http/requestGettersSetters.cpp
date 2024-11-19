/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requestGettersSetters.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 20:26:19 by Everton           #+#    #+#             */
/*   Updated: 2024/11/19 16:28:04 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPRequest.hpp"

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
