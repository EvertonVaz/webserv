
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 10:00:00 by Everton           #+#    #+#             */
/*   Updated: 2024/11/10 10:00:00 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGIHandler.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <cstring>
#include <sstream>
#include <cerrno>

CGIHandler::CGIHandler(ErrorHandler& errorHandler, FilePath& filePath, const HTTPRequest& request)
    : errorHandler(errorHandler), filePath(filePath), request(request) {
}

CGIHandler::~CGIHandler() {}

void CGIHandler::handleResponse(HTTPResponse& response) {
    if (!filePath.getCanExecute()) {
        return errorHandler.handleError(403, response);
    }
    executeCGI(response);
}

void CGIHandler::handleExec(int *inputPipe, int *outputPipe) {
    char** envp = NULL;
    setEnvironment(envp);

    dup2(inputPipe[0], STDIN_FILENO);
    dup2(outputPipe[1], STDOUT_FILENO);

    close(inputPipe[1]);
    close(outputPipe[0]);
    std::string path = filePath.getPath();

    char* argv[] = { const_cast<char*>(path.c_str()), NULL };
    execve(argv[0], argv, envp);
    std::cerr << "Execve failed: " << strerror(errno) << std::endl;
    exit(1);
}

void CGIHandler::fillResponse(std::string cgiOutput, HTTPResponse& response) {
    size_t headerEnd = cgiOutput.find("\n\n");
    if (headerEnd != std::string::npos) {
        std::string headers = cgiOutput.substr(0, headerEnd);
        std::string body = cgiOutput.substr(headerEnd + 2);

        std::istringstream headerStream(headers);
        std::string line;
        while (std::getline(headerStream, line) && line != "\r") {
            size_t pos = line.find(": ");
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 2);
                response.addHeader(key, value);
            }
        }
        response.setBody(body);
        response.setStatusCode(200);
    }
}

void CGIHandler::handlePOST(int *inputPipe, int *outputPipe, const HTTPRequest& request) {
    close(inputPipe[0]);
    close(outputPipe[1]);
    if (request.getMethod() == "POST") {
        std::string body = request.getBody();
        write(inputPipe[1], body.c_str(), body.length());
    }
    close(inputPipe[1]);
}

std::string CGIHandler::readCGI(int *outputPipe) {
    char buffer[1024];
    std::string cgiOutput;
    ssize_t bytesRead;
    while ((bytesRead = read(outputPipe[0], buffer, sizeof(buffer))) > 0) {
        cgiOutput.append(buffer, bytesRead);
    }
    close(outputPipe[0]);

    return cgiOutput;
}

void CGIHandler::executeCGI(HTTPResponse& response) {
    int inputPipe[2];
    int outputPipe[2];

    if (pipe(inputPipe) == -1 || pipe(outputPipe) == -1) {
        return errorHandler.handleError(500, response);
    }

    pid_t pid = fork();
    if (pid == -1) {
        std::cout << "Fork failed" << std::endl;
        return errorHandler.handleError(500, response);
    } else if (pid == 0) {
        handleExec(inputPipe, outputPipe);
    } else {

        handlePOST(inputPipe, outputPipe, request);
        std::string cgiOutput = readCGI(outputPipe);

        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            fillResponse(cgiOutput, response);
        } else {
            return errorHandler.handleError(500, response);
        }
    }
}

void CGIHandler::setEnvironment(char**& envp) {
    envVarsStorage.clear();
    extern char **environ;
    for (char **env = environ; *env != NULL; ++env) {
        envVarsStorage.push_back(std::string(*env));
    }
    // Método da requisição (GET, POST, etc.)
    envVarsStorage.push_back("REQUEST_METHOD=" + request.getMethod());

    // Consulta da string (para métodos GET) - Descomentado se necessário
    envVarsStorage.push_back("QUERY_STRING=" + request.getQueryString());

    // Conteúdo da requisição (para métodos POST)
    if (request.getMethod() == "POST") {
        std::ostringstream oss;
        oss << request.getContentLength();
        envVarsStorage.push_back("CONTENT_LENGTH=" + oss.str());
        envVarsStorage.push_back("CONTENT_TYPE=" + request.getContentType());
    }

    // Informação do script
    envVarsStorage.push_back("SCRIPT_NAME=" + filePath.getPath());

    // Informação do servidor
    std::string host = request.getHeaders().at("host");
    size_t colonPos = host.find(":");
    if (colonPos != std::string::npos) {
        envVarsStorage.push_back("SERVER_NAME=" + host.substr(0, colonPos));
        envVarsStorage.push_back("SERVER_PORT=" + host.substr(colonPos + 1));
    } else {
        envVarsStorage.push_back("SERVER_NAME=" + host);
        envVarsStorage.push_back("SERVER_PORT=80"); // Porta padrão HTTP
    }

    // Variáveis adicionais conforme a necessidade
    envVarsStorage.push_back("GATEWAY_INTERFACE=CGI/1.1");
    envVarsStorage.push_back("SERVER_PROTOCOL=HTTP/1.1");
    envVarsStorage.push_back("REMOTE_ADDR=127.0.0.1");
    envVarsStorage.push_back("REMOTE_PORT=12345");

    // Converter as strings para C-strings
    std::vector<char*> envpVec;
    for (std::vector<std::string>::iterator it = envVarsStorage.begin(); it != envVarsStorage.end(); ++it) {
        envpVec.push_back(const_cast<char*>(it->c_str()));
    }
    envpVec.push_back(NULL); // Finalizar com NULL

    // Alocar memória para envp
    size_t envCount = envpVec.size();
    envp = new char*[envCount];
    for (size_t i = 0; i < envCount; ++i) {
        envp[i] = envpVec[i];
    }
}