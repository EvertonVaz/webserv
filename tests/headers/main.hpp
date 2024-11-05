/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 20:16:41 by Everton           #+#    #+#             */
/*   Updated: 2024/11/05 19:55:55 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
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
bool testErrorPage();

bool testServerInitializationSuccess();
bool testServerSocketCreationFailure();
bool testServerSetsockoptFailure();
bool testServerInetPtonFailure();
bool testServerBindFailure();
bool testServerListenFailure();
bool testServerDestructorClosesSockets();
bool testServerMultipleListenSockets();

bool test_connection_manager_initialization();
bool test_connection_manager_accept_new_connection();
bool test_connection_manager_read_from_client();
bool test_connection_manager_write_to_client();
bool test_connection_manager_close_connection_on_error();
bool test_connection_manager_read_and_write_simultaneously();

bool test_route_request();
bool test_handle_request_serve_static_file();
bool test_handle_request_method_not_allowed();
bool test_handle_request_redirect();
bool test_handle_request_autoindex_enabled();
bool test_handle_request_autoindex_disabled_no_index();