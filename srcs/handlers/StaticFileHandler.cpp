/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticFileHandler.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 22:59:21 by Everton           #+#    #+#             */
/*   Updated: 2024/11/19 11:49:59 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "StaticFileHandler.hpp"
#include "ErrorHandler.hpp"
#include "../aux.hpp"
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <dirent.h>
#include <climits>
#include <cstdlib>

StaticFileHandler::StaticFileHandler(ErrorHandler errorHandler, FilePath filePath)
    : filePath(filePath), errorHandler(errorHandler)
{
    directoryListingEnabled = false;
}

StaticFileHandler::~StaticFileHandler() {}

void StaticFileHandler::handleResponse(HTTPResponse& response) {

    if (!filePath.getIsSafe() && filePath.getCanRead()) {
        return errorHandler.handleError(403, response);
    }

    if (!filePath.getPathExist()) {
        return errorHandler.handleError(404, response);
    }

    if (!filePath.getIsFile() && filePath.getIsDirectory()) {
        if (directoryListingEnabled) {
            listDirectory(filePath.getPath(), response);
        } else {
            return errorHandler.handleError(403, response);
        }
    } else if (filePath.getIsFile()) {
        serveFile(filePath.getPath(), response);
    } else {
        return errorHandler.handleError(403, response);
    }
}

void StaticFileHandler::serveFile(const std::string& filePath, HTTPResponse& response) {
    std::ifstream file(filePath.c_str(), std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        return errorHandler.handleError(403, response);;
    }

    std::string extension = filePath.substr(filePath.find_last_of('.'));
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    response.setStatusCode(200);
    response.setBody(content);
    response.addHeader("Content-Type", getContentType(extension));

    file.close();
}

void StaticFileHandler::listDirectory(const std::string& dirPath, HTTPResponse& response) {
    DIR* dir = opendir(dirPath.c_str());
    if (!dir)
        return errorHandler.handleError(500, response);

    std::ostringstream body;
    body << "<!DOCTYPE html>";
    body << "<html lang='en'>";
    body << "<head>";
    body << "    <meta charset='UTF-8'>";
    body << "    <meta name='viewport' content='width=device-width, initial-scale=1.0'>";
    body << "    <title>Index of " << filePath.getUri() << "</title>";
    body << "    <link rel='stylesheet' href='" << filePath.getUri() << "styles.css'>";
    body << "</head>";
    body << "<body>";
    body << "    <div class='container'>";
    body << "        <h1>Index of " << filePath.getUri() << "</h1>";
    body << "        <ul>";

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        std::string name = entry->d_name;
        if (name != "." && name != "..") {
            std::string fullPath = filePath.getUri() + name;
            body << "            <li><a href='" << fullPath << "'>" << name << "</a></li>";
        }
    }

    body << "        </ul>";
    body << "        <a href='/' class='button'>Voltar para a página inicial</a>";
    body << "    </div>";
    body << "</body>";
    body << "</html>";

    closedir(dir);
    response.setStatusCode(200);
    response.setBody(body.str());
    response.addHeader("Content-Type", "text/html");
}

void StaticFileHandler::setDirectoryListingEnabled(bool enabled) {
    directoryListingEnabled = enabled;
}