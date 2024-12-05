/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 20:55:57 by Everton           #+#    #+#             */
/*   Updated: 2024/12/05 08:42:08 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPRequest.hpp"
#include <cstddef>
#include <fcntl.h>
#include <sstream>
#include <algorithm>
#include "../aux.hpp"

HTTPRequest::HTTPRequest() {
    state = REQUEST_LINE;
    contentLength = 0;
    chunkedEncoding = false;
    this->maxBodySize = 0;
}

HTTPRequest::~HTTPRequest() {}

bool HTTPRequest::parseRequestLine(const std::string& line) {
    std::istringstream iss(line);
    if (!(iss >> method >> uri >> httpVersion)) {
        return setState(ERROR400), false;
    }
    std::string methods[] = {"GET", "POST", "DELETE"};
    bool validMethod = false;
    for (size_t i = 0; i < 3; ++i) {
        if (method == methods[i]) {
            validMethod = true;
            break;
        }
    }
    if (!validMethod) {
        return setState(ERROR501), false;
    }
    if (httpVersion != "HTTP/1.1") {
        return setState(ERROR505), false;
    }
    return true;
}

void HTTPRequest::handleQueryString() {
    size_t pos = uri.find("?");
    if (pos != std::string::npos) {
        queryString = uri.substr(pos + 1);
        uri.erase(pos);
    }
}

bool HTTPRequest::endOfHeader() {
	std::map<std::string, std::string>::iterator it;

    handleQueryString();
	it = headers.find("content-length");
	if (it != headers.end()) {
		contentLength = static_cast<size_t>(std::atoi(it->second.c_str()));
	}

    it = headers.find("content-type");
	if (it != headers.end()) {
		contentType = it->second;
	}

	it = headers.find("transfer-encoding");
	if (it != headers.end() && it->second == "chunked") {
		chunkedEncoding = true;
	}

	if (contentLength > 0 || chunkedEncoding) {
		state = BODY;
	} else {
		state = COMPLETE;
	}
	return true;
}

bool HTTPRequest::parseHeaderLine(const std::string& line) {
    if (line.empty()) {
		return endOfHeader();
    }

    size_t pos = line.find(":");
    if (pos == std::string::npos) {
        return setState(ERROR400), false;
    }

    std::string key = line.substr(0, pos);
    std::string value = line.substr(pos + 1);

    key.erase(std::remove(key.begin(), key.end(), ' '), key.end());
    value.erase(0, value.find_first_not_of(" "));

    std::transform(key.begin(), key.end(), key.begin(), ::tolower);

    headers[key] = value;
    return true;
}

bool HTTPRequest::chunkedEncondingHandler() {
    while (true) {
        size_t pos = rawData.find("\r\n");
        if (pos == std::string::npos)
            return false;

        std::string chunkSizeStr = rawData.substr(0, pos);
        size_t chunkSize = std::strtoul(chunkSizeStr.c_str(), NULL, 16);
        if (chunkSize == 0)
            return setState(COMPLETE), true;
        if (rawData.size() < pos + 2 + chunkSize + 2)
            return false;
        if (maxBodySize > 0 && body.size() + chunkSize > maxBodySize)
            return setState(ERROR413), false;

        body += rawData.substr(pos + 2, chunkSize);
        rawData.erase(0, pos + 2 + chunkSize + 2);
    }
}

bool HTTPRequest::parseBody() {
    if (chunkedEncoding) {
        return (chunkedEncondingHandler());
    } else if (contentLength > 0) {
        bool isNotSafe = !(body.size() <= contentLength && body.size() <= maxBodySize);
        if (isNotSafe || contentLength > maxBodySize || body.size() > 512 MB)
            return setState(ERROR413), false;

        body += rawData;
        rawData.clear();
        if (contentLength == body.size())
            setState(COMPLETE);
        return true;
    }
    return setState(COMPLETE), true;
}

std::string HTTPRequest::lineConstructor() {
    size_t pos = rawData.find("\r\n");

    if (pos == std::string::npos)
        return "";
    std::string line = rawData.substr(0, pos);
    rawData.erase(0, pos + 2);
    return line;
}

void HTTPRequest::appendData(const std::string& data, std::vector<ServerConfig> serverConfigs) {
    rawData += data;
    while (state != COMPLETE && state < ERROR400) {
        if (state == REQUEST_LINE) {
            std::string line = lineConstructor();
            if (line == "")
                return;
            if (!parseRequestLine(line)) {
                return;
            }
            state = HEADERS;
        } else if (state == HEADERS) {
            std::string line = lineConstructor();
            if (!parseHeaderLine(line)) {
                return;
            }
        } else if (state == BODY && rawData.size() > 0) {
            maxBodySize = selectConfig(*this, serverConfigs).getMaxBodySize();
            if (!parseBody()) {
                return;
            }
        }
        if (rawData.size() == 0)
            return;
    }
}
