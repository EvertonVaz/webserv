/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FilePath.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 11:07:26 by Everton           #+#    #+#             */
/*   Updated: 2024/11/08 12:16:41 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <set>
#include <string>

class FilePath {
    private:
        std::string _path;
        std::string _root;
        std::string _uri;
        std::set<std::string> _index;
        bool _isSafe;
        bool _isFile;
        bool _isDirectory;

        void setIsDirectory(bool enabled);
        void setIsFile(bool isFile);

        bool isPathSafe(std::string path, std::string root);
        bool isPathExist(std::string path);
        std::string constructorSafeFilePath();

    public:
        FilePath(std::string root, std::string uri, std::set<std::string> index);
        ~FilePath();

        bool getIsFile();
        bool getIsDirectory();
        bool getPathExist();
        bool getIsSafe();
        std::string getUri();
        std::string getPath();
};