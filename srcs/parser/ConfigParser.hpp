/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 14:08:43 by Everton           #+#    #+#             */
/*   Updated: 2024/11/09 14:25:10 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <sstream>
#include <vector>
#include "ServerConfig.hpp"

class ConfigParser {
	private:
		std::string filename;
		std::stringstream file;
		RouteConfig currentRoute;
		ServerConfig currentServer;
        std::vector<ServerConfig> servers;
		void parseServerDirective(ServerConfig& server, const std::string& key, std::istringstream& iss);
		void parseLocationDirective(RouteConfig& route, const std::string& key, std::istringstream& iss);

		enum ParserState {
			STATE_GLOBAL,
			STATE_SERVER,
			STATE_LOCATION
		};
		ParserState state;
		std::string route_name;

		struct KeywordHandler {
			void (ConfigParser::*handler)(std::istringstream& iss);
		};

		std::map<std::string, KeywordHandler> globalKeywords;
		void initializeGlobalKeywords();

	public:
        ConfigParser();
		~ConfigParser();
		ConfigParser *loadConfig(const std::string& filename);
		ConfigParser *parse();
        virtual std::vector<ServerConfig> getServers() const;

		void setServer(ServerConfig server);
		void handleServer(std::istringstream& iss);
		void handleLocation(std::istringstream& iss);
		void handleClosingBrace(std::istringstream& iss);
		void handleDirective(std::istringstream& iss, const std::string& key);
};
