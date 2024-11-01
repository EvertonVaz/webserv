/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 18:01:41 by Everton           #+#    #+#             */
/*   Updated: 2024/10/31 19:32:04 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../http/HTTPRequest.hpp"
#include "../http/HTTPResponse.hpp"
#include "../parser/ServerConfig.hpp"
#include "../parser/RouteConfig.hpp"

class Router {
private:
    ServerConfig serverConfig;

public:
    Router();
    Router(const ServerConfig& config);
    ~Router();

    
};
