/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 20:39:59 by Everton           #+#    #+#             */
/*   Updated: 2024/12/03 16:02:49 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <map>
#include "../parser/ServerConfig.hpp"

#define MAX_BUFFER_SIZE 65535
#define KB * 1024
#define MB * 1024 KB

class HTTPRequest {
	public:
		enum ParseState {
			REQUEST_LINE,
			HEADERS,
			BODY,
			COMPLETE,
			ERROR400 = 400,
			ERROR413 = 413,
			ERROR414 = 414,
			ERROR431 = 431,
			ERROR501 = 501,
			ERROR505 = 505
    	};

	private:
		std::string uri;
		std::string body;
		std::string method;
		std::string uploadPath;
		std::string queryString;
		std::string httpVersion;
		std::string contentType;
		std::map<std::string, std::string> headers;

		ParseState state;
		std::string rawData;
		size_t contentLength;
		bool chunkedEncoding;
		size_t maxBodySize;

		bool parseRequestLine(const std::string& line);
		bool parseHeaderLine(const std::string& line);
		bool endOfHeader();
		void handleQueryString();
		bool parseBody();
		std::string lineConstructor();
		bool chunkedEncondingHandler();

	public:
		HTTPRequest();
		~HTTPRequest();

		void appendData(const std::string& data, std::vector<ServerConfig> serverConfigs);
		bool isComplete() const;
		bool hasError() const;

		std::string getMethod() const;
		std::string getURI() const;
		std::string getQueryString() const;
		std::string getHTTPVersion() const;
		std::map<std::string, std::string> getHeaders() const;
		std::string getBody() const;
		std::string getContentType() const;
		size_t getContentLength() const;
		size_t getMaxBodySize() const;
		ParseState getState() const;
		std::string getUploadPath() const;

		void setState(ParseState state);
		void setMaxBodySize(size_t size);
		void setUploadPath(const std::string& path);
		void setUri(const std::string& uri);
};

std::ostream& operator<<(std::ostream& os, const HTTPRequest& request);