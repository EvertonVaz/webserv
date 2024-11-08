#pragma once

#include "../http/HTTPRequest.hpp"
#include "../http/HTTPResponse.hpp"
#include "../parser/RouteConfig.hpp"

class CGIHandler {
    private:
        HTTPRequest _request;
        RouteConfig _routeConfig;

        void setEnvironmentVariables(char**& envp);
        void executeCGI(int input_fd, int output_fd);
        std::string getScriptPath() const;

    public:
        CGIHandler(const HTTPRequest& request, const RouteConfig& routeConfig);
        ~CGIHandler();

        void handleResponse(HTTPResponse& response);
};
