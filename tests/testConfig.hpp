/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testConfig.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 20:16:41 by Everton           #+#    #+#             */
/*   Updated: 2024/10/11 14:10:58 by Everton          ###   ########.fr       */
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
bool testConfigAutoindexOnOff();
bool testConfigUploadEnable();
bool testConfigCgiExtensions();
bool testConfigReturnCodes();
bool testConfigInvalidAutoindexValue();
bool testConfigInvalidUploadEnableValue();
bool testConfigMissingUploadPath();
bool testConfigInvalidReturnCode();
bool testConfigDuplicateServerNames();
bool testConfigAllDirectives();
bool testConfigInvalidHost();
bool testConfigInvalidPort();
bool testConfigMissingSemicolon();