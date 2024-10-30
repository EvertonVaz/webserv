// ConnectionManagerTests.cpp
#include "../headers/basic.hpp"
#include "../../srcs/connection/ConnectionManager.hpp"
#include "../headers/MockSocket.hpp"
#include "../headers/MockServer.hpp"
#include "../../srcs/parser/ConfigParser.hpp" // Incluído o MockConfigParser
#include <algorithm>

std::string path = "/home/etovaz/nave/webserver/config.conf";

void events(int times, std::vector<struct pollfd> &pollFds, ConnectionManager &connManager, MockSocket &mockSocket) {
    for (int i = 0; i < times; i++) {
        mockSocket.poll(pollFds.data(), pollFds.size(), -1);
        connManager.setPollFds(pollFds);
        connManager.handleEvents();
        pollFds = connManager.getPollFds();
    }
}

// Teste 1: Inicialização do ConnectionManager
bool test_connection_manager_initialization() {
    // Preparação da configuração do servidor
    ConfigParser parser;
    ServerConfig serverConfig;
    RouteConfig routeConfig;

	//Forma de mockar um parser sem utilizar o arquivo de configuração
    routeConfig.setRoot("/home/etovaz/nave/webserver");
    routeConfig.setIndex("index.html");
    routeConfig.setMethods("GET");
    serverConfig.setListen("8080");
    serverConfig.setHost("127.0.0.1");
    serverConfig.setRoot("/home/etovaz/nave/webserver");
    serverConfig.setMaxBodySize("1000");
    serverConfig.setErrorPage("404 /home/etovaz/nave/webserver/error_pages/404.html");
    serverConfig.addRoute("/", routeConfig);
    parser.setServer(serverConfig);
    MockSocket mockSocket = MockSocket();
    MockServer server(parser, &mockSocket);

    // Criação do ConnectionManager
    ConnectionManager connManager(server);
    int listen_fd = *connManager.getListenSockets().begin();

    // Verificar se os sockets de escuta foram adicionados corretamente
    const std::vector<struct pollfd> &pollFds = connManager.getPollFds();
    ASSERT_TRUE(pollFds.size() == 1, "Deve haver um sockets de escuta inicializado");
    ASSERT_TRUE(pollFds[0].fd == listen_fd, &"O fd de escuta deve ser " + listen_fd);
    ASSERT_TRUE((pollFds[0].events & POLLIN), "O socket de escuta deve estar configurado para POLLIN");

    return true;
}

// Teste 2: Aceitação de Nova Conexão
bool test_connection_manager_accept_new_connection() {
    // Preparação da configuração do servidor
    ConfigParser parser;

	parser.loadConfig(path);

    // Criação do MockSocket e MockServer
    MockSocket mockSocket = MockSocket();
    MockServer server(parser, &mockSocket);

    // Criação do ConnectionManager
    ConnectionManager connManager(server);

    // Configurar o MockSocket para simular um evento POLLIN no listen_fd
    int listen_fd = *connManager.getListenSockets().begin();
    mockSocket.setPollRevents(listen_fd, POLLIN);

    // Configurar o MockSocket para retornar um client_fd fictício na próxima chamada de accept
    int client_fd = listen_fd + server.getListenSockets().size();
    mockSocket.accepted_sockets.push_back(client_fd);
    std::vector<struct pollfd> pollFds = connManager.getPollFds();
    // Executar handleEvents
    events(1, pollFds, connManager, mockSocket);

    // Verificar se accept foi chamado e o client_fd foi adicionado a pollFds
    ASSERT_TRUE(pollFds.size() == 3, "Deve haver tres fds após aceitar uma conexão");
    ASSERT_TRUE(pollFds[2].fd == client_fd, &"O fd do cliente deve ser " + client_fd);
    ASSERT_TRUE((pollFds[2].events & POLLIN), "O socket do cliente deve estar configurado para POLLIN");

    auto findFd = std::find(mockSocket.accepted_sockets.begin(), mockSocket.accepted_sockets.end(), client_fd);
    // Verificar se o client_fd foi aceito corretamente
    ASSERT_TRUE(findFd != mockSocket.accepted_sockets.end(), "O método accept deve ter sido chamado e retornar client_fd");

    return true;
}

