/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/14 14:55:48 by llefranc          #+#    #+#             */
/*   Updated: 2021/04/14 15:26:55 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerSocket.hpp"


/* ------------------------------------------------------------- */
/* ------------------------ COPLIEN FORM ----------------------- */

ServerSocket::ServerSocket(int port, int maxClient) :
	_port(port), _maxClient(maxClient), _fd() {}

ServerSocket::ServerSocket(const ServerSocket& c) :
		_port(c._port), _maxClient(c._maxClient), _fd(c._fd) {}

ServerSocket::~ServerSocket() {}

ServerSocket& ServerSocket::operator=(ServerSocket a)
{
	swap(a, *this);
	return *this;
}


/* ------------------------------------------------------------- */
/* -------------------------- GETTERS -------------------------- */

int ServerSocket::getFd() const { return _fd; }

int ServerSocket::getPort() const { return _port; }


/* ------------------------------------------------------------- */
/* --------------------------- METHODS ------------------------- */

void ServerSocket::createSocket()
{
	// Creating a TCP socket
	if ((_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		throw std::runtime_error("Fatal error: socket function failed\n");  // A peaufiner, il ne faut surement pas compleement exit des qu'une fonction bug

	// Setting its options
	int yes = true;
	if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		throw std::runtime_error("Fatal error: setsockopt function failed\n");  // A peaufiner, il ne faut surement pas compleement exit des qu'une fonction bug
	
	// Initializing structure for socket information (IP, port..)
	struct sockaddr_in socketAddr;
	bzero((char *) &socketAddr, sizeof(socketAddr));
	socketAddr.sin_family = AF_INET;
	socketAddr.sin_addr.s_addr = INADDR_ANY;
	socketAddr.sin_port = htons(_port);
	
	// Binding the socket to a port
	if (bind(_fd, (struct sockaddr *) &socketAddr, sizeof(socketAddr)) < 0)
		throw std::runtime_error("Fatal error: bind function failed\n");  // A peaufiner, il ne faut surement pas compleement exit des qu'une fonction bug

	listen(_fd, _maxClient);
	printLog(" >> PORT " + convertNbToString(_port) + ": HTTP socket created\n");
}


/* ------------------------------------------------------------- */
/* --------------- NON-MEMBER FUNCTION OVERLOADS --------------- */

void swap(ServerSocket& a, ServerSocket& b)
{
	std::swap(a._port, b._port);
	std::swap(a._maxClient, b._maxClient);
	std::swap(a._fd, b._fd);
}