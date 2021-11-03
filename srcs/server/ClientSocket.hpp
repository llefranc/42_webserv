/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/22 15:02:08 by llefranc          #+#    #+#             */
/*   Updated: 2021/06/23 18:29:54 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTSOCKET_HPP
#define CLIENTSOCKET_HPP

#include <iostream>
#include <string>
#include <exception>
#include <queue>

#include "Request.hpp"
#include "Response.hpp"
#include "msg_format/StatusLine.hpp"
#include "../includes/webserv.hpp"
#include "../parser/ServerInfo.hpp"

class ClientSocket
{
	private:

		/* ------------------------------------------------------------- */
		/* ------------------------- ATTRIBUTES ------------------------ */

		int								_fd;			// File descriptor receiving / sending with the client
		const std::vector<ServerInfo>*	_infoVirServs;	// Servers blocks from config file that match a specific port
		
		Request							_request;		// Object containing the request
		Response            			_response;		// Queue containing the responses created from request object
        size_t                          _bytesSent;

	public:

		/* ------------------------------------------------------------- */
		/* ------------------------ COPLIEN FORM ----------------------- */

		ClientSocket(int fd, const std::vector<ServerInfo>* infoVirServs);
		ClientSocket(const ClientSocket& c);
		~ClientSocket();
		ClientSocket& operator=(ClientSocket a);


		/* ------------------------------------------------------------- */
		/* -------------------------- GETTERS -------------------------- */

		int getFd() const;
		Request* getRequest();
		Response* getResponse();
        size_t getNbBytesSent();


		/* ------------------------------------------------------------- */
		/* --------------------------- METHODS ------------------------- */

		// Add buffer into request, and request will parse the new line delimited by CRLF
		int receiveRequest(const char* buffer);
	
        void updateNbBytesSent(size_t n);


		/* ------------------------------------------------------------- */
		/* --------------- NON-MEMBER FUNCTION OVERLOADS --------------- */

		friend void swap(ClientSocket& a, ClientSocket& b);
		
}; // class ClientSocket

#endif