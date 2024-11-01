/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 18:01:22 by Everton           #+#    #+#             */
/*   Updated: 2024/10/31 19:32:49 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Router.cpp
#include "Router.hpp"
#include "../aux.hpp"

Router::Router() {}

Router::Router(const ServerConfig& config) {
    serverConfig = config;
}

Router::~Router() {
}

