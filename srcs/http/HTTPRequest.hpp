/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 20:39:59 by Everton           #+#    #+#             */
/*   Updated: 2024/11/10 21:39:55 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <map>
#include "../parser/ServerConfig.hpp"

class HTTPRequest {
	public:
		enum ParseState {
			REQUEST_LINE,
			HEADERS,
			BODY,
			COMPLETE,
			ERROR
    	};

	private:
		std::string method;
		std::string uri;
		std::string queryString;
		std::string httpVersion;
		std::string contentType;
		std::map<std::string, std::string> headers;
		std::string body;

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

		void setMaxBodySize(size_t size);

};