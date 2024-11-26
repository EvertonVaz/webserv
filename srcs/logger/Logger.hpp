/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 06:28:58 by Everton           #+#    #+#             */
/*   Updated: 2024/11/25 21:28:43 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <fstream>
#include <string>

class Logger {
    public:
        enum LogLevel { INFO, WARNING, DEFAULT, ERROR = 400 };

    private:
        bool ArchiveEnabled;
        Logger(bool ArchiveEnabled = false);
        ~Logger();
        Logger(const Logger&);
        Logger& operator=(const Logger&);

        std::ofstream logFile;
        LogLevel currentLevel;
        std::string getLevelString(LogLevel level);
        std::string getMethodColor(const std::string& method);
        std::string getStatusColor(int status);

    public:
        static Logger& getInstance();
        void setLogFile(const std::string& filename);
        void log(LogLevel level, const std::string& message);
        void logRoute(const std::string& msg, const std::string& method, const std::string& route, int status);
};
