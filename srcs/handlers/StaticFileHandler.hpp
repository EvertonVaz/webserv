/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticFileHandler.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 22:59:26 by Everton           #+#    #+#             */
/*   Updated: 2024/11/08 13:28:32 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include "../http/HTTPResponse.hpp"
#include "../utils/FilePath.hpp"

class StaticFileHandler {
    private:
        FilePath filePath;
        std::string errorPagesPath;
        bool directoryListingEnabled;

        void serveFile(const std::string& filePath, HTTPResponse& response);
        void listDirectory(const std::string& dirPath, HTTPResponse& response);

    public:
        StaticFileHandler(std::string errorPagesPath, FilePath filePath);
        ~StaticFileHandler();

        void handleResponse(HTTPResponse& response);

        void setDirectoryListingEnabled(bool enabled);
};
