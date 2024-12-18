/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RouteConfig.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 19:07:32 by Everton           #+#    #+#             */
/*   Updated: 2024/12/03 09:41:59 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <set>
#include <list>
#include <string>

class RouteConfig {
	private:
		std::string path;
		bool autoindex;
		std::string root;
		bool upload_enable;
		std::string upload_path;
		std::list<std::string> index;
		std::set<std::string> methods;
		std::set<std::string> cgi_extensions;
		std::set<std::string> allowed_methods;
		std::map<int, std::string> return_codes;

		void initializeDirectiveMap();
		struct DirectiveHandler {
			void (RouteConfig::*handler)(const std::string&);
		};
		std::map<std::string, DirectiveHandler> directiveMap;

	public:
		RouteConfig();
		RouteConfig(const RouteConfig& other);
		RouteConfig& operator=(const RouteConfig& other);
		~RouteConfig();

		bool getAutoindex() const;
		std::string getPath() const;
		std::string getRoot() const;
		bool getUploadEnable() const;
		std::string getUploadPath() const;
		std::list<std::string> getIndex() const;
		std::set<std::string> getMethods() const;
		std::set<std::string> getCgiExtensions() const;
		std::set<std::string> getAllowedMethods() const;
		std::map<int, std::string> getReturnCodes() const;

		void setPath(const std::string& path);
		void setMethods(const std::string& method);
		void setRoot(const std::string& root);
		void setIndex(const std::string& index);
		void setAllowedMethods(const std::string& methods);
		void setAutoindex(const std::string& autoindex);
		void setUploadPath(const std::string& upload_path);
		void setCgiExtension(const std::string& cgi_extensions);
		void setUploadEnable(const std::string& upload_enable);
		void setReturnCodes(const std::string& return_codes);

		void applyDirective(const std::string& key, const std::string& value);

		bool operator==(const RouteConfig& other) const;
		bool operator!=(const RouteConfig& other) const;
		bool operator<(const RouteConfig& other) const;
		bool operator>(const RouteConfig& other) const;
};