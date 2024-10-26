/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 20:51:59 by Everton           #+#    #+#             */
/*   Updated: 2024/10/26 10:19:44 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "basic.hpp"
#include "testConfig.hpp"

void runConfigTests() {
    std::cout << "\n\nConfig tests" << std::endl;
    RUN_TEST(testConfigValidConfigParsing);
    RUN_TEST(testConfigMissingSemicolon);
    RUN_TEST(testConfigUnmatchedBraces);
    RUN_TEST(testConfigInvalidDirective);
    RUN_TEST(testConfigMissingRequiredDirective);
    RUN_TEST(testConfigEmptyConfigFile);
    RUN_TEST(testConfigMultipleServerBlocks);
    RUN_TEST(testConfigAutoindexOnOff);
    RUN_TEST(testConfigUploadEnable);
    RUN_TEST(testConfigCgiExtensions);
    RUN_TEST(testConfigReturnCodes);
    RUN_TEST(testConfigInvalidAutoindexValue);
    RUN_TEST(testConfigInvalidUploadEnableValue);
    RUN_TEST(testConfigMissingUploadPath);
    RUN_TEST(testConfigInvalidReturnCode);
    RUN_TEST(testConfigDuplicateServerNames);
    RUN_TEST(testConfigAllDirectives);
    RUN_TEST(testConfigInvalidHost);
    RUN_TEST(testConfigInvalidPort);
    RUN_TEST(testErrorPage);

    std::cout << "\n\nServer tests" << std::endl;
    RUN_TEST(testServerInitializationSuccess);
    RUN_TEST(testServerSocketCreationFailure);
    RUN_TEST(testServerSetsockoptFailure);
    RUN_TEST(testServerInetPtonFailure);
    RUN_TEST(testServerBindFailure);
    RUN_TEST(testServerListenFailure);
    RUN_TEST(testServerDestructorClosesSockets);
    RUN_TEST(testServerMultipleListenSockets);
    printSummary();
}

int main() {
    runConfigTests();
    return 0;
}