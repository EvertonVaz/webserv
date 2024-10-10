/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 20:51:59 by Everton           #+#    #+#             */
/*   Updated: 2024/10/10 20:19:18 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "basic.hpp"
#include "testConfig.hpp"

int main() {
    RUN_TEST(testConfigValidConfigParsing);
    RUN_TEST(testConfigMissingSemicolon);
    RUN_TEST(testConfigUnmatchedBraces);
    RUN_TEST(testConfigInvalidDirective);
    RUN_TEST(testConfigMissingRequiredDirective);
    RUN_TEST(testConfigEmptyConfigFile);
    RUN_TEST(testConfigMultipleServerBlocks);
    // RUN_TEST(testConfigRouteConfigurations);
    // RUN_TEST(testConfigLargeConfigFile);
    // RUN_TEST(testConfigInvalidValue);
    printSummary();
    return 0;
}
