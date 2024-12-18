/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 10:52:05 by Everton           #+#    #+#             */
/*   Updated: 2024/11/27 14:31:14 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <set>
#include <string>
#include <cctype>
#include <vector>
#include "http/HTTPRequest.hpp"
#include "parser/ServerConfig.hpp"

bool removeTrailingSemicolon(std::string &s, const std::string &key);
std::string& trim(std::string& s);
std::vector<std::string> split(const std::string& s, char delimiter);
bool isNumber(const std::string& s);
bool isValidIPv4(const std::string& ip);
std::string getContentType(const std::string& extension);
std::string joinMethods(const std::set<std::string>& methods);
ServerConfig selectConfig(HTTPRequest request, std::vector<ServerConfig> serverConfigs);
const std::map<int, std::string> httpReasonPhrase();
std::string itostr(const int value);
std::string getProgram(const std::string& cgiExtension);
