/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RouteConfig.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 19:07:32 by Everton           #+#    #+#             */
/*   Updated: 2024/10/10 21:54:07 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <set>
#include <string>
#include <map>

class RouteConfig {
	private:
		std::string root;
		std::string autoindex;
		std::string upload_path;
		std::string cgi_extension;
		std::string upload_enable;
		std::set<std::string> index;
		std::set<std::string> methods;
		std::set<std::string> allowed_methods;
		std::pair<int, std::string> return_codes;

		void initializeDirectiveMap();
		struct DirectiveHandler {
			void (RouteConfig::*handler)(const std::string&);
		};
		std::map<std::string, DirectiveHandler> directiveMap;

	public:
		RouteConfig();

		std::string getRoot() const;
		std::set<std::string> getMethods() const;

		void addMethod(const std::string& method);
		void setRoot(const std::string& root);
		void setIndex(const std::string& index);
		void setAllowedMethods(const std::string& methods);
		void setAutoindex(const std::string& autoindex);
		void setUploadPath(const std::string& upload_path);
		void setCgiExtension(const std::string& cgi_extension);
		void setUploadEnable(const std::string& upload_enable);
		void setReturnCodes(const std::string& return_codes);

		void applyDirective(const std::string& key, const std::string& value);
};