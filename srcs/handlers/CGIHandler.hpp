/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 10:05:26 by Everton           #+#    #+#             */
/*   Updated: 2024/11/10 10:05:29 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ErrorHandler.hpp"
#include "../utils/FilePath.hpp"
#include "../http/HTTPRequest.hpp"
#include "../http/HTTPResponse.hpp"

class CGIHandler {
    private:
        ErrorHandler& errorHandler;
        FilePath& filePath;
        const HTTPRequest& request;
        std::vector<std::string> envVarsStorage;

        void executeCGI(HTTPResponse& response);
        void setEnvironment(char**& envp);
        void handleFork(int inputPipe[], int outputPipe[]);
        void fillResponse(std::string cgiOutput, HTTPResponse& response);

    public:
        CGIHandler(ErrorHandler& errorHandler, FilePath& filePath, const HTTPRequest& request);
        ~CGIHandler();


        void handleResponse(HTTPResponse& response);
};