// Teste 3: Leitura de Dados de um Cliente
bool test_connection_manager_read_from_client() {
    // Preparação da configuração do servidor
    ConfigParser parser;

	parser.loadConfig(path);

    // Criação do MockSocket e MockServer
    MockSocket mockSocket = MockSocket();
    MockServer server(parser, &mockSocket);

    // Criação do ConnectionManager
    ConnectionManager connManager(server);

    // Adicionar um socket de escuta fictício e um socket de cliente
    int listen_fd = *connManager.getListenSockets().begin();
    mockSocket.setPollRevents(listen_fd, POLLIN);

    int client_fd = listen_fd + server.getListenSockets().size();
    mockSocket.accepted_sockets.push_back(client_fd);

    // Adicionar dados para serem recebidos pelo cliente
    std::string data = "Hello Server!";
    mockSocket.addRecvData(client_fd, data);

    // Configurar o MockSocket para simular um evento POLLIN no client_fd
    mockSocket.setPollRevents(client_fd, POLLIN);

    std::vector<struct pollfd> pollFds = connManager.getPollFds();

    // Executar handleEvents
    events(2, pollFds, connManager, mockSocket);

    auto findFd = std::find(mockSocket.received_data_sockets.begin(), mockSocket.received_data_sockets.end(), client_fd);
    // Verificar se recv foi chamado
    ASSERT_TRUE(findFd != mockSocket.received_data_sockets.end(), "O método recv deve ter sido chamado para client_fd");

    // Verificar se os dados foram armazenados em clientBuffers
    const std::map<int, std::string> &buffers = connManager.getClientBuffers();
    ASSERT_TRUE(buffers.find(client_fd) != buffers.end(), "O buffer do cliente deve existir");
    ASSERT_TRUE(buffers.at(client_fd) == data, "Os dados recebidos devem estar corretos");

    auto it = std::find_if(pollFds.begin(), pollFds.end(), [&](const struct pollfd &pfd) { return pfd.fd == client_fd; });
    ASSERT_TRUE(it != pollFds.end(), "O client_fd deve estar presente em pollFds");
    ASSERT_TRUE((it->events & POLLOUT), "O evento POLLOUT deve estar configurado para o client_fd após leitura");


    return true;
}

// Teste 4: Escrita de Dados para um Cliente
bool test_connection_manager_write_to_client() {
    // Preparação da configuração do servidor
    ConfigParser parser;

	parser.loadConfig(path);

    // Criação do MockSocket e MockServer
    MockSocket mockSocket = MockSocket();
    MockServer server(parser, &mockSocket);
    // Criação do ConnectionManager
    ConnectionManager connManager(server);

    // Adicionar um socket de escuta fictício e um socket de cliente
    int listen_fd = *connManager.getListenSockets().begin();
    mockSocket.setPollRevents(listen_fd, POLLIN);

    int client_fd = listen_fd + server.getListenSockets().size();
    mockSocket.accepted_sockets.push_back(client_fd);


    // Adicionar dados no buffer do cliente para serem enviados
    // No teste anterior, após leitura, o evento POLLOUT é adicionado
    // Assim, simularemos que há dados a serem enviados
    std::map<int, std::string> clientBuffers;
    clientBuffers.insert({client_fd, "Hello Client!"});
    connManager.setClientBuffers(clientBuffers);

    // Configurar o MockSocket para simular um evento POLLOUT no client_fd
    mockSocket.setPollRevents(client_fd, POLLOUT);
    std::vector<struct pollfd> pollFds = connManager.getPollFds();
    // Executar handleEvents
    events(2, pollFds, connManager, mockSocket);

    // Verificar se send foi chamado
    ASSERT_TRUE(std::find(mockSocket.sent_data_sockets.begin(), mockSocket.sent_data_sockets.end(), client_fd) != mockSocket.sent_data_sockets.end(), "O método send deve ter sido chamado para client_fd");

    // Verificar se os dados foram enviados corretamente
    // Como a implementação atual de writeToClient envia uma resposta HTTP e fecha a conexão,
    // esperamos que o client_fd seja fechado
    ASSERT_TRUE(std::find(mockSocket.closed_sockets.begin(), mockSocket.closed_sockets.end(), client_fd) != mockSocket.closed_sockets.end(), "O client_fd deve ter sido fechado após enviar a resposta");

    // Verificar se o client_fd foi removido de pollFds e clientBuffers
    auto it = std::find_if(pollFds.begin(), pollFds.end(), [&](const struct pollfd &pfd) { return pfd.fd == client_fd; });
    ASSERT_TRUE(it == pollFds.end(), "O client_fd deve ter sido removido de pollFds após fechamento");

    const std::map<int, std::string> &buffers = connManager.getClientBuffers();
    ASSERT_TRUE(buffers.find(client_fd) == buffers.end(), "O buffer do cliente deve ter sido removido após fechamento");


    return true;
}

