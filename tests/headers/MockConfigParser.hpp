/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MockConfigParser.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 19:16:16 by Everton           #+#    #+#             */
/*   Updated: 2024/11/05 19:18:08 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../../srcs/parser/ConfigParser.hpp"


class MockConfigParser : public ConfigParser {
    public:
        std::vector<ServerConfig> getServers() const {
            return servers;
        }

        void addServer(const ServerConfig &config) {
            servers.push_back(config);
        }

    private:
        std::vector<ServerConfig> servers;
};