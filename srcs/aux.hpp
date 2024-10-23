/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 10:52:05 by Everton           #+#    #+#             */
/*   Updated: 2024/10/23 10:53:29 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <cctype>
#include <vector>

bool removeTrailingSemicolon(std::string &s, const std::string &key);
std::string& trim(std::string& s);
std::vector<std::string> split(const std::string& s, char delimiter);
bool isNumber(const std::string& s);
bool isValidIPv4(const std::string& ip);