/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 10:52:05 by Everton           #+#    #+#             */
/*   Updated: 2024/10/31 18:16:03 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <set>
#include <string>
#include <cctype>
#include <vector>
#include "./http/HTTPResponse.hpp"

bool removeTrailingSemicolon(std::string &s, const std::string &key);
std::string& trim(std::string& s);
std::vector<std::string> split(const std::string& s, char delimiter);
bool isNumber(const std::string& s);
bool isValidIPv4(const std::string& ip);
std::string getContentType(const std::string& extension);
void serveStaticFile(const std::string& filePath, HTTPResponse& response);
std::string joinMethods(const std::set<std::string>& methods);