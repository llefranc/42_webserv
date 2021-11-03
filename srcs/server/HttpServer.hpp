/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/14 16:21:22 by llefranc          #+#    #+#             */
/*   Updated: 2021/06/17 16:01:50 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPSERVER_HPP
#define HTTPSERVER_HPP

#include <list>
#include <vector>
#include <sys/select.h>
#include <fcntl.h>

#include "ServerSocket.hpp"
#include "ClientSocket.hpp"
#include "../includes/webserv.hpp"
#include "../parser/ServerInfo.hpp"

class ServerSocket;
class ClientSocket;

class HttpServer
{
	private:

		/* ------------------------------------------------------------- */
		/* ------------------------- ATTRIBUTES ------------------------ */

		std::list<ServerSocket>	_serverSocks;	// All passive sockets, each one listening on a specific port
		std::list<ClientSocket>	_clientSocks;	// All connection established with clients
		fd_set					_readFds;		// Useful when using select and trying to receive data
		fd_set					_writeFds;		// Useful when using select and trying to send data
		int						_nbReadyFds;	// Nb of fds activated (could be from write fd_set or read fd_set)


	public:

		/* ------------------------------------------------------------- */
		/* ------------------------ COPLIEN FORM ----------------------- */
		
		HttpServer();
		HttpServer(const HttpServer& c);
		~HttpServer();
		HttpServer& operator=(HttpServer a);


		/* ------------------------------------------------------------- */
		/* --------------------------- METHODS ------------------------- */
		
		// Create the socket and add socket id + socket fd
		void addServerSocket(ServerSocket sock);
		
		// Store the client fd when a connection is etablished
		void addClientSocket(int fdNewClient, int port, std::map<int, std::vector<ServerInfo> >& mSrv);
		
		// Infinite loop, sets fd_set with all fds for passive accept sockets + connected clients
		void etablishConnection(std::map<int, std::vector<ServerInfo> >& mSrv);

        // Close the file descriptor associated to the iterator, and then remove the ClientSocket object
        // from the list.
        void closeConnection(std::list<ClientSocket>::iterator *it);

	private:

		/* ------------------------------------------------------------- */
		/* ------------------ PRIVATE MEMBER FUNCTIONS ----------------- */
		
		// Set readfd with the content of a socket's list
		template <typename T>
		void addSocketsToReadFdSet(const std::list<T>& sockets)
		{
			for (typename std::list<T>::const_iterator it = sockets.begin(); it != sockets.end(); ++it)
				FD_SET(it->getFd(), &_readFds);
		}

		// Set writefd with the content of a socket's list for each client socket containing at least
		// one response in the queue
		template <typename T>
		void addSocketsToWriteFdSet(std::list<T>& sockets)
		{
			for (typename std::list<T>::iterator it = sockets.begin(); it != sockets.end(); ++it)
				if (!it->getResponse()->getBuffer().empty())
					FD_SET(it->getFd(), &_writeFds);
		}

		// Check all passive accept socket and if one is ready, create a new client connection
		void connectNewClients(std::map<int, std::vector<ServerInfo> >& mSrv);

		// Checks all clients connections, and if one is communicating receive his request
		void requestHandler();

		// If a response is ready and the client is willing to accept it, send it
		void sendToClients();


	public:
	
		/* ------------------------------------------------------------- */
		/* --------------- NON-MEMBER FUNCTION OVERLOADS --------------- */

		friend void swap(HttpServer& a, HttpServer& b);

}; // class HttpServer

#endif