// MockServer.hpp
#ifndef MOCKSERVER_HPP
#define MOCKSERVER_HPP

#include "../../srcs/server/Server.hpp"
#include "MockSocket.hpp"

class MockServer : public Server {
    public:
        MockServer(const ConfigParser &parser, MockSocket *mockSocket)
            : Server(parser, mockSocket) {}

        // Expor o socketInterface e listenSockets
        ISocket* getSocketInterface() const {
            return socketInterface;
        }

        std::vector<int> getListenSockets() const {
            return listenSockets;
        }
};

#endif
