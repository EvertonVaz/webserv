/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseGettersSetters.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 20:07:20 by Everton           #+#    #+#             */
/*   Updated: 2024/11/07 20:44:38 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPResponse.hpp"
#include "../aux.hpp"

int HTTPResponse::getStatusCode() const {
    return statusCode;
}

std::string HTTPResponse::getReasonPhrase() const {
    return reasonPhrase;
}

std::string HTTPResponse::getHTTPVersion() const {
    return httpVersion;
}

std::map<std::string, std::string> HTTPResponse::getHeaders() const {
    return headers;
}

std::string HTTPResponse::getBody() const {
    return body;
}

bool HTTPResponse::shouldCloseConnection() const {
    return closeConnection;
}

void HTTPResponse::setStatusCode(int code) {
    std::map<int, std::string> phrases = httpReasonPhrase();
    statusCode = code;
    reasonPhrase = phrases[code];
}

void HTTPResponse::setReasonPhrase(const std::string& phrase) {
    reasonPhrase = phrase;
}

void HTTPResponse::setHTTPVersion(const std::string& version) {
    httpVersion = version;
}

void HTTPResponse::addHeader(const std::string& name, const std::string& value) {
    headers[name] = value;
}

void HTTPResponse::setBody(const std::string& bodyContent) {
    body = bodyContent;
}

void HTTPResponse::setCloseConnection(bool close) {
    closeConnection = close;
}