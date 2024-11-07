/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticFileHandler.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 22:59:26 by Everton           #+#    #+#             */
/*   Updated: 2024/11/07 09:50:10 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <set>
#include <string>
#include "../http/HTTPResponse.hpp"

class StaticFileHandler {
    private:
        bool uriIsFile;
        std::string uri;
        std::string rootDirectory;
        std::string errorPagesPath;
        bool directoryListingEnabled;
        std::set<std::string> indexFiles;

        bool isPathSafe(const std::string& path);
        void serveFile(const std::string& filePath, HTTPResponse& response);
        void listDirectory(const std::string& dirPath, HTTPResponse& response);
        std::string getContentType(const std::string& extension);

    public:
        StaticFileHandler(std::string errorPagesPath);
        ~StaticFileHandler();

        void handleRequest(HTTPResponse& response);

        void setUri(const std::string& uri);
        void setRootDirectory(const std::string& rootDir);
        void setDirectoryListingEnabled(bool enabled);
        void setIndexFiles(const std::set<std::string>& indexFiles);
        void setErrorPagesPath(const std::string& path);
};
