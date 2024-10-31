/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 20:55:57 by Everton           #+#    #+#             */
/*   Updated: 2024/10/30 21:32:30 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPRequest.hpp"
#include <sstream>
#include <algorithm>

HTTPRequest::HTTPRequest() {
    state = REQUEST_LINE;
    contentLength = 0;
    chunkedEncoding = false;
    bodyBytesReceived = 0;
}

HTTPRequest::~HTTPRequest() {
}

bool HTTPRequest::parseRequestLine(const std::string& line) {
    std::istringstream iss(line);
    if (!(iss >> method >> uri >> httpVersion)) {
        state = ERROR;
        return false;
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
        state = ERROR;
        return false;
    }
    if (httpVersion != "HTTP/1.1" && httpVersion != "HTTP/1.0") {
        state = ERROR;
        return false;
    }

    return true;
}

bool HTTPRequest::endOfHeader() {
	std::map<std::string, std::string>::iterator it;

	it = headers.find("Content-Length");
	if (it != headers.end()) {
		contentLength = static_cast<size_t>(std::atoi(it->second.c_str()));
	}

	it = headers.find("Transfer-Encoding");
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
        state = ERROR;
        return false;
    }

    std::string key = line.substr(0, pos);
    std::string value = line.substr(pos + 1);

    key.erase(std::remove(key.begin(), key.end(), ' '), key.end());
    value.erase(0, value.find_first_not_of(" "));

    std::transform(key.begin(), key.end(), key.begin(), ::tolower);

    headers[key] = value;
    return true;
}

bool HTTPRequest::parseBody() {
    if (chunkedEncoding) {
        while (true) {
            size_t pos = rawData.find("\r\n");
            if (pos == std::string::npos)
				return false;

            std::string chunkSizeStr = rawData.substr(0, pos);
            size_t chunkSize = std::strtoul(chunkSizeStr.c_str(), NULL, 16);
            if (chunkSize == 0) {
                state = COMPLETE;
                return true;
            }

            if (rawData.size() < pos + 2 + chunkSize + 2)
                return false;

            body += rawData.substr(pos + 2, chunkSize);
            rawData.erase(0, pos + 2 + chunkSize + 2);
        }
    } else if (contentLength > 0) {
        if (rawData.size() >= contentLength) {
            body = rawData.substr(0, contentLength);
            rawData.erase(0, contentLength);
            state = COMPLETE;
            return true;
        } else {
            return false;
        }
    } else {
        state = COMPLETE;
        return true;
    }
}

void HTTPRequest::appendData(const std::string& data) {
    rawData += data;

    while (state != COMPLETE && state != ERROR) {
        if (state == REQUEST_LINE) {
            size_t pos = rawData.find("\r\n");
            if (pos == std::string::npos)
				return;
            std::string line = rawData.substr(0, pos);
            rawData.erase(0, pos + 2);
            if (!parseRequestLine(line)) {
                state = ERROR;
                return;
            }
            state = HEADERS;
        } else if (state == HEADERS) {
            size_t pos = rawData.find("\r\n");
            if (pos == std::string::npos)
                return;
            std::string line = rawData.substr(0, pos);
            rawData.erase(0, pos + 2);
            if (!parseHeaderLine(line)) {
                state = ERROR;
                return;
            }
        } else if (state == BODY) {
            if (!parseBody())
                return;
        }
    }
}

bool HTTPRequest::isComplete() const {
    return state == COMPLETE;
}

bool HTTPRequest::hasError() const {
    return state == ERROR;
}

std::string HTTPRequest::getMethod() const {
    return method;
}

std::string HTTPRequest::getURI() const {
    return uri;
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
