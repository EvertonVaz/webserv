/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorHandler.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 22:59:05 by Everton           #+#    #+#             */
/*   Updated: 2024/11/06 22:59:06 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../http/HTTPResponse.hpp"
#include <string>

class ErrorHandler {
public:
    ErrorHandler();
    ~ErrorHandler();

    void handleError(int statusCode, HTTPResponse& response, const std::string& customMessage = "");
};
