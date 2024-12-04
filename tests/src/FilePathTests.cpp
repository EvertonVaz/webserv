/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FilePathTests.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:10:52 by Everton           #+#    #+#             */
/*   Updated: 2024/12/03 17:44:39 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/basic.hpp"
#include "../headers/MockConfigParser.hpp"
#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include "../../srcs/utils/FilePath.hpp"

// Helper function to create mock environment
void createMockEnvironment(HTTPRequest& mockRequest, RouteConfig& mockRouteConfig) {
    MockConfigParser parser;
    MockConfigParser* p = parser.createDefaultParser();
    mockRequest.setUri("/test/path");
    mockRouteConfig = p->getServers()[0].getRoutes()["/"];
}

// Test functions
bool test_FilePath_Constructor_WithValidInputs() {
    HTTPRequest mockRequest;
    RouteConfig mockRouteConfig;
    createMockEnvironment(mockRequest, mockRouteConfig);

    std::string root = "/var/www";

    FilePath filePath(root, mockRequest, mockRouteConfig);

    ASSERT_TRUE(filePath.getIsSafe(), "File path should be safe");
    ASSERT_TRUE(filePath.getUri() == "/test/path/", "URI should be '/test/path'");
    return true;
}

bool test_FilePath_IsFile_WithExistingFile() {
    // Create a mock request to a known existing file
    HTTPRequest mockRequest;
    mockRequest.setUri("/testfile.txt");

    RouteConfig mockRouteConfig;

    mockRouteConfig.setPath("/cgi-bin/");

    std::string root = "."; // current directory

    // Ensure that testfile.txt exists in the current directory
    FILE* file = fopen("testfile.txt", "w");
    if (file) {
        fputs("Test file content", file);
        fclose(file);
    } else {
        std::cerr << "Failed to create test file." << std::endl;
        return false;
    }

    FilePath filePath(root, mockRequest, mockRouteConfig);

    ASSERT_TRUE(filePath.getIsFile(), "Should detect as a file");
    ASSERT_TRUE(!filePath.getIsDirectory(), "Should not detect as a directory");
    ASSERT_TRUE(filePath.getPathExist(), "Path should exist");
    ASSERT_TRUE(filePath.getCanRead(), "Should have read permission");

    // Clean up
    remove("testfile.txt");

    return true;
}

bool test_FilePath_IsDirectory_WithExistingDirectory() {
    // Create a mock request to a known existing directory
    HTTPRequest mockRequest;
    mockRequest.setUri("/testdir/");

    RouteConfig mockRouteConfig;

    mockRouteConfig.setPath("/cgi-bin/");

    std::string root = "."; // current directory

    // Ensure that testdir exists in the current directory
    int result = mkdir("testdir", 0755);
    if (result != 0 && errno != EEXIST) {
        std::cerr << "Failed to create test directory." << std::endl;
        return false;
    }

    FilePath filePath(root, mockRequest, mockRouteConfig);

    ASSERT_TRUE(!filePath.getIsFile(), "Should not detect as a file");
    ASSERT_TRUE(filePath.getIsDirectory(), "Should detect as a directory");
    ASSERT_TRUE(filePath.getPathExist(), "Path should exist");
    ASSERT_TRUE(filePath.getCanRead(), "Should have read permission");

    // Clean up
    rmdir("testdir");

    return true;
}

bool test_FilePath_PathSafety() {
    HTTPRequest mockRequest;
    RouteConfig mockRouteConfig;
    createMockEnvironment(mockRequest, mockRouteConfig);
    mockRequest.setUri("/../../etc/passwd");

    std::string root = "/var/www";

    FilePath filePath(root, mockRequest, mockRouteConfig);

    ASSERT_TRUE(!filePath.getIsSafe(), "File path should not be safe");
    ASSERT_TRUE(!filePath.getPathExist(), "Path should not exist");
    return true;
}

bool test_FilePath_NonExistingFile() {
    HTTPRequest mockRequest;

    RouteConfig mockRouteConfig;
    createMockEnvironment(mockRequest, mockRouteConfig);
    mockRequest.setUri("/nonexistentfile.txt");

    std::string root = "."; // current directory

    FilePath filePath(root, mockRequest, mockRouteConfig);

    ASSERT_TRUE(!filePath.getIsFile(), "Should not detect as a file");
    ASSERT_TRUE(!filePath.getIsDirectory(), "Should not detect as a directory");
    ASSERT_TRUE(!filePath.getPathExist(), "Path should not exist");

    return true;
}

bool test_FilePath_CGIPathHandling() {
    HTTPRequest mockRequest;
    RouteConfig mockRouteConfig;
    createMockEnvironment(mockRequest, mockRouteConfig);
    
    mockRequest.setUri("/script.cgi");
    mockRouteConfig.setPath("/cgi-bin/");

    std::string root = "."; // current directory

    // Ensure that /cgi-bin/script.cgi exists
    mkdir("cgi-bin", 0755);
    FILE* file = fopen("cgi-bin/script.cgi", "w");
    if (file) {
        fputs("#!/bin/bash\necho 'Content-Type: text/plain'\necho\necho 'Hello, World!'", file);
        fclose(file);
        chmod("cgi-bin/script.cgi", 0755);
    } else {
        std::cerr << "Failed to create CGI script." << std::endl;
        return false;
    }

    FilePath filePath(root, mockRequest, mockRouteConfig);

    ASSERT_TRUE(filePath.getIsFile(), "Should detect as a file");
    ASSERT_TRUE(filePath.getCanExecute(), "Should have execute permission");
    ASSERT_TRUE(filePath.getPath() == "./cgi-bin/script.cgi", "Path should be './cgi-bin/script.cgi'");

    // Clean up
    remove("cgi-bin/script.cgi");
    rmdir("cgi-bin");

    return true;
}

bool test_FilePath_AutoIndexEnabled() {
    HTTPRequest mockRequest;

    RouteConfig mockRouteConfig;
    createMockEnvironment(mockRequest, mockRouteConfig);

    mockRequest.setUri("/testdir/");
    mockRouteConfig.setPath("/cgi-bin/");

    std::string root = "."; // current directory

    // Ensure that testdir exists in the current directory
    mkdir("testdir", 0755);

    FilePath filePath(root, mockRequest, mockRouteConfig);

    ASSERT_TRUE(filePath.getIsDirectory(), "Should detect as a directory");
    ASSERT_TRUE(filePath.getPath() == "./testdir", "Path should be './testdir'");
    ASSERT_TRUE(filePath.getIsSafe(), "Path should be safe");

    // Clean up
    rmdir("testdir");

    return true;
}



