/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 20:51:59 by Everton           #+#    #+#             */
/*   Updated: 2024/10/04 11:13:12 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "basic.hpp"
bool testValidConfigParsing();
bool testMissingSemicolon();
bool testUnmatchedBraces();
bool testInvalidDirective();
bool testMissingRequiredDirective();
// bool testInvalidValue();
// bool testEmptyConfigFile();
// bool testMultipleServerBlocks();
// bool testRouteConfigurations();
// bool testLargeConfigFile();

int main() {
    RUN_TEST(testValidConfigParsing);
    RUN_TEST(testMissingSemicolon);
    RUN_TEST(testUnmatchedBraces);
    RUN_TEST(testInvalidDirective);
    RUN_TEST(testMissingRequiredDirective);
    // RUN_TEST(testInvalidValue);
    // RUN_TEST(testEmptyConfigFile);
    // RUN_TEST(testMultipleServerBlocks);
    // RUN_TEST(testRouteConfigurations);
    // RUN_TEST(testLargeConfigFile);

    return 0;
}
