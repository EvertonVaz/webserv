// MockSocket.hpp
#ifndef MOCKSOCKET_HPP
#define MOCKSOCKET_HPP

#include "../srcs/interfaces/ISocket.hpp"
#include <iostream>
#include <vector>

class MockSocket : public ISocket {
    public:
		bool fail_socket;
		bool fail_setsockopt;
		bool fail_bind;
		bool fail_listen;
		bool fail_accept;
		bool fail_close;
        bool fail_recv;
        std::vector<int> closed_sockets;

        MockSocket() {
			fail_socket = false;
			fail_setsockopt = false;
			fail_bind = false;
			fail_listen = false;
			fail_accept = false;
			fail_close = false;
            fail_recv = false;

            std::cout << "MockSocket created" << std::endl;
        }

        ~MockSocket() {
            std::cout << "MockSocket destroyed" << std::endl;
        }

        virtual int socket(int domain, int type, int protocol) {
			(void)domain, (void)type, (void)protocol;
            if (fail_socket) return -1;
            return 42;
        }

        virtual int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen) {
            (void)sockfd, (void)level, (void)optname, (void)optval, (void)optlen;
			if (fail_setsockopt) return -1;
            return 0;
        }

        virtual int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
			(void)sockfd, (void)addr, (void)addrlen;
            if (fail_bind) return -1;
            return 0;
        }

        virtual int listen(int sockfd, int backlog) {
			(void)backlog, (void)sockfd;
            if (fail_listen) return -1;
            return 0;
        }

        virtual int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen) {
			(void)sockfd, (void)addr, (void)addrlen;
            if (fail_accept) return -1;
            return 43; // Retorna um descriptor de socket fictício para nova conexão
        }

        virtual int close(int sockfd) {
			(void)sockfd;
            if (fail_close) return -1;
            closed_sockets.push_back(sockfd);
            return 0;
        }

        virtual int recv(int sockfd, void *buf, size_t len, int flags) {
            (void)sockfd, (void)buf, (void)len, (void)flags;
            if (fail_recv) return -1;
            return 0;
        }
};

#endif
