/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 10:45:21 by Everton           #+#    #+#             */
/*   Updated: 2024/12/07 16:49:53 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <string>
#include "../logger/Logger.hpp"

class HTTPResponse {
    private:
        bool closeConnection;
        int statusCode;
        std::string body;
        std::string httpVersion;
        std::string reasonPhrase;
        std::map<std::string, std::string> headers;

        Logger* logger;
        std::string getHTTPDate();

    public:
        HTTPResponse();
        ~HTTPResponse();

        void setStatusCode(int code);
        void setReasonPhrase(const std::string& phrase);
        void setHTTPVersion(const std::string& version);
        void addHeader(const std::string& name, const std::string& value);
        void setBody(const std::string& bodyContent);
        void setCloseConnection(bool close);

        int getStatusCode() const;
        std::string getReasonPhrase() const;
        std::string getHTTPVersion() const;
        std::map<std::string, std::string> getHeaders() const;
        std::string getBody() const;
        bool shouldCloseConnection() const;

        std::string generateResponse();
};
