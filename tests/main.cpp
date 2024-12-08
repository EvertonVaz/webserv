/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 20:51:59 by Everton           #+#    #+#             */
/*   Updated: 2024/12/08 14:35:08 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./headers/basic.hpp"
#include "./headers/main.hpp"

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
}

void runSererTests() {
    std::cout << "\n\nServer tests" << std::endl;
    RUN_TEST(testServerInitializationSuccess);
    RUN_TEST(testServerSocketCreationFailure);
    RUN_TEST(testServerSetsockoptFailure);
    RUN_TEST(testServerInetPtonFailure);
    RUN_TEST(testServerBindFailure);
    RUN_TEST(testServerListenFailure);
    RUN_TEST(testServerDestructorClosesSockets);
    RUN_TEST(testServerMultipleListenSockets);
}

void runConnManagerTests() {
    std::cout << "\n\nConnectionManager tests" << std::endl;
    RUN_TEST(test_connection_manager_initialization);
    RUN_TEST(test_connection_manager_accept_new_connection);
    RUN_TEST(test_connection_manager_read_from_client);
    RUN_TEST(test_connection_manager_write_to_client);
    RUN_TEST(test_connection_manager_close_connection_on_error);
    RUN_TEST(test_connection_manager_read_and_write_simultaneously);
}

void runRouterTests() {
    std::cout << "\n\nRouter tests" << std::endl;
    RUN_TEST(test_route_request);
    RUN_TEST(test_handle_request_serve_static_file);
    RUN_TEST(test_handle_request_method_not_allowed);
    RUN_TEST(test_handle_request_redirect);
    RUN_TEST(test_handle_request_autoindex_enabled);
    RUN_TEST(test_handle_request_autoindex_disabled_no_index);
}

int runHTTPRequest() {
    RUN_TEST(testHTTPRequestParsingWithBody);
    RUN_TEST(testHTTPRequestParsingChunkedEncoding);
    RUN_TEST(testHTTPRequestParsingLargeRequest);
    RUN_TEST(testHTTPRequestParsingInvalidVersion);
    RUN_TEST(testHTTPRequestParsingUnsupportedMethod);
    return 0;
}

int runFilePath() {
    RUN_TEST(test_FilePath_Constructor_WithValidInputs);
    RUN_TEST(test_FilePath_IsFile_WithExistingFile);
    RUN_TEST(test_FilePath_IsDirectory_WithExistingDirectory);
    RUN_TEST(test_FilePath_PathSafety);
    RUN_TEST(test_FilePath_NonExistingFile);
    RUN_TEST(test_FilePath_CGIPathHandling);
    RUN_TEST(test_FilePath_AutoIndexEnabled);

    return 0;
}

int main() {
    runConfigTests();
    runSererTests();
    // runConnManagerTests();
    runRouterTests();
    runHTTPRequest();
    runFilePath();

    printSummary();
    return 0;
}