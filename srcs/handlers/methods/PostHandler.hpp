/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostHandler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 09:08:59 by Everton           #+#    #+#             */
/*   Updated: 2024/12/07 16:03:47 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../../http/HTTPRequest.hpp"
#include "../../logger/Logger.hpp"

class PostHandler {
    private:
        HTTPRequest _request;
        std::string _boundary;
        std::string _endBoundary;
        std::string _contentType;
        std::string _body;
        std::map<std::string, std::string> _headers;

        Logger *_logger;

        bool boundaryCreate();
        bool handleHeaders();
        int handleMultiPart();
        bool directoryExists(const std::string& path);
        bool saveFileWithName(const std::string& content);
        bool saveFileNoName(const std::string& content);
    public:
        PostHandler();
        ~PostHandler();
        int handlePost();

        void setRequest(const HTTPRequest& request);
};