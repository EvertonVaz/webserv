/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorHandler.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 22:59:05 by Everton           #+#    #+#             */
/*   Updated: 2024/11/07 18:55:53 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../http/HTTPResponse.hpp"
#include <string>

class ErrorHandler {
    private:
        std::string errorPagesPath;
        
    public:
        ErrorHandler(std::string errorPagesPath);
        ~ErrorHandler();

        void handleError(int statusCode, HTTPResponse& response);
        void setErrorPagesPath(const std::string& path);
};
