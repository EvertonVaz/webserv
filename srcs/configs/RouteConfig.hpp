/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RouteConfig.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 19:07:32 by Everton           #+#    #+#             */
/*   Updated: 2024/10/04 16:17:31 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <set>
#include <map>
#include <string>
#include <sstream>

class RouteConfig {
	private:
		std::string root;
		std::string index;
		std::string autoindex;
		std::string upload_path;
		std::string cgi_extension;
		std::string upload_enable;
		std::set<std::string> methods;
		std::set<std::string> allowed_methods;
		std::map<int, std::string> return_codes;

	public:
		RouteConfig() {}

		std::string getRoot() const {
			return root;
		};
		std::set<std::string> getMethods() const {
			return methods;
		};
		void addMethod(const std::string& method) {
			std::istringstream iss(method);
			while (iss) {
				std::string word;
				iss >> word;
				if (!word.empty())
					methods.insert(word);
			}
		};
		void setRoot(const std::string& root) {
			this->root = root;
		};
		void fillIndex(const std::string& index) {
			std::istringstream iss(index);
			while (iss) {
				std::string word;
				iss >> word;
				if (!word.empty())
					methods.insert(word);
			}
		};
		void fillAllowedMethods(const std::string& methods) {
			std::istringstream iss(methods);
			while (iss) {
				std::string word;
				iss >> word;
				if (!word.empty())
					allowed_methods.insert(word);
			}
		};
};