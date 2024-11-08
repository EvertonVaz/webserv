/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operators.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 14:31:41 by Everton           #+#    #+#             */
/*   Updated: 2024/11/08 14:39:51 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool Server::operator==(const Server &other) const {
    return (this->listenSockets == other.listenSockets &&
            this->servers == other.servers &&
            this->socketInterface == other.socketInterface);
}

bool Server::operator!=(const Server &other) const {
    return !(*this == other);
}

Server &Server::operator=(const Server &other) {
    this->listenSockets = other.listenSockets;
    this->servers = other.servers;
    this->socketInterface = other.socketInterface;
    return *this;
}

bool Server::operator<(const Server &other) const {
    return this->listenSockets < other.listenSockets;
}

bool Server::operator>(const Server &other) const {
    return this->listenSockets > other.listenSockets;
}

