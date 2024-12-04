/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connGettersSetters.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 19:55:40 by Everton           #+#    #+#             */
/*   Updated: 2024/12/04 08:21:20 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConnectionManager.hpp"

std::set<int> ConnectionManager::getListenSockets() const {
    return listenSockets;
}

std::vector<struct pollfd> ConnectionManager::getPollFds() const {
    return pollFds;
}

std::map<int, std::string> ConnectionManager::getClientBuffers() const {
    return clientBuffers;
}

void ConnectionManager::setPollFds(std::vector<struct pollfd> pollFds) {
    this->pollFds = pollFds;
}

void ConnectionManager::setClientBuffers(std::map<int, std::string> clientBuffers) {
    this->clientBuffers = clientBuffers;
}

void ConnectionManager::setPollRevents(int clientSockFd, short revents) {
    for (size_t i = 0; i < pollFds.size(); i++) {
        if (pollFds[i].fd == clientSockFd) {
            pollFds[i].revents = revents;
            return;
        }
    }
}
