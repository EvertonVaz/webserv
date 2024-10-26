/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socketImp.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 12:06:37 by Everton           #+#    #+#             */
/*   Updated: 2024/10/24 12:13:39 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketImp.hpp"
#include <sys/socket.h>

int SocketImp::socket(int domain, int type, int protocol) {
	return ::socket(domain, type, protocol);
}

int SocketImp::setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen) {
	return ::setsockopt(sockfd, level, optname, optval, optlen);
}

int SocketImp::bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
	return ::bind(sockfd, addr, addrlen);
}

int SocketImp::listen(int sockfd, int backlog) {
	return ::listen(sockfd, backlog);
}

int SocketImp::accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen) {
	return ::accept(sockfd, addr, addrlen);
}

int SocketImp::close(int sockfd) {
	return ::close(sockfd);
}

int SocketImp::recv(int sockfd, void *buf, size_t len, int flags) {
	return ::recv(sockfd, buf, len, flags);
}