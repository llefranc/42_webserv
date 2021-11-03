/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerInfo.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/12 16:58:27 by hherin            #+#    #+#             */
/*   Updated: 2021/06/08 19:15:48 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerInfo.hpp"

// ============================================================================
// =========================== COPLIEN FORM ===================================

ServerInfo::ServerInfo() : _port(-1), _max_cli_body(-1) { }

ServerInfo::ServerInfo(ServerInfo const &copy) : _port(copy._port),
 _error_path(copy._error_path), _names(copy._names), _max_cli_body(copy._max_cli_body),
  _location(copy._location)
{}

ServerInfo &ServerInfo::operator=(ServerInfo const &copy)
{
    ServerInfo tmp(copy);
    std::swap(tmp, *this);
    return *this;
}

ServerInfo::~ServerInfo() { }


// ============================================================================
// =============================== GETTERS ====================================

int const &ServerInfo::getMaxClientsBS() const { return _max_cli_body; }

std::string const &ServerInfo::getError() const { return _error_path; }

std::vector<std::string> const &ServerInfo::getNames() const { return _names; }

int const &ServerInfo::getPort() const { return _port; }

std::map<std::string, Location> const *ServerInfo::getLocation() const { return &_location; }

std::string const &ServerInfo::getHost() const { return _host; }


// ============================================================================
// =============================== SETTERS ====================================

/**
** set variable inside server object
** param nb is which method of the object is choosen
** param (*D) conversion function to pass in class' method
*/
void	ServerInfo::setServer(int nb, int const &pos, std::string const &buf)
{
	typedef void (ServerInfo::*MemFuncPtr)(const char*);
	MemFuncPtr F[] = { &ServerInfo::setPort, &ServerInfo::setError, &ServerInfo::setNames, 
                        &ServerInfo::setMaxClientsBS, &ServerInfo::setHost };
                        
    const char *tmp = buf.c_str() + pos;
	int i = 0;

	while (isspace(tmp[i]))
        i++;
    
	(this->*F[nb])(tmp + i);
}

void ServerInfo::setMaxClientsBS(char const *m)
{
    std::string cString = m;
    if (numberOfWords(m) != 1)
        throw std::runtime_error("Error : " + cString + " - put only one input for body size\n");
    
    cString = *wsTrim(cString);
    for (size_t i = 0; i < cString.size(); i++) 
		if (cString[i] < 48 || cString[i]  > 57)
			throw std::runtime_error("Error : " + cString + " - use only digit for listen\n");

    _max_cli_body = atoi(m);
}

void ServerInfo::setError(char const *e) 
{
    if (numberOfWords(e) != 1)
        throw std::runtime_error("Error : " + std::string(e) + " - put only one error path\n");
    _error_path = e;
}

void ServerInfo::setHost(char const *h) { _host = h; }

void ServerInfo::setNames(char const *n) { setStringArray(n, _names); }

void ServerInfo::setPort(char const *p) 
{
    std::string cString = p;
    if (numberOfWords(p) != 1)
        throw std::runtime_error("Error : " + cString + " - put only one port to listen\n");
    
    cString = *wsTrim(cString);
    for (size_t i = 0; i < cString.size(); i++) 
		if (cString[i] < 48 || cString[i]  > 57)
			throw std::runtime_error("Error : " + cString + " - use only digit for listen\n");
    
    _port = atoi(p);
}

std::map<std::string, Location> *ServerInfo::setSrvLocation() { return &_location; }

// ============================================================================
// ========================== PRIVATE METHODS =================================
template <class T>
void ServerInfo::myswap(T t1, T t2)
{
    T tmp = t1;
    t1 = t2;
    t2 = tmp;
}