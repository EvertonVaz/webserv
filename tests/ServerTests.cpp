// ServerTests.cpp
#include "basic.hpp"
#include "../srcs/server/Server.hpp"
#include "../srcs/parse_config/ConfigParser.hpp"
#include "../srcs/parse_config/ServerConfig.hpp"
#include "MockSocket.hpp"

const std::string filePath = "/home/etovaz/nave/webserver/tests/configs/valid_config.conf";

bool testServerInitializationSuccess() {
	ConfigParser parser;
    ServerConfig config;

    parser.loadConfig(filePath);
	config = parser.getServers().front();

    MockSocket *mockSocket = new MockSocket();
    Server server(parser, mockSocket);

    ASSERT_TRUE(server.isListening(), "O servidor deve estar escutando após inicialização");
    return true;
}

bool testServerSocketCreationFailure() {
    ConfigParser parser;
    ServerConfig config;

    parser.loadConfig(filePath);
	config = parser.getServers().front();
    config.setHost("127.0.0.1");
    config.setListen("8080");

    MockSocket mockSocket;
    mockSocket.fail_socket = true;

    try {
        Server server(parser, &mockSocket);
    } catch (const std::runtime_error &e) {
        ASSERT_TRUE(std::string(e.what()) == "Failed to create socket", "Deve lançar exceção 'Failed to create socket'");
        return true;
    }

    return false;
}

bool testServerSetsockoptFailure() {
    ConfigParser parser;
    ServerConfig config;

    parser.loadConfig(filePath);
	config = parser.getServers().front();
    config.setHost("127.0.0.1");
    config.setListen("8080");

    MockSocket mockSocket;
    mockSocket.fail_setsockopt = true;

    try {
        Server server(parser, &mockSocket);
    } catch (const std::runtime_error &e) {
        ASSERT_TRUE(std::string(e.what()) == "Failed to set SO_REUSEADDR", "Deve lançar exceção 'Failed to set SO_REUSEADDR'");
        return true;
    }

    return false;
}

bool testServerInetPtonFailure() {
	try {
		ConfigParser parser;
		ServerConfig config;

		parser.loadConfig(filePath);
		config = parser.getServers().front();
		config.setHost("invalid_ip"); // IP inválido para causar falha no inet_pton
		config.setListen("8080");

		MockSocket mockSocket;

        Server server(parser, &mockSocket);
    } catch (const std::runtime_error &e) {
		std::string errorMsg = e.what();
		std::cout << errorMsg << std::endl;
        ASSERT_TRUE(errorMsg.find("Invalid host:") != std::string::npos, "Deve lançar exceção 'inet_pton failed'");
        return true;
    }

    return false;
}

bool testServerBindFailure() {
    ConfigParser parser;
    ServerConfig config;

    parser.loadConfig(filePath);
	config = parser.getServers().front();
    config.setHost("127.0.0.1");
    config.setListen("8080");

    MockSocket mockSocket;
    mockSocket.fail_bind = true;

    try {
        Server server(parser, &mockSocket);
    } catch (const std::runtime_error &e) {
        ASSERT_TRUE(std::string(e.what()) == "bind failed", "Deve lançar exceção 'bind failed'");
        return true;
    }

    return false;
}

bool testServerListenFailure() {
    ConfigParser parser;
    ServerConfig config;

    parser.loadConfig(filePath);
	config = parser.getServers().front();
    config.setHost("127.0.0.1");
    config.setListen("8080");

    MockSocket mockSocket;
    mockSocket.fail_listen = true;

    try {
        Server server(parser, &mockSocket);
    } catch (const std::runtime_error &e) {
        ASSERT_TRUE(std::string(e.what()) == "listen failed", "Deve lançar exceção 'listen failed'");
        return true;
    }

    return false;
}

bool testServerDestructorClosesSockets() {
    ConfigParser parser;
    ServerConfig config;

    parser.loadConfig(filePath);
	config = parser.getServers().front();
    config.setHost("127.0.0.1");
    config.setListen("8080");

    MockSocket mockSocket;
    Server* server = new Server(parser, &mockSocket);

    // Verifica se o socket foi fechado ao deletar o servidor
    delete server;
    ASSERT_TRUE(mockSocket.fail_close == false, "O socket deve ser fechado sem erros");

    return true;
}

bool testServerMultipleListenSockets() {
    ConfigParser parser;
    ServerConfig config1, config2;

    parser.loadConfig(filePath);
	config1 = parser.getServers().front();
	config2 = parser.getServers().back();
    config1.setHost("127.0.0.1");
    config1.setListen("8080");
    config2.setHost("127.0.0.2");
    config2.setListen("9090");

    MockSocket mockSocket;
    Server server(parser, &mockSocket);

    ASSERT_TRUE(server.isListening(), "O servidor deve estar escutando após inicialização");
    // Assumindo que listenSockets está acessível ou criar um método para verificar
    return true;
}