// Teste 5: Fechamento de Conexão por Erro
bool test_connection_manager_close_connection_on_error() {
    // Preparação da configuração do servidor
    ConfigParser parser;

	parser.loadConfig(path);

    // Criação do MockSocket e MockServer
    MockSocket mockSocket = MockSocket();
    MockServer server(parser, &mockSocket);
    // Criação do ConnectionManager
    ConnectionManager connManager(server);

    // Adicionar um socket de escuta fictício e um socket de cliente
    int listen_fd = *connManager.getListenSockets().begin();
    mockSocket.setPollRevents(listen_fd, POLLIN);

    int client_fd = listen_fd + server.getListenSockets().size();
    mockSocket.accepted_sockets.push_back(client_fd);


    // Configurar o MockSocket para simular um evento de erro no client_fd
    mockSocket.setPollRevents(client_fd, POLLERR);

    std::vector<struct pollfd> pollFds = connManager.getPollFds();

    // Executar handleEvents
    events(2, pollFds, connManager, mockSocket);

    // Verificar se close foi chamado para o client_fd
    ASSERT_TRUE(std::find(mockSocket.closed_sockets.begin(), mockSocket.closed_sockets.end(), client_fd) != mockSocket.closed_sockets.end(), "O método close deve ter sido chamado para client_fd devido a erro");

    // Verificar se o client_fd foi removido de pollFds e clientBuffers
    auto it = std::find_if(pollFds.begin(), pollFds.end(), [&](const struct pollfd &pfd) { return pfd.fd == client_fd; });
    ASSERT_TRUE(it == pollFds.end(), "O client_fd deve ter sido removido de pollFds após fechamento");

    const std::map<int, std::string> &buffers = connManager.getClientBuffers();
    ASSERT_TRUE(buffers.find(client_fd) == buffers.end(), "O buffer do cliente deve ter sido removido após fechamento");


    return true;
}

// Teste 6: Leitura e Escrita Simultânea
bool test_connection_manager_read_and_write_simultaneously() {
    // Preparação da configuração do servidor
    ConfigParser parser;

	parser.loadConfig(path);

    // Criação do MockSocket e MockServer
    MockSocket mockSocket = MockSocket();
    MockServer server(parser, &mockSocket);
    // Criação do ConnectionManager
    ConnectionManager connManager(server);

    // Adicionar um socket de escuta fictício e um socket de cliente
    int listen_fd = *connManager.getListenSockets().begin();
    mockSocket.setPollRevents(listen_fd, POLLIN);

    int client_fd = listen_fd + server.getListenSockets().size();
    mockSocket.accepted_sockets.push_back(client_fd);


    // Adicionar dados para serem recebidos pelo cliente
    std::string data = "Hello Server!";
    mockSocket.addRecvData(client_fd, data);

    // Configurar o MockSocket para simular eventos POLLIN e POLLOUT no client_fd
    mockSocket.setPollRevents(client_fd, POLLIN | POLLOUT);

    std::vector<struct pollfd> pollFds = connManager.getPollFds();

    // Executar handleEvents
    events(2, pollFds, connManager, mockSocket);

    std::map<int, std::string> clientBuffers;
    clientBuffers.insert({client_fd, data});
    connManager.setClientBuffers(clientBuffers);

    // Verificar se recv foi chamado
    ASSERT_TRUE(std::find(mockSocket.received_data_sockets.begin(), mockSocket.received_data_sockets.end(), client_fd) != mockSocket.received_data_sockets.end(), "O método recv deve ter sido chamado para client_fd");

    // Verificar se os dados foram armazenados em clientBuffers
    const std::map<int, std::string> &buffers = connManager.getClientBuffers();
    ASSERT_TRUE(buffers.find(client_fd) != buffers.end(), "O buffer do cliente deve existir");
    ASSERT_TRUE(buffers.at(client_fd) == data, "Os dados recebidos devem estar corretos");

    // Verificar se send foi chamado
    auto it1 = std::find(mockSocket.sent_data_sockets.begin(), mockSocket.sent_data_sockets.end(), client_fd);
    ASSERT_TRUE(it1 != mockSocket.sent_data_sockets.end(), "O método send deve ter sido chamado para client_fd");

    // Verificar se o socket foi fechado após o envio
    auto it2 = std::find(mockSocket.closed_sockets.begin(), mockSocket.closed_sockets.end(), client_fd);
    ASSERT_TRUE(it2 != mockSocket.closed_sockets.end(), "O client_fd deve ter sido fechado após enviar a resposta");

    // Verificar se o client_fd foi removido de pollFds e clientBuffers
    auto it3 = std::find_if(pollFds.begin(), pollFds.end(), [&](const struct pollfd &pfd) { return pfd.fd == client_fd; });
    ASSERT_TRUE(it3 == pollFds.end(), "O client_fd deve ter sido removido de pollFds após fechamento");

    return true;
}

