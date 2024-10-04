/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RouteConfig.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 19:07:32 by Everton           #+#    #+#             */
/*   Updated: 2024/10/03 20:50:32 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <set>
#include <string>

class RouteConfig {
	private:
		std::string root;
		std::string index;
		std::set<std::string> methods;

	public:
		RouteConfig() {}

		std::string getRoot() const {
			return root;
		};
		std::set<std::string> getMethods() const {
			return methods;
		};
		void addMethod(const std::string& method) {
			methods.insert(method);
		};
		void setRoot(const std::string& root) {
			this->root = root;
		};
		void setIndex(const std::string& index) {
			this->index = index;
		};
};