/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostHandler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 09:08:59 by Everton           #+#    #+#             */
/*   Updated: 2024/11/26 08:50:25 by Everton          ###   ########.fr       */
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
        bool handleMultiPart();
        bool handleHeaders();
        bool createDirectory(const std::string& path);
    public:
        PostHandler();
        ~PostHandler();
        bool handlePost();

        void setRequest(const HTTPRequest& request);
};