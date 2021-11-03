/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/14 16:14:02 by llefranc          #+#    #+#             */
/*   Updated: 2021/07/01 14:24:20 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpServer.hpp"


/* ------------------------------------------------------------- */
/* ------------------------ COPLIEN FORM ----------------------- */

HttpServer::HttpServer() :
	_readFds(), _writeFds(), _nbReadyFds() {}

HttpServer::~HttpServer() {}

HttpServer::HttpServer(const HttpServer& c) :
	_serverSocks(c._serverSocks), _clientSocks(c._clientSocks),
	_readFds(c._readFds), _writeFds(c._writeFds), _nbReadyFds(c._nbReadyFds) {}

HttpServer& HttpServer::operator=(HttpServer a)
{
	swap(a, *this);
	return *this;
}


/* ------------------------------------------------------------- */
/* --------------------------- METHODS ------------------------- */

void HttpServer::addServerSocket(ServerSocket sock)
{
	// Creating the socket and binding it to a port
	sock.createSocket();
	_serverSocks.push_back(sock);
}

void HttpServer::addClientSocket(int fdNewClient, int port, std::map<int, std::vector<ServerInfo> >& mSrv)
{
	_clientSocks.push_back(ClientSocket(fdNewClient, &(mSrv.find(port)->second)));
}

void HttpServer::etablishConnection(std::map<int, std::vector<ServerInfo> >& mSrv)
{
	struct timeval tv;

	while (true)
	{
		tv.tv_sec = MAX_SEC_TIMEOUT;
		tv.tv_usec = MAX_USEC_TIMEOUT;

		// Setting readFd with all passive accept sockets and all clients previously connected
		FD_ZERO(&_readFds);
		FD_ZERO(&_writeFds);
		
		addSocketsToReadFdSet<ServerSocket>(_serverSocks);
		addSocketsToReadFdSet<ClientSocket>(_clientSocks);
		addSocketsToWriteFdSet<ClientSocket>(_clientSocks);

		// Waiting for incoming connections on server sockets / client sockets
		if ((_nbReadyFds = select(FD_SETSIZE, &_readFds, &_writeFds, NULL, &tv)) < 0)
			throw std::runtime_error("Fatal error: select function failed\n");
		else if (!_nbReadyFds)
			throw std::runtime_error("Webserv timeout\n");

		// Send responses to clients
		sendToClients();
		// If a passive socket was activated, creates a new client connection
		connectNewClients(mSrv);
		// Receive from client
		requestHandler();
	}
}


/* ------------------------------------------------------------- */
/* ------------------ PRIVATE MEMBER FUNCTIONS ----------------- */

void HttpServer::sendToClients()
{
	for (std::list<ClientSocket>::iterator it = _clientSocks.begin(); it != _clientSocks.end(); ++it)
	{
		if (FD_ISSET(it->getFd(), &_writeFds))
		{
			const std::string *buffer = &it->getResponse()->getBuffer();
            
            long n = send(it->getFd(), static_cast<const void*>(buffer->c_str() + it->getNbBytesSent()), 
                           buffer->size() - it->getNbBytesSent(), 0);

            if (n != -1)
                it->updateNbBytesSent(n);
                
            // If an error occured or we have send everything, we close the connection. Else is still
            // some part of the buffer need to be send, going again through the select.
            if (n == -1 || it->getNbBytesSent() == buffer->size())
            {
                if (n == -1)
                    printLog("\033[1;31m >> FD " + convertNbToString(it->getFd()) + 
                        ": Failed to send response, connection closed\033[0m\n");
                else
                {
                    printLog("\033[1;32m >> FD " + convertNbToString(it->getFd()) + ": Response sent (code: " +
                        convertNbToString(it->getResponse()->getCode()) + "), connection closed\033[0m\n");
                }
                        
                std::list<ClientSocket>::iterator tmp = it--;
                closeConnection(&tmp);
            }

			// Exit for loop if no other sockets are ready
			--_nbReadyFds;
		}
	}
}

void HttpServer::requestHandler()
{
	for (std::list<ClientSocket>::iterator it = _clientSocks.begin(); it != _clientSocks.end() && _nbReadyFds; ++it)
	{
		if (FD_ISSET(it->getFd(), &_readFds))
		{
			char buffer[REQUEST_BUFFER_SIZE + 1];
			bzero(buffer, REQUEST_BUFFER_SIZE + 1);
			int n = recv(it->getFd(), buffer, REQUEST_BUFFER_SIZE, 0);

			if (n < 0)
            {
                printLog(" >> FD " + convertNbToString(it->getFd()) + ": Error on recv function, connection closed\n");
                std::list<ClientSocket>::iterator tmp = it--;
                closeConnection(&tmp);
            }

			// Client closed the connection
			else if (!n)
			{
				printLog(" >> FD " + convertNbToString(it->getFd()) + ": recv function returned 0, connection closed\n");
                std::list<ClientSocket>::iterator tmp = it--;
                closeConnection(&tmp);
			}

			// Concatenate buffer to actual stored request
            else
                it->receiveRequest(buffer);
	
			// Exit for loop if no other sockets are ready
			--_nbReadyFds;
		}
	}
}

void HttpServer::connectNewClients(std::map<int, std::vector<ServerInfo> >& mSrv)
{
	for (std::list<ServerSocket>::iterator it = _serverSocks.begin(); it != _serverSocks.end() && _nbReadyFds; ++it)
	{
		if (FD_ISSET(it->getFd(), &_readFds))
		{
			// Not use but need to be set for accept function
			struct sockaddr_in addrCli;
			int lenCli = sizeof(addrCli);
			bzero((char *) &addrCli, sizeof(addrCli));

			// Creates a new socket for a client connection
			int newClient;
			if ((newClient = accept(it->getFd(), (struct sockaddr *)&addrCli, (socklen_t *)&lenCli)) < 0)
				throw std::runtime_error("Fatal error: accept function failed\n");

			// Setting the fd in non-blocking mode, then saving it
			fcntl(newClient, F_SETFL, O_NONBLOCK);
			addClientSocket(newClient, it->getPort(), mSrv);
			printLog("\033[1;36m >> FD " + convertNbToString(newClient) + ": Client socket created\033[0m\n");

			// Exit for loop if no other sockets are ready
			--_nbReadyFds;
		}	
	}
}

void HttpServer::closeConnection(std::list<ClientSocket>::iterator *it)
{
    close((*it)->getFd());
    _clientSocks.erase(*it);
}


/* ------------------------------------------------------------- */
/* --------------- NON-MEMBER FUNCTION OVERLOADS --------------- */

void swap(HttpServer& a, HttpServer& b)
{
	std::swap(a._serverSocks, b._serverSocks);
	std::swap(a._clientSocks, b._clientSocks);
	std::swap(a._readFds, b._readFds);
	std::swap(a._writeFds, b._writeFds);
	std::swap(a._nbReadyFds, b._nbReadyFds);
}
