/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 06:29:15 by Everton           #+#    #+#             */
/*   Updated: 2024/11/27 11:05:41 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <ctime>
#include <sstream>
#include <iostream>
#include "Logger.hpp"
#include "../aux.hpp"

Logger::Logger(bool archiveEnabled) :
    ArchiveEnabled(archiveEnabled), currentLevel(INFO) {}

Logger::~Logger() {
    if (logFile.is_open() && ArchiveEnabled) {
        logFile.close();
    }
}

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

void Logger::setLogFile(const std::string& filename) {
    if (logFile.is_open()) {
        logFile.close();
    }
    logFile.open(filename.c_str(), std::ios_base::app);
    ArchiveEnabled = true;
}

void Logger::log(LogLevel level, const std::string& msg) {
    std::time_t now = std::time(0);
    std::tm* local_tm = std::localtime(&now);
    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", local_tm);
    std::string dt_str(buffer);
    std::ostringstream logStream;
    logStream << "[" << dt_str << "] "
              << getLevelString(level) << ": " << msg;
    if (ArchiveEnabled && logFile.is_open()) {
        logFile << logStream.str() << std::endl;
    } else {
        std::cout << logStream.str() << std::endl;
    }
}

std::string pad(std::string str, size_t len) {
    if (str.length() < len) {
        str.append(len - str.length(), ' ');
    }
    return str;
}

void Logger::logRoute(const std::string& msg, const std::string& method, const std::string& route, int status) {
    std::time_t now = std::time(0);
    std::tm* local_tm = std::localtime(&now);
    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", local_tm);
    std::string dt_str(buffer);
    std::ostringstream logStream;
    logStream << "[" << dt_str << "] "
              << getLevelString(INFO) << ": " << msg << " | "
              << pad(getMethodColor(method), 21) << " | "
              << getStatusColor(status) << " | " << route;
    if (ArchiveEnabled && logFile.is_open()) {
        logFile << logStream.str() << std::endl;
    } else {
        std::cout << logStream.str() << std::endl;
    }
}

std::string Logger::getMethodColor(const std::string& method) {
    if (method == "GET") {
        return "\033[48;5;45;30mGET\033[0m";
    } else if (method == "POST") {
        return "\033[48;5;82;30mPOST\033[0m";
    } else if (method == "DELETE") {
        return "\033[48;5;196;30mDELETE\033[0m";
    } else {
        return "\033[48;5;226;30m" + method + "\033[0m";
    }
}

std::string Logger::getStatusColor(int status) {
    if (status >= 200 && status < 300) {
        return "\033[48;5;82;30m" + itostr(status) + "\033[0m";
    } else if (status >= 300 && status < 400) {
        return "\033[48;5;226;30m" + itostr(status) + "\033[0m";
    } else if (status >= 400 && status < 600) {
        return "\033[48;5;196;38;5;15m"  + itostr(status) + "\033[0m";
    } else {
        return itostr(status);
    }
}

std::string Logger::getLevelString(LogLevel level) {

    if (level == INFO) return "\033[48;5;153;30mINFO\033[0m";
    if (level == WARNING) return "\033[48;5;226;30mWARNING\033[0m";
    if (level >= ERROR) return "\033[48;5;196;30mERROR\033[0m";
    return "DEFAULT";
}