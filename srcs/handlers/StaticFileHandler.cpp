/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticFileHandler.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 22:59:21 by Everton           #+#    #+#             */
/*   Updated: 2024/11/06 23:14:37 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "StaticFileHandler.hpp"
#include "ErrorHandler.hpp"
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <dirent.h>
#include <climits>
#include <cstdlib>

StaticFileHandler::StaticFileHandler() {
    uriIsFile = false;
    directoryListingEnabled = false;
}

StaticFileHandler::~StaticFileHandler() {
}

void StaticFileHandler::setUri(const std::string& uri) {
    std::string filePath = uri;
    if (filePath[filePath.length() - 1] != '/')
        filePath += "/";
    this->uri = filePath;
}

void StaticFileHandler::setRootDirectory(const std::string& rootDir) {
    rootDirectory = rootDir;
}

void StaticFileHandler::setDirectoryListingEnabled(bool enabled) {
    directoryListingEnabled = enabled;
}

void StaticFileHandler::setIndexFiles(const std::set<std::string>& indexFiles) {
    this->indexFiles = indexFiles;
}

void StaticFileHandler::handleRequest(HTTPResponse& response) {
    ErrorHandler errorHandler;
    std::string filePath = rootDirectory + uri;

    if (!indexFiles.empty())
        filePath += indexFiles.begin()->c_str();
    if (filePath[filePath.length() - 1] == '/')
        filePath.erase(filePath.length() - 1);
    if (!isPathSafe(filePath)) {
        errorHandler.handleError(403, response);
        return;
    }

    struct stat statBuf;
    if (stat(filePath.c_str(), &statBuf) == -1) {
        errorHandler.handleError(404, response);
        return;
    }

    if (S_ISDIR(statBuf.st_mode)) {
        if (directoryListingEnabled) {
            listDirectory(filePath, response);
        } else {
            errorHandler.handleError(403, response);
            return;
        }
    } else if (S_ISREG(statBuf.st_mode)) {
        serveFile(filePath, response);
    } else {
        errorHandler.handleError(403, response);
        return;
    }
}

bool StaticFileHandler::isPathSafe(const std::string& path) {
    char resolvedRoot[PATH_MAX];
    char resolvedPath[PATH_MAX];

    realpath(rootDirectory.c_str(), resolvedRoot);
    realpath(path.c_str(), resolvedPath);

    std::string rootStr(resolvedRoot);
    std::string pathStr(resolvedPath);

    return pathStr.compare(0, rootStr.length(), rootStr) == 0;
}

void StaticFileHandler::serveFile(const std::string& filePath, HTTPResponse& response) {
    std::ifstream file(filePath.c_str(), std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        return ErrorHandler().handleError(404, response);;
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
        return ErrorHandler().handleError(500, response);

    std::ostringstream body;
    body << "<html><body>";
    body << "<h1>Index of " << dirPath << "</h1><ul>";

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        std::string name = entry->d_name;
        if (name != "." && name != "..") {
            body << "<li><a href=\"" << uri + name << "\">" << name << "</a></li>";
        }
    }

    body << "</ul></body></html>";
    closedir(dir);
    response.setStatusCode(200);
    response.setBody(body.str());
    response.addHeader("Content-Type", "text/html");
}

std::string StaticFileHandler::getContentType(const std::string& extension) {
    if (extension == ".html") return "text/html";
    if (extension == ".css") return "text/css";
    if (extension == ".js") return "application/javascript";
    if (extension == ".json") return "application/json";
    if (extension == ".png") return "image/png";
    if (extension == ".jpg" || extension == ".jpeg") return "image/jpeg";
    if (extension == ".gif") return "image/gif";
    return "application/octet-stream";
}
