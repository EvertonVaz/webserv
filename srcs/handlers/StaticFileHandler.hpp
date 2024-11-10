/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticFileHandler.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 22:59:26 by Everton           #+#    #+#             */
/*   Updated: 2024/11/09 18:18:51 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include "../http/HTTPResponse.hpp"
#include "../utils/FilePath.hpp"
#include "ErrorHandler.hpp"

class StaticFileHandler {
    private:
        FilePath filePath;
        ErrorHandler errorHandler;
        bool directoryListingEnabled;

        void serveFile(const std::string& filePath, HTTPResponse& response);
        void listDirectory(const std::string& dirPath, HTTPResponse& response);

    public:
        StaticFileHandler(ErrorHandler errorHandler, FilePath filePath);
        ~StaticFileHandler();

        void handleResponse(HTTPResponse& response);

        void setDirectoryListingEnabled(bool enabled);
};
