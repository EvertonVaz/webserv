/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 10:05:26 by Everton           #+#    #+#             */
/*   Updated: 2024/11/28 09:58:37 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ErrorHandler.hpp"
#include "../logger/Logger.hpp"
#include "../utils/FilePath.hpp"
#include "../http/HTTPRequest.hpp"
#include "../http/HTTPResponse.hpp"

class CGIHandler {
    private:
        ErrorHandler& errorHandler;
        FilePath& filePath;
        const HTTPRequest& request;
        std::vector<std::string> envVarsStorage;
        Logger* logger;

        void executeCGI(HTTPResponse& response);
        void setEnvironment(void);
        void handleExec(int *inputPipe, int *outputPipe);
        void handlePOST(int *inputPipe, int *outputPipe, const HTTPRequest& request);
        std::string readCGI(int *outputPipe);
        void fillResponse(std::string cgiOutput, HTTPResponse& response);

    public:
        CGIHandler(ErrorHandler& errorHandler, FilePath& filePath, const HTTPRequest& request);
        ~CGIHandler();


        void handleResponse(HTTPResponse& response);
};
