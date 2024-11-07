/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorHandler.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 22:59:13 by Everton           #+#    #+#             */
/*   Updated: 2024/11/06 23:12:27 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorHandler.hpp"
#include <sstream>

ErrorHandler::ErrorHandler() {}

ErrorHandler::~ErrorHandler() {}

void ErrorHandler::handleError(int statusCode, HTTPResponse& response, const std::string& customMessage) {
    response.setStatusCode(statusCode);
    response.addHeader("Content-Type", "text/html");

    std::string message = customMessage;
    if (message.empty()) {
        switch (statusCode) {
            case 400: message = "400 Bad Request"; break;
            case 403: message = "403 Forbidden"; break;
            case 404: message = "404 Not Found"; break;
            case 405: message = "405 Method Not Allowed"; break;
            case 500: message = "500 Internal Server Error"; break;
            default: {
                std::ostringstream oss;
                oss << statusCode << " Error";
                message = oss.str();
                break;
            }
        }
    }

    std::string body = "<html><body><h1>" + message + "</h1></body></html>";
    response.setBody(body);
}
