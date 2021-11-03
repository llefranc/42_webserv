/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/03 14:14:47 by lucaslefran       #+#    #+#             */
/*   Updated: 2021/06/18 14:41:42 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <ctime>
#include <sys/stat.h>
#include <cstdio>
#include <fstream>
#include <iostream>

#include "Request.hpp"
#include "Cgi.hpp"
#include "msg_format/StatusLine.hpp"
#include "msg_format/Body.hpp"

#include "../parser/FileParser.hpp"
#include "../parser/ServerInfo.hpp"

class Response
{
	private:
	
		/* ------------------------------------------------------------- */
		/* ------------------------- ATTRIBUTES ------------------------ */

		const std::vector<ServerInfo>*		_infoVirServs;	// Servers blocks from config file that match a specific port
		Request*							_req;			// Request object when the request is fully received, used to create response

		StatusLine							_staLine;		// Fist line of http response
	
		std::string							_buffer;		// Buffer containing the response that will be send. Directly writing
															// headers into it.

        bool                                _autoIndex;     // Sets to true if request is GET or HEAD, the target (after rooting) 
                                                            // is a directory, and autoindex is on.

	public:

		/* ------------------------------------------------------------- */
		/* ------------------------ COPLIEN FORM ----------------------- */

		Response();
		Response(Request* req, const StatusLine& staLine, const std::vector<ServerInfo>* infoVirServs);
		Response(const Response& c);
		~Response();
		Response& operator=(Response a);

		
		/* ------------------------------------------------------------- */
		/* --------------------------- SETTERS ------------------------- */

		void setRequest(Request* req);
		void setStatusLine(const StatusLine& staLine);
		void setInfoVirtualServs(const std::vector<ServerInfo>* infoVirServs);


		/* ------------------------------------------------------------- */
		/* --------------------------- GETTERS ------------------------- */
		
		const StatusLine& getStatusLine() const;
		int getCode() const;
		const std::string& getBuffer() const;
		

		/* ------------------------------------------------------------- */
		/* --------------------------- METHODS ------------------------- */

		// Reset response object
		void clear();

		// Fill response buffer according to request object and status line previously set
		void fillBuffer();


	private:

		/* ------------------------------------------------------------- */
		/* ----------------------- PRIVATE METHODS --------------------- */

		// Fills buffer with Content-length header
		void fillContentlengthHeader(const std::string& size);

		// Fills buffer with server header with server name (webserv)
		void fillServerHeader();

		// Fills buffer with Date header with the actual date
		void fillDateHeader();

		// Fills buffer with Last-Modified header (last modification of a file)
		void fillLastModifiedHeader(const char* uri);

        // Fills buffer with Location header (for HTTP redirection)
        void fillLocationHeader(const std::string& redirectedUri);

		// Fills buffer with status line
		void fillStatusLine(const StatusLine& staLine);

		// Transforms URI using index and root settings
		std::string reconstructFullURI(int method,
				const std::pair<const std::string, const Location*>& loc, std::string uri);

		// Replaces the location name that matched with root directive
		void replaceLocInUri(std::string* uri, const std::string& root, const std::string& locName);

		// Try to add all the indexs until one path is correct. If none are correct, throws a StatusLine
		// object with a 301 error code
		std::string addIndex(const std::string& uri, const std::vector<std::string>& indexs);
		
		// Compares the methods allowed from a location block with the requested method. If not allowed,
		// throws a StatusLine object with a 405 error code
		void checkMethods(int method, const std::vector<std::string>& methodsAllowed) const;

		// Fill the buffer with status line + 2 headers (server and date), then with the appropriate 
		// error page as body (by default or the one set in the server block from the config file)
		void fillError(const StatusLine& sta);

		// Creates a file and wrote the post body to it (or if the file is already existing,
		// appends it at the end)
		void postToFile(const std::string& uri);

        // Execute CGI and sets the buffer with status line / headers / cgi output as payload
		void execCgi(const std::string& realUri, std::string* cgiName);

        void execGet(const std::string& realUri);

        void execPost(const std::string& realUri);

        void execDelete(const std::string& realUri);


	public:
	
		/* ------------------------------------------------------------- */
		/* --------------- NON-MEMBER FUNCTION OVERLOADS --------------- */

		// Swap two Response object
		friend void swap(Response& a, Response& b);
	
}; // class Response

#endif