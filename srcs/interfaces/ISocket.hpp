/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ISocket.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 12:03:48 by Everton           #+#    #+#             */
/*   Updated: 2024/10/25 10:03:27 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <unistd.h>

class ISocket {
	public:
		virtual ~ISocket() {};
		virtual int socket(int domain, int type, int protocol) = 0;
		virtual int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen) = 0;
		virtual int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen) = 0;
		virtual int listen(int sockfd, int backlog) = 0;
		virtual int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen) = 0;
		virtual int close(int sockfd) = 0;
};