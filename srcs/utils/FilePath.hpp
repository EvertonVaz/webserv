/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FilePath.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 11:07:26 by Everton           #+#    #+#             */
/*   Updated: 2024/11/09 10:25:44 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <list>
#include <string>

class FilePath {
    private:
        std::string _path;
        std::string _root;
        std::string _uri;
        std::list<std::string> _index;
        bool _isSafe;
        bool _isFile;
        bool _isDirectory;
        bool _autoindex;

        void setIsDirectory(bool enabled);
        void setIsFile(bool isFile);

        bool isPathSafe(std::string path, std::string root);
        bool isPathExist(std::string path);
        std::string constructorSafeFilePath();

    public:
        FilePath(std::string root, std::string uri, std::list<std::string> index, bool autoindex);
        ~FilePath();

        bool getIsFile();
        bool getIsDirectory();
        bool getPathExist();
        bool getIsSafe();
        std::string getUri();
        std::string getPath();
        void setAutoIndex(bool autoindex);
};