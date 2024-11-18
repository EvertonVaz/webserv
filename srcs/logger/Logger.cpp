/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 06:29:15 by Everton           #+#    #+#             */
/*   Updated: 2024/11/17 21:44:57 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Logger.hpp"
#include <iostream>
#include <ctime>
#include <sstream>

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
    logStream << swithColor(level) << "[" << dt_str << "] "
              << getLevelString(level) << ": " << msg
              <<  swithColor(Logger::DEFAULT);
    if (ArchiveEnabled && logFile.is_open()) {
        logFile << logStream.str() << std::endl;
    } else {
        std::cout << logStream.str() << std::endl;
    }
}

std::string Logger::swithColor(LogLevel level) {
    switch (level) {
        case INFO:    return "\033[0;32m";
        case WARNING: return "\033[0;33m";
        case ERROR:   return "\033[0;31m";
        case DEFAULT: return "\033[0m";
        default:      return "";
    }
}

std::string Logger::getLevelString(LogLevel level) {
    switch (level) {
        case INFO:    return "INFO";
        case WARNING: return "WARNING";
        case ERROR:   return "ERROR";
        default:      return "";
    }
}