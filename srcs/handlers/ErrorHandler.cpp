/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorHandler.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 22:59:13 by Everton           #+#    #+#             */
/*   Updated: 2024/11/25 09:52:13 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorHandler.hpp"
#include <sstream>
#include <fstream>

ErrorHandler::ErrorHandler(std::string errorPagesPath) {
    this->errorPagesPath = errorPagesPath;
}

ErrorHandler::~ErrorHandler() {}

std::string defaultPage(std::string filePath) {
    std::string message = "Problema de permissão ou arquivo não encontrado: " + filePath;
    return "<!DOCTYPE html> \
            <html lang='pt-BR'> \
            <head> \
                <meta charset='UTF-8'> \
                <title>Error!</title> \
                <style> \
                    body { \
                        background-color: #f8f9fa; \
                        color: #343a40; \
                        font-family: Arial, sans-serif; \
                        text-align: center; \
                        padding-top: 100px; \
                    } \
                    h1 { \
                        font-size: 50px; \
                    } \
                    p { \
                        font-size: 20px; \
                    } \
                    a { \
                        color: #007bff; \
                        text-decoration: none; \
                    } \
                    a:hover { \
                        text-decoration: underline; \
                    } \
                </style> \
            </head> \
            <body> \
                <h1>Problemas ao abrir o arquivo de erro</h1> \
                <p>"+ message +"</p> \
                <p><a href='/'>Voltar para a página inicial</a></p> \
            </body> \
            </html>";
}

void ErrorHandler::handleError(int statusCode, HTTPResponse& response) {
    response.setStatusCode(statusCode);
    response.addHeader("Content-Type", "text/html");

    std::ostringstream ss;
    ss << statusCode;
    if (errorPagesPath[errorPagesPath.length() - 1] != '/')
        errorPagesPath += "/";
    std::string filePath = errorPagesPath + ss.str() + ".html";
    std::ifstream file(filePath.c_str());
    if (file.is_open()) {
        std::stringstream buffer;
        buffer << file.rdbuf();
        response.setBody(buffer.str());
        file.close();
    } else {
        std::string body = defaultPage(filePath);
        response.setBody(body);
    }
}

void ErrorHandler::setErrorPagesPath(const std::string& path) {
    errorPagesPath = path;
}