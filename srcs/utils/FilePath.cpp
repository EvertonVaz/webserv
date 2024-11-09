/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FilePath.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 11:15:05 by Everton           #+#    #+#             */
/*   Updated: 2024/11/09 14:51:42 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FilePath.hpp"
#include <linux/limits.h>
#include <sys/stat.h>
#include <cstdlib>

FilePath::FilePath(std::string root, std::string uri, std::list<std::string> index, bool autoindex)
    : _root(root), _uri(uri), _index(index), _autoindex(autoindex) {
    _isFile = false;
    _isDirectory = false;

    if (_root[_root.length() - 1] == '/')
        _root.erase(_root.length() - 1);
    isPathExist(_root + uri);
    if (_isDirectory && uri[uri.length() - 1] != '/')
        _uri += "/";
    _path = constructorSafeFilePath();
}

FilePath::~FilePath() {}

void FilePath::setIsDirectory(bool enabled) {
    _isDirectory = enabled;
}

void FilePath::setIsFile(bool enabled) {
    _isFile = enabled;
}

std::string FilePath::getPath() {
    return _path;
}

bool FilePath::isPathSafe(std::string path, std::string root) {
    char resolvedRoot[PATH_MAX];
    char resolvedPath[PATH_MAX];

    realpath(root.c_str(), resolvedRoot);
    realpath(path.c_str(), resolvedPath);

    std::string rootStr(resolvedRoot);
    std::string pathStr(resolvedPath);

    _isSafe = pathStr.compare(0, rootStr.length(), rootStr) == 0;
    return _isSafe;
}

bool FilePath::isPathExist(std::string path) {
    if (path[path.length() - 1] == '/')
        path.erase(path.length() - 1);

    struct stat statBuf;
    if (stat(path.c_str(), &statBuf) == -1) {
        return false;
    }

    if (S_ISDIR(statBuf.st_mode)) {
        setIsDirectory(true);
    } else if (S_ISREG(statBuf.st_mode)) {
        setIsFile(true);
        setIsDirectory(false);
    }

    _canRead = (statBuf.st_mode & S_IRUSR) != 0;
    _canWrite = (statBuf.st_mode & S_IWUSR) != 0;
    _canExecute = (statBuf.st_mode & S_IXUSR) != 0;
    return _isFile || _isDirectory;
}

std::string FilePath::constructorSafeFilePath() {
    std::string filePath = _root + _uri;

    if (!_autoindex && _isDirectory) {
        std::list<std::string>::iterator it = _index.begin();
        for (; it != _index.end(); ++it) {
            filePath += *it;
            if (isPathExist(filePath))
                break;
        }
    }

    if (filePath[filePath.length() - 1] == '/')
        filePath.erase(filePath.length() - 1);
    isPathExist(filePath);
    if (!isPathSafe(filePath, _root)) {
        setIsFile(false);
        setIsDirectory(false);
        return "";
    }
    return filePath;
}

bool FilePath::getIsFile() {
    return _isFile;
}

bool FilePath::getIsDirectory() {
    return _isDirectory;
}

bool FilePath::getPathExist() {
    return _isFile || _isDirectory;
}

bool FilePath::getIsSafe() {
    return _isSafe;
}

std::string FilePath::getUri() {
    return _uri;
}

void FilePath::setAutoIndex(bool autoindex) {
    _autoindex = autoindex;
}

bool FilePath::getCanRead() {
    return _canRead;
}

bool FilePath::getCanWrite() {
    return _canWrite;
}

bool FilePath::getCanExecute() {
    return _canExecute;
}
