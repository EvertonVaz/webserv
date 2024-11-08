#include "CGIHandler.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <cstring>

CGIHandler::CGIHandler(const HTTPRequest& request, const RouteConfig& routeConfig)
    : _request(request), _routeConfig(routeConfig) {}

CGIHandler::~CGIHandler() {}

void CGIHandler::handleResponse(HTTPResponse& response) {
    (void)response;
    int inputPipe[2];
    int outputPipe[2];
    if (pipe(inputPipe) == -1 || pipe(outputPipe) == -1) {
        // TODO: Tratar erro
        return;
    }

    pid_t pid = fork();
    if (pid == -1) {
        // TODO: Tratar erro no fork
        return;
    }

    if (pid == 0) {
        dup2(inputPipe[0], STDIN_FILENO);
        dup2(outputPipe[1], STDOUT_FILENO);
        close(inputPipe[1]);
        close(outputPipe[0]);

        char** envp = NULL;
        setEnvironmentVariables(envp);
        std::string scriptPath = getScriptPath();
        char* argv[] = { const_cast<char*>(scriptPath.c_str()), NULL };
        execve(argv[0], argv, envp);

        _exit(1);
    } else {
        close(inputPipe[0]);
        close(outputPipe[1]);

        // Enviar dados para o script CGI (se necessário)
        // ...código para escrever no inputPipe[1] se for método POST...

        close(inputPipe[1]);

        // Ler a saída do script CGI
        char buffer[1024];
        ssize_t bytesRead;
        std::string cgiOutput;
        while ((bytesRead = read(outputPipe[0], buffer, sizeof(buffer))) > 0) {
            cgiOutput.append(buffer, bytesRead);
        }

        // Processar a saída do CGI e preencher a resposta
        // ...código para atualizar response com cgiOutput...

        close(outputPipe[0]);
        waitpid(pid, NULL, 0);
    }
}

void CGIHandler::setEnvironmentVariables(char**& envp) {
    (void)envp;
    // Configurar as variáveis de ambiente necessárias para o script CGI
    // Exemplo: REQUEST_METHOD, QUERY_STRING, CONTENT_LENGTH, CONTENT_TYPE, etc.
}

std::string CGIHandler::getScriptPath() const {
    // Retorna o caminho completo para o script CGI baseado na URI e root
    // Exemplo: combinar _routeConfig.getRoot() com _request.getURI()
    return "";
}

