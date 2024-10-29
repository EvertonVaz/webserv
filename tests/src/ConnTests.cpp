// ConnectionManagerTests.cpp
#include "../headers/basic.hpp"
#include "../../srcs/connection/ConnectionManager.hpp"
#include "../headers/MockSocket.hpp"
#include "../headers/MockServer.hpp"
#include "../../srcs/parser/ConfigParser.hpp" // Incluído o MockConfigParser
#include <algorithm>

std::string path = "/home/etovaz/nave/webserver/config.conf";

// Teste 1: Inicialização do ConnectionManager
bool test_connection_manager_initialization() {
    // Preparação da configuração do servidor
    ConfigParser parser;

	parser.loadConfig(path);
    MockSocket *mockSocket = new MockSocket();
    MockServer server(parser, mockSocket);

    // Adicionar um socket de escuta fictício
    int listen_fd = 100;
    server.getListenSockets().push_back(listen_fd);

    // Criação do ConnectionManager
    ConnectionManager connManager(server);

    // Verificar se os sockets de escuta foram adicionados corretamente
    const std::vector<struct pollfd> &pollFds = connManager.getPollFds();
    ASSERT_TRUE(pollFds.size() == 1, "Deve haver um socket de escuta inicializado");
    ASSERT_TRUE(pollFds[0].fd == listen_fd, "O fd de escuta deve ser 100");
    ASSERT_TRUE((pollFds[0].events & POLLIN), "O socket de escuta deve estar configurado para POLLIN");

    // Limpeza
    delete mockSocket;

    return true;
}

// Teste 2: Aceitação de Nova Conexão
bool test_connection_manager_accept_new_connection() {
    // Preparação da configuração do servidor
    ConfigParser parser;

	parser.loadConfig(path);

    // Criação do MockSocket e MockServer
    MockSocket *mockSocket = new MockSocket();
    MockServer server(parser, mockSocket);

    // Adicionar um socket de escuta fictício
    int listen_fd = 100;
    server.getListenSockets().push_back(listen_fd);

    // Criação do ConnectionManager
    ConnectionManager connManager(server);

    // Configurar o MockSocket para simular um evento POLLIN no listen_fd
    mockSocket->setPollRevents(listen_fd, POLLIN);

    // Configurar o MockSocket para retornar um client_fd fictício na próxima chamada de accept
    int client_fd = 101;
    // Simulamos que o método accept retorna client_fd
    // Para isso, garantimos que socket() seja chamado para criar o client_fd
    // Porém, na implementação atual do MockSocket, accept chama socket(), então
    // asseguramos que socket() retorne client_fd
    // Ajuste no método accept no MockSocket não é necessário pois já retorna um fd único

    // Executar handleEvents
    connManager.handleEvents();

    // Verificar se accept foi chamado e o client_fd foi adicionado a pollFds
    const std::vector<struct pollfd> &pollFds = connManager.getPollFds();
    ASSERT_TRUE(pollFds.size() == 2, "Deve haver dois fds após aceitar uma conexão");
    ASSERT_TRUE(pollFds[1].fd == client_fd, "O fd do cliente deve ser 101");
    ASSERT_TRUE((pollFds[1].events & POLLIN), "O socket do cliente deve estar configurado para POLLIN");

    // Verificar se o client_fd foi aceito corretamente
    ASSERT_TRUE(std::find(mockSocket->accepted_sockets.begin(), mockSocket->accepted_sockets.end(), client_fd) != mockSocket->accepted_sockets.end(), "O método accept deve ter sido chamado e retornar client_fd");

    // Limpeza
    delete mockSocket;

    return true;
}

// Teste 3: Leitura de Dados de um Cliente
bool test_connection_manager_read_from_client() {
    // Preparação da configuração do servidor
    ConfigParser parser;

	parser.loadConfig(path);

    // Criação do MockSocket e MockServer
    MockSocket *mockSocket = new MockSocket();
    MockServer server(parser, mockSocket);

    // Adicionar um socket de escuta fictício e um socket de cliente
    int listen_fd = 100;
    int client_fd = 101;
    server.getListenSockets().push_back(listen_fd);
    server.getListenSockets().push_back(client_fd); // Supondo que client_fd já foi aceito

    // Criação do ConnectionManager
    ConnectionManager connManager(server);

    // Adicionar dados para serem recebidos pelo cliente
    std::string data = "Hello Server!";
    mockSocket->addRecvData(client_fd, data);

    // Configurar o MockSocket para simular um evento POLLIN no client_fd
    mockSocket->setPollRevents(client_fd, POLLIN);

    // Executar handleEvents
    connManager.handleEvents();

    // Verificar se recv foi chamado
    ASSERT_TRUE(std::find(mockSocket->received_data_sockets.begin(), mockSocket->received_data_sockets.end(), client_fd) != mockSocket->received_data_sockets.end(), "O método recv deve ter sido chamado para client_fd");

    // Verificar se os dados foram armazenados em clientBuffers
    const std::map<int, std::string> &buffers = connManager.getClientBuffers();
    ASSERT_TRUE(buffers.find(client_fd) != buffers.end(), "O buffer do cliente deve existir");
    ASSERT_TRUE(buffers.at(client_fd) == data, "Os dados recebidos devem estar corretos");

    // Verificar se POLLOUT foi adicionado ao events do cliente
    const std::vector<struct pollfd> &pollFds = connManager.getPollFds();
    auto it = std::find_if(pollFds.begin(), pollFds.end(), [&](const struct pollfd &pfd) { return pfd.fd == client_fd; });
    ASSERT_TRUE(it != pollFds.end(), "O client_fd deve estar presente em pollFds");
    ASSERT_TRUE((it->events & POLLOUT), "O evento POLLOUT deve estar configurado para o client_fd após leitura");

    // Limpeza
    delete mockSocket;

    return true;
}

// Teste 4: Escrita de Dados para um Cliente
bool test_connection_manager_write_to_client() {
    // Preparação da configuração do servidor
    ConfigParser parser;

	parser.loadConfig(path);

    // Criação do MockSocket e MockServer
    MockSocket *mockSocket = new MockSocket();
    MockServer server(parser, mockSocket);

    // Adicionar um socket de escuta fictício e um socket de cliente
    int listen_fd = 100;
    int client_fd = 101;
    server.getListenSockets().push_back(listen_fd);
    server.getListenSockets().push_back(client_fd); // Supondo que client_fd já foi aceito

    // Criação do ConnectionManager
    ConnectionManager connManager(server);

    // Adicionar dados no buffer do cliente para serem enviados
    // No teste anterior, após leitura, o evento POLLOUT é adicionado
    // Assim, simularemos que há dados a serem enviados
    connManager.getClientBuffers().insert({client_fd, "Hello Client!"});

    // Configurar o MockSocket para simular um evento POLLOUT no client_fd
    mockSocket->setPollRevents(client_fd, POLLOUT);

    // Executar handleEvents
    connManager.handleEvents();

    // Verificar se send foi chamado
    ASSERT_TRUE(std::find(mockSocket->sent_data_sockets.begin(), mockSocket->sent_data_sockets.end(), client_fd) != mockSocket->sent_data_sockets.end(), "O método send deve ter sido chamado para client_fd");

    // Verificar se os dados foram enviados corretamente
    // Como a implementação atual de writeToClient envia uma resposta HTTP e fecha a conexão,
    // esperamos que o client_fd seja fechado
    ASSERT_TRUE(std::find(mockSocket->closed_sockets.begin(), mockSocket->closed_sockets.end(), client_fd) != mockSocket->closed_sockets.end(), "O client_fd deve ter sido fechado após enviar a resposta");

    // Verificar se o client_fd foi removido de pollFds e clientBuffers
    const std::vector<struct pollfd> &pollFds = connManager.getPollFds();
    auto it = std::find_if(pollFds.begin(), pollFds.end(), [&](const struct pollfd &pfd) { return pfd.fd == client_fd; });
    ASSERT_TRUE(it == pollFds.end(), "O client_fd deve ter sido removido de pollFds após fechamento");

    const std::map<int, std::string> &buffers = connManager.getClientBuffers();
    ASSERT_TRUE(buffers.find(client_fd) == buffers.end(), "O buffer do cliente deve ter sido removido após fechamento");

    // Limpeza
    delete mockSocket;

    return true;
}

// Teste 5: Fechamento de Conexão por Erro
bool test_connection_manager_close_connection_on_error() {
    // Preparação da configuração do servidor
    ConfigParser parser;

	parser.loadConfig(path);

    // Criação do MockSocket e MockServer
    MockSocket *mockSocket = new MockSocket();
    MockServer server(parser, mockSocket);

    // Adicionar um socket de escuta fictício e um socket de cliente
    int listen_fd = 100;
    int client_fd = 101;
    server.getListenSockets().push_back(listen_fd);
    server.getListenSockets().push_back(client_fd); // Supondo que client_fd já foi aceito

    // Criação do ConnectionManager
    ConnectionManager connManager(server);

    // Configurar o MockSocket para simular um evento de erro no client_fd
    mockSocket->setPollRevents(client_fd, POLLERR);

    // Executar handleEvents
    connManager.handleEvents();

    // Verificar se close foi chamado para o client_fd
    ASSERT_TRUE(std::find(mockSocket->closed_sockets.begin(), mockSocket->closed_sockets.end(), client_fd) != mockSocket->closed_sockets.end(), "O método close deve ter sido chamado para client_fd devido a erro");

    // Verificar se o client_fd foi removido de pollFds e clientBuffers
    const std::vector<struct pollfd> &pollFds = connManager.getPollFds();
    auto it = std::find_if(pollFds.begin(), pollFds.end(), [&](const struct pollfd &pfd) { return pfd.fd == client_fd; });
    ASSERT_TRUE(it == pollFds.end(), "O client_fd deve ter sido removido de pollFds após fechamento");

    const std::map<int, std::string> &buffers = connManager.getClientBuffers();
    ASSERT_TRUE(buffers.find(client_fd) == buffers.end(), "O buffer do cliente deve ter sido removido após fechamento");

    // Limpeza
    delete mockSocket;

    return true;
}

// Teste 6: Leitura e Escrita Simultânea
bool test_connection_manager_read_and_write_simultaneously() {
    // Preparação da configuração do servidor
    ConfigParser parser;

	parser.loadConfig(path);

    // Criação do MockSocket e MockServer
    MockSocket *mockSocket = new MockSocket();
    MockServer server(parser, mockSocket);

    // Adicionar um socket de escuta fictício e um socket de cliente
    int listen_fd = 100;
    int client_fd = 101;
    server.getListenSockets().push_back(listen_fd);
    server.getListenSockets().push_back(client_fd); // Supondo que client_fd já foi aceito

    // Criação do ConnectionManager
    ConnectionManager connManager(server);

    // Adicionar dados para serem recebidos pelo cliente
    std::string data = "Hello Server!";
    mockSocket->addRecvData(client_fd, data);

    // Configurar o MockSocket para simular eventos POLLIN e POLLOUT no client_fd
    mockSocket->setPollRevents(client_fd, POLLIN | POLLOUT);

    // Executar handleEvents
    connManager.handleEvents();

    // Verificar se recv foi chamado
    ASSERT_TRUE(std::find(mockSocket->received_data_sockets.begin(), mockSocket->received_data_sockets.end(), client_fd) != mockSocket->received_data_sockets.end(), "O método recv deve ter sido chamado para client_fd");

    // Verificar se os dados foram armazenados em clientBuffers
    const std::map<int, std::string> &buffers = connManager.getClientBuffers();
    ASSERT_TRUE(buffers.find(client_fd) != buffers.end(), "O buffer do cliente deve existir");
    ASSERT_TRUE(buffers.at(client_fd) == data, "Os dados recebidos devem estar corretos");

    // Verificar se send foi chamado
    ASSERT_TRUE(std::find(mockSocket->sent_data_sockets.begin(), mockSocket->sent_data_sockets.end(), client_fd) != mockSocket->sent_data_sockets.end(), "O método send deve ter sido chamado para client_fd");

    // Verificar se o socket foi fechado após o envio
    ASSERT_TRUE(std::find(mockSocket->closed_sockets.begin(), mockSocket->closed_sockets.end(), client_fd) != mockSocket->closed_sockets.end(), "O client_fd deve ter sido fechado após enviar a resposta");

    // Verificar se o client_fd foi removido de pollFds e clientBuffers
    const std::vector<struct pollfd> &pollFds = connManager.getPollFds();
    auto it = std::find_if(pollFds.begin(), pollFds.end(), [&](const struct pollfd &pfd) { return pfd.fd == client_fd; });
    ASSERT_TRUE(it == pollFds.end(), "O client_fd deve ter sido removido de pollFds após fechamento");

    const std::map<int, std::string> &buffers_after = connManager.getClientBuffers();
    ASSERT_TRUE(buffers_after.find(client_fd) == buffers_after.end(), "O buffer do cliente deve ter sido removido após fechamento");

    // Limpeza
    delete mockSocket;

    return true;
}

