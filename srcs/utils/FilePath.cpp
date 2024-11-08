/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FilePath.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 11:15:05 by Everton           #+#    #+#             */
/*   Updated: 2024/11/08 12:18:10 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FilePath.hpp"
#include <linux/limits.h>
#include <sys/stat.h>
#include <cstdlib>

FilePath::FilePath(std::string root, std::string uri, std::set<std::string> index)
    : _root(root), _uri(uri), _index(index) {
    _isFile = false;
    _isDirectory = false;

    if (_uri[_uri.length() - 1] != '/')
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
    struct stat statBuf;
    if (stat(path.c_str(), &statBuf) == -1) {
        return false;
    }

    if (S_ISDIR(statBuf.st_mode)) {
        setIsDirectory(true);
    } else if (!S_ISREG(statBuf.st_mode)) {
        setIsFile(true);
    }

    return _isFile || _isDirectory;
}

std::string FilePath::constructorSafeFilePath() {
    std::string filePath = _root + _uri;

    std::set<std::string>::iterator it = _index.begin();
    for (; it != _index.end(); ++it) {
        filePath += *it;
        if (isPathExist(filePath)) {
            break;
        }
    }

    if (filePath[filePath.length() - 1] == '/')
        filePath.erase(filePath.length() - 1);

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