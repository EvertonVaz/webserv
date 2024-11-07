#pragma once
#include "../http/HTTPRequest.hpp"
#include "../http/HTTPResponse.hpp"
#include <string>

class StaticFileHandler {
    private:
        bool uriIsFile;
        std::string uri;
        std::string rootDirectory;
        bool directoryListingEnabled;
        std::set<std::string> indexFiles;

        bool isPathSafe(const std::string& path);
        void serveFile(const std::string& filePath, HTTPResponse& response);
        void listDirectory(const std::string& dirPath, HTTPResponse& response);
        std::string getContentType(const std::string& extension);

    public:
        StaticFileHandler();
        ~StaticFileHandler();

        void handleRequest(HTTPResponse& response);

        void setUri(const std::string& uri);
        void setRootDirectory(const std::string& rootDir);
        void setDirectoryListingEnabled(bool enabled);
        void setIndexFiles(const std::set<std::string>& indexFiles);
};
