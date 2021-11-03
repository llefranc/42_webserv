/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/14 14:56:29 by llefranc          #+#    #+#             */
/*   Updated: 2021/04/14 15:27:04 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERSOCKET_HPP
#define SERVERSOCKET_HPP

#include <iostream>
#include <string>
#include <exception>

#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h> //inet itoa
#include <sys/time.h>

#include "../includes/webserv.hpp"

class ServerSocket
{
	private:

		/* ------------------------------------------------------------- */
		/* ------------------------- ATTRIBUTES ------------------------ */

		int _port;			// The port that server socket will listen 
		int _maxClient;		// Number of max clients that server socket is listening
		int	_fd;			// Fd representing server socket


	public:

		/* ------------------------------------------------------------- */
		/* ------------------------ COPLIEN FORM ----------------------- */

		ServerSocket(int port, int maxClient);
		ServerSocket(const ServerSocket& copy);
		~ServerSocket();
		ServerSocket& operator=(ServerSocket assign);
		

		/* ------------------------------------------------------------- */
		/* -------------------------- GETTERS -------------------------- */

		// Return socket file descriptor
		int getFd() const;

		// Return the port that the socket is listening
		int getPort() const;
		
		
		/* ------------------------------------------------------------- */
		/* --------------------------- METHODS ------------------------- */
		
		// Create a TCP socket listening on a specific port for x clients
		void createSocket();
	
	
	private:

		/* ------------------------------------------------------------- */
		/* --------------- NON-MEMBER FUNCTION OVERLOADS --------------- */

		friend void swap(ServerSocket& a, ServerSocket& b);

}; // class ServerSocket

#endif