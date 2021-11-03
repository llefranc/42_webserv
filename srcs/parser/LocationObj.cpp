/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationObj.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucaslefrancq <lucaslefrancq@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 15:17:35 by hherin            #+#    #+#             */
/*   Updated: 2021/06/21 16:32:33 by lucaslefran      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LocationObj.hpp"

// ============================================================================
// =========================== COPLIEN FORM ===================================

Location::Location(int s) : _port(s), _autoindex(0)
{
	setStringArray("POST GET HEAD DELETE", _allow_methd);
}

Location::Location(Location const &loc) : _port(loc._port), _autoindex(loc._autoindex), _root(loc._root), 
_upload_path(loc._upload_path), _redirect(loc._redirect), _allow_methd(loc._allow_methd), _index(loc._index),
_cgi_exe(loc._cgi_exe) {}

Location &Location::operator=(Location const &loc)
{
	Location tmp(loc);
	myswap(tmp, *this);
	return *this;
}

Location::~Location() { }

// ============================================================================
// =============================== GETTERS ====================================

int const &Location::getPort() const { return _port; }

std::string const &Location::getRoot() const { return _root; }

std::string const &Location::getUploadPath() const { return _upload_path; }

std::vector<std::string> const &Location::getMethods() const { return _allow_methd; }

std::vector<std::string> const &Location::getIndex() const { return _index; }

std::map<std::string, std::string> const &Location::getCgiExe() const { return _cgi_exe; }

bool const &Location::getAutoIndex() const { return _autoindex; }

std::string const &Location::getRedirect() const { return _redirect; }

// ============================================================================
// =============================== SETTERS ====================================

/**
** set variable inside location object
** @param nb is which method of the object is choosen
** @param 
*/
void	Location::setLocation(int nb, int const &pos, std::string const &buf)
{
	typedef void (Location::*MemFuncPtr)(const char*);
	MemFuncPtr F[] = { &Location::setMethods, &Location::setIndex,
						&Location::setUploadStore, &Location::setRoot, &Location::setCgiExe, 
						&Location::setAutoIndex, &Location::setRedirect};

	const char *tmp = buf.c_str() + pos;
	int i = 0;

	while (isspace(tmp[i]))
		i++;

	(this->*F[nb])(tmp + i);
}

void Location::setRoot(char const *r) 
{
	std::string cString = r;
	if (numberOfWords(r) != 1)
		throw std::runtime_error("Error : " + std::string(cString) + " - put only one root path\n");
	_root = *wsTrim(cString);
}

void Location::setUploadStore(char const *u) 
{
	std::string cString = u;
	if (numberOfWords(u) != 1)
		throw std::runtime_error("Error : " + std::string(cString) + " - put only one upload_path input\n");
	_upload_path = *wsTrim(cString);
}

void Location::setMethods(char const *n) 
{
	_allow_methd.clear();
	setStringArray(n, _allow_methd);
	const char *method_checking[] = { "POST", "GET", "HEAD", "DELETE"};
	for (size_t i = 0; i < _allow_methd.size(); i++){
		for (size_t j = 0; j < 4; j++){
			if (!strcmp(method_checking[j], _allow_methd[i].c_str()))
				break;
			if (j == 4)
				throw std::runtime_error("Error : method " + std::string(_allow_methd[i]) + " is not accepted\n");
		}
	}
}

void Location::setRedirect(char const *r) 
{
	std::vector<std::string> v;
	setStringArray(r, v);
	if (v.size() != 1)
		throw std::runtime_error("Error : " + std::string(r) + " - too many input for redirect\n");
	_redirect = v[0];
}

void Location::setIndex(char const *n) { setStringArray(n, _index); }

void Location::setCgiExe(char const *c) 
{
	std::vector<std::string> cgiTmp;
	setStringArray(c, cgiTmp);
	if (cgiTmp.size() != 2)
		throw std::runtime_error("Wrong cgi input in the config file\n"); 
	_cgi_exe.insert(std::pair<std::string, std::string>(cgiTmp[0], cgiTmp[1]));
}

void Location::setAutoIndex(char const *i)
{
	std::string tmp(i);
	tmp = *wsTrim(tmp);
	if (tmp.compare("on") && tmp.compare("off"))
		throw std::runtime_error("Error autoIndex : " + std::string(i) + "isn't a good input\n");
	else if (!tmp.compare("on"))
		_autoindex = 1;
	else
		_autoindex = 0;
}

// ============================================================================
// =============================== METHODS ====================================

void Location::printLocation(const std::string& locName) const
{
	std::cout << "\nLOCATION\n-------------------------\n"
			<< "name = |" << locName << "|\n"
			<< "root = |" << _root << "|\n";
	
	if (_cgi_exe.size())
		for (std::map<std::string, std::string>::const_iterator it = _cgi_exe.begin(); it != _cgi_exe.end(); it++)
			std::cout << "cgi_exe size = 2  and .ext and exe =  |" << it->first << "| |" << it->second << "|\n";
	else
		std::cout << "cgi_exe size = " << _cgi_exe.size() << "\n";
	
	for (std::vector<std::string>::const_iterator it = _allow_methd.begin(); it != _allow_methd.end(); ++it)
		std::cout << "method = |" << *it << "|\n";

	for (std::vector<std::string>::const_iterator it = _index.begin(); it != _index.end(); ++it)
		std::cout << "index = |" << *it << "|\n";
	
	std::cout << "-------------------------\n\n";
}


// ============================================================================
// ========================== PRIVATE METHODS =================================
template <class T>
void Location::myswap(T t1, T t2)
{
	T tmp = t1;
	t1 = t2;
	t2 = tmp;
}