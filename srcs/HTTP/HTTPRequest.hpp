/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Everton <egeraldo@student.42sp.org.br>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 20:39:59 by Everton           #+#    #+#             */
/*   Updated: 2024/10/30 21:41:03 by Everton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <map>

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
		std::string httpVersion;
		std::map<std::string, std::string> headers;
		std::string body;

		ParseState state;
		std::string rawData;
		size_t contentLength;
		bool chunkedEncoding;
		size_t bodyBytesReceived;

		bool parseRequestLine(const std::string& line);
		bool parseHeaderLine(const std::string& line);
		bool endOfHeader();
		bool parseBody();

	public:
		HTTPRequest();
		~HTTPRequest();

		void appendData(const std::string& data);
		bool isComplete() const;
		bool hasError() const;

		std::string getMethod() const;
		std::string getURI() const;
		std::string getHTTPVersion() const;
		std::map<std::string, std::string> getHeaders() const;
		std::string getBody() const;
};