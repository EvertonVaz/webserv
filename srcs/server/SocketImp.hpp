/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socketImp.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 12:04:43 by Everton           #+#    #+#             */
/*   Updated: 2024/10/25 09:31:38 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../interfaces/ISocket.hpp"

class SocketImp : public ISocket {
	public:
		SocketImp() {};
		~SocketImp() {};
		int socket(int domain, int type, int protocol);
		int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
		int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
		int listen(int sockfd, int backlog);
		int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
		int close(int sockfd);
		int recv(int sockfd, void *buf, size_t len, int flags);
		int send(int sockfd, const void *buf, size_t len, int flags);
		int poll(struct pollfd *fds, nfds_t nfds, int timeout);
};