#include "StaticFileHandler.hpp"
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <dirent.h>

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
    std::string filePath = rootDirectory + uri;

    if (!indexFiles.empty())
        filePath += indexFiles.begin()->c_str();
    if (filePath[filePath.length() - 1] == '/')
        filePath.erase(filePath.length() - 1);
    if (!isPathSafe(filePath)) {
        response.setStatusCode(403);
        response.setBody("<html><body><h1>403 Forbidden</h1></body></html>");
        response.addHeader("Content-Type", "text/html");
        return;
    }

    struct stat statBuf;
    if (stat(filePath.c_str(), &statBuf) == -1) {
        response.setStatusCode(404);
        response.setBody("<html><body><h1>404 Not Found</h1></body></html>");
        response.addHeader("Content-Type", "text/html");
        return;
    }

    if (S_ISDIR(statBuf.st_mode)) {
        if (directoryListingEnabled) {
            return listDirectory(filePath, response);
        } else {
            response.setStatusCode(403);
            response.setBody("<html><body><h1>403 Forbidden</h1></body></html>");
            response.addHeader("Content-Type", "text/html");
            return ;
        }
    } else if (S_ISREG(statBuf.st_mode)) {
        return serveFile(filePath, response);
    } else {
        response.setStatusCode(403);
        response.setBody("<html><body><h1>403 Forbidden</h1></body></html>");
        response.addHeader("Content-Type", "text/html");
        return ;
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
        response.setStatusCode(404);
        response.setBody("<html><body><h1>404 Not Found</h1></body></html>");
        response.addHeader("Content-Type", "text/html");
        return;
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
    if (!dir) {
        response.setStatusCode(500);
        response.setBody("<html><body><h1>500 Internal Server Error</h1></body></html>");
        response.addHeader("Content-Type", "text/html");
        return;
    }

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
