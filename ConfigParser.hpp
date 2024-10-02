/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 14:08:43 by Everton           #+#    #+#             */
/*   Updated: 2024/10/02 14:57:44 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <vector>
#include <sstream>


struct RouteConfig {
    std::string path;
    std::string methods;
    std::string upload_directory;
    std::string index;
};

struct ServerConfig {
    int listen;
    std::string server_name;
    std::string root;
    std::string error_page;
    std::vector<RouteConfig> routes;
};

class ConfigParser {
	private:
		std::string filename;
		void parseServerDirective(ServerConfig& server, const std::string& key, std::istringstream& iss);
		void parseLocationDirective(RouteConfig& route, const std::string& key, std::istringstream& iss);

	public:
		ConfigParser(const std::string& filename);
		bool parse(std::vector<ServerConfig>& servers);
		std::string getFilename() const;
};