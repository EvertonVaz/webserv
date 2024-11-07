/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 10:51:09 by Everton           #+#    #+#             */
/*   Updated: 2024/11/07 20:20:50 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPResponse.hpp"
#include <sstream>

HTTPResponse::HTTPResponse() {
	httpVersion = "HTTP/1.1";
	statusCode = 200;
	reasonPhrase = "OK";
	closeConnection = false;
}

HTTPResponse::~HTTPResponse() {
}

static inline std::string getHTTPDate() {
    time_t now = time(NULL);
    if (now == ((time_t)-1)) {
        throw std::runtime_error("Error getting current time");
    }

    struct tm *gmt = gmtime(&now);
    if (gmt == NULL) {
        throw std::runtime_error("Error converting time to GMT");
    }

    char buf[30];
    size_t bytes = strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", gmt);
    if (bytes == 0) {
        throw std::runtime_error("Error formatting time");
    }
    return std::string(buf);
}

std::string HTTPResponse::generateResponse(){
    std::ostringstream responseStream;

    responseStream << httpVersion << " " << statusCode << " " << reasonPhrase << "\r\n";
    if (headers.find("Date") == headers.end()) {
		headers["Date"] = getHTTPDate();
    }
    if (headers.find("Server") == headers.end()) {
        headers["Server"] = "MyWebServer/1.0";
    }
    if (closeConnection) {
        headers["Connection"] = "close";
    } else {
        headers["Connection"] = "keep-alive";
    }
    if (headers.find("Content-Length") == headers.end()) {
		std::ostringstream oss;
		oss << body.size();
		headers["Content-Length"] = oss.str();
	}
    for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); ++it) {
        responseStream << it->first << ": " << it->second << "\r\n";
    }
    responseStream << "\r\n";
    responseStream << body;
    return responseStream.str();
}
