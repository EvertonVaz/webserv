/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticFileHandler.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 22:59:26 by Everton           #+#    #+#             */
/*   Updated: 2024/12/07 16:29:19 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include "../http/HTTPResponse.hpp"
#include "../utils/FilePath.hpp"
#include "ErrorHandler.hpp"

class StaticFileHandler {
    private:
        int statusCode;
        FilePath filePath;
        std::string method;
        ErrorHandler errorHandler;
        bool directoryListingEnabled;

        void serveFile(const std::string& filePath, HTTPResponse& response);
        void listDirectory(const std::string& dirPath, HTTPResponse& response);

    public:
        StaticFileHandler(ErrorHandler errorHandler, FilePath filePath);
        ~StaticFileHandler();

        void handleResponse(HTTPResponse& response);
        void setStatusCode(int code);
        void setDirectoryListingEnabled(bool enabled);
        void setMethod(std::string method);
        void setStatusCodeAndMethod(int code, std::string method);
};
