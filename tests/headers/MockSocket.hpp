#ifndef MOCKSOCKET_HPP
#define MOCKSOCKET_HPP

#include "../../srcs/interfaces/ISocket.hpp"
#include <cstring>
#include <iostream>
#include <map>
#include <queue>
#include <vector>

class MockSocket : public ISocket {
    public:
        // Flags para simular falhas
        bool fail_socket;
        bool fail_setsockopt;
        bool fail_bind;
        bool fail_listen;
        bool fail_accept;
        bool fail_close;
        bool fail_recv;
        bool fail_send;
        bool fail_poll;
        bool fail_fcntl;

        // Rastreamento de chamadas
        std::vector<int> closed_sockets;
        std::vector<int> accepted_sockets;
        std::vector<int> sent_data_sockets;
        std::vector<int> received_data_sockets;

        // Simulação de dados a serem recebidos
        std::map<int, std::queue<std::string> > recv_data;

        // Configuração de respostas para poll
        std::map<int, short> poll_revents; // Mapeia fd para revents

        MockSocket() {
			fail_socket = false;
			fail_setsockopt = false;
			fail_bind = false;
			fail_listen = false;
			fail_accept = false;
			fail_close = false;
            fail_recv = false;
            fail_send = false;
            fail_poll = false;
            fail_fcntl = false;

            std::cout << "MockSocket created" << std::endl;
        }

        ~MockSocket() {
            std::cout << "MockSocket destroyed" << std::endl;
        }

        int socket(int domain, int type, int protocol) {
			(void)domain, (void)type, (void)protocol;
            if (fail_socket) return -1;
            // Retorna um fd de socket fictício, começando em 100 para evitar conflitos
            static int next_fd = 100;
            return next_fd++;
        }

        int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen) {
            (void)sockfd, (void)level, (void)optname, (void)optval, (void)optlen;
			if (fail_setsockopt) return -1;
            return 0;
        }

        int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
			(void)sockfd, (void)addr, (void)addrlen;
            if (fail_bind) return -1;
            return 0;
        }

        int listen(int sockfd, int backlog) {
			(void)backlog, (void)sockfd;
            if (fail_listen) return -1;
            return 0;
        }

        int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen) {
			(void)sockfd, (void)addr, (void)addrlen;
            int af_net = 2;
            int sock_stream = 1;

            if (fail_accept) return -1;

            int client_fd = MockSocket::socket(af_net, sock_stream, 0);
            if (client_fd == -1) return -1;

            accepted_sockets.push_back(client_fd);
            return client_fd;
        }

        int close(int sockfd) {
			(void)sockfd;
            if (fail_close) return -1;

            closed_sockets.push_back(sockfd);
            return 0;
        }

        int recv(int sockfd, void *buf, size_t len, int flags) {
            (void)flags;
            if (fail_recv) return -1;
            if (recv_data.find(sockfd) != recv_data.end() && !recv_data[sockfd].empty()) {
                std::string data = recv_data[sockfd].front();
                recv_data[sockfd].pop();
                size_t bytes_to_copy = std::min(len - 1, data.size());
                memcpy(buf, data.c_str(), bytes_to_copy);
                ((char*)buf)[bytes_to_copy] = '\0';
                received_data_sockets.push_back(sockfd);
                return bytes_to_copy;
            }
            return 0;
        }

        int send(int sockfd, const void *buf, size_t len, int flags) {
            (void)flags, (void)buf;
            if (fail_send) return -1;
            sent_data_sockets.push_back(sockfd);
            return len; // Assume que todos os bytes foram enviados
        }

        int poll(struct pollfd *fds, nfds_t nfds, int timeout) {
            (void)timeout;
            if (fail_poll) return -1;
            // Simular eventos baseados em poll_revents
            int ready = 0;
            for (nfds_t i = 0; i < nfds; ++i) {
                int fd = fds[i].fd;
                if (poll_revents.find(fd) != poll_revents.end()) {
                    fds[i].revents = poll_revents[fd];
                    ready++;
                } else {
                    fds[i].revents = 0;
                }
            }
            return ready;
        }

        int fcntl(int sockfd, int cmd, int arg) {
            (void)sockfd, (void)cmd, (void)arg;
            if (fail_fcntl) return -1;
            return 0;
        }

        // Método para adicionar dados a serem recebidos pelo recv
        void addRecvData(int sockfd, const std::string &data) {
            recv_data[sockfd].push(data);
        }

        // Método para definir eventos de revents para poll
        void setPollRevents(int sockfd, short revents) {
            poll_revents[sockfd] = revents;
        }
};

#endif
