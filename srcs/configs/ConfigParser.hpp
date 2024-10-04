/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 14:08:43 by Everton           #+#    #+#             */
/*   Updated: 2024/10/03 21:02:33 by Everton          ###   ########.fr       */
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

	public:
        ConfigParser();
		ConfigParser *loadConfig(const std::string& filename);
		ConfigParser *parse();
        std::vector<ServerConfig> getServers() const;
};