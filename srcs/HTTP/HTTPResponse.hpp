/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 10:45:21 by Everton           #+#    #+#             */
/*   Updated: 2024/10/31 10:50:32 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <map>

class HTTPResponse {
    private:
        std::string httpVersion;
        int statusCode;
        std::string reasonPhrase;
        std::map<std::string, std::string> headers;
        std::string body;

        bool closeConnection;

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

        // Método para enviar a resposta (pode ser implementado no ConnectionManager)
        // void sendResponse(int clientSockFd, ISocket* socketInterface);
};
