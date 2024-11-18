/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 06:28:58 by Everton           #+#    #+#             */
/*   Updated: 2024/11/15 16:03:03 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <fstream>
#include <string>

class Logger {
    public:
        enum LogLevel { INFO, WARNING, ERROR, DEFAULT };

        static Logger& getInstance();
        void log(LogLevel level, const std::string& message);
        void setLogFile(const std::string& filename);

    private:
        bool ArchiveEnabled;
        Logger(bool ArchiveEnabled = false);
        ~Logger();
        Logger(const Logger&);
        Logger& operator=(const Logger&);

        std::ofstream logFile;
        LogLevel currentLevel;
        std::string getLevelString(LogLevel level);
        std::string swithColor(LogLevel level);
};
