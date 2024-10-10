/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testConfig.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 20:16:41 by Everton           #+#    #+#             */
/*   Updated: 2024/10/10 20:18:47 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

bool testConfigValidConfigParsing();
bool testConfigMissingSemicolon();
bool testConfigUnmatchedBraces();
bool testConfigInvalidDirective();
bool testConfigMissingRequiredDirective();
bool testConfigEmptyConfigFile();
bool testConfigMultipleServerBlocks();
bool testConfigRouteConfigurations();
bool testConfigLargeConfigFile();
bool testConfigInvalidValue();