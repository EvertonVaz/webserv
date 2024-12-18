/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FilePath.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 11:07:26 by Everton           #+#    #+#             */
/*   Updated: 2024/12/07 15:04:45 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <list>
#include <string>
#include "../parser/RouteConfig.hpp"
#include "../http/HTTPRequest.hpp"

class FilePath {
    private:
        std::string _path;
        std::string _uri;
        std::string _root;
        HTTPRequest _request;
        RouteConfig _routeConfig;
        std::list<std::string> _index;
        bool _isCgiRoute;
        bool _isSafe;
        bool _isFile;
        bool _isDirectory;
        bool _autoindex;
        bool _canRead;
        bool _canWrite;
        bool _canExecute;

        void setIsDirectory(bool enabled);
        void setIsFile(bool isFile);

        bool isPathSafe(std::string path, std::string root);
        bool isPathExist(std::string path);
        std::string isCgi(std::string uri);
        std::string constructorSafeFilePath();

    public:
        FilePath(std::string root, HTTPRequest request, RouteConfig routeConfig);
        ~FilePath();

        bool getIsFile() const;
        bool getIsDirectory() const;
        bool getPathExist() const;
        bool getIsSafe() const;
        bool getCanRead() const;
        bool getCanWrite() const;
        bool getCanExecute() const;
        std::string getUri() const;
        std::string getPath() const;
        void setAutoIndex(bool autoindex);
};