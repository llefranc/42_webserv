/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucaslefrancq <lucaslefrancq@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/09 14:06:51 by hherin            #+#    #+#             */
/*   Updated: 2021/06/21 16:38:34 by lucaslefran      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FileParser.hpp"

// ============================================================================
// =========================== COPLIEN FORM ===================================

FileParser::FileParser(const char *filepath) : _bracket(0), _cli_srv(0), _filePath(filepath) { _file.open(filepath); }

FileParser::FileParser(const char *filepath, bool s) : _bracket(0), _cli_srv(0), _filePath(filepath)
{
	#if DEBUG
		std::cout << "FileParser object trying to read file at path :|" << filepath << "|\n";
	#endif
	
	_file.open(filepath);
	parseRequestFile();
	(void)s;
}

FileParser::FileParser(FileParser const&copy) :  _buf(copy._buf), _bracket(copy._bracket), 
_m_srv(copy._m_srv), _requestFile(copy._requestFile), _cli_srv(copy._cli_srv), _filePath(copy._filePath)
{ _file.open(_filePath.c_str()); }

FileParser &FileParser::operator=(FileParser const&copy)
{
	FileParser tmp(copy);

	std::swap(tmp, *this);
	return *this;
}

FileParser::~FileParser() { _file.close(); }


// ============================================================================
// ============================ PUBLIC METHODS ================================

std::string const& FileParser::getRequestFile() { return _requestFile; }

std::map<int, std::vector<ServerInfo> > const &FileParser::getConfigFile() { parseConfigFile(); return _m_srv; }

void FileParser::setServerInfo(ServerInfo *s) { _cli_srv = s; }

size_t FileParser::getRequestFileSize() const { return _requestFile.size(); }


// ============================================================================
// =========================== PRIVATE METHODS ================================

// Parse all the file whithout any process
void FileParser::parseRequestFile()
{
	while (std::getline(_file, _buf))
		_requestFile.append(_buf + "\n");
}

// Parsing of the configuration file
void FileParser::parseConfigFile()
{
	// loop for each block server if there is something else, throw an error
	while (std::getline(_file, _buf)){
		
		if (!_buf.compare(0, 9, "server {")){
			_bracket = 0;
			bracketRegulator(_bracket, _buf);
			newServer();
		}
		else if (!_buf.compare(0, 1, "}") || !_buf.compare(0, 2, ""))
			continue;
		else throw std::runtime_error(errParseMessage("error line : ", _buf.c_str(), " not a good name for server block\n")); // ERREUR 
	}
}

// check bracket la ou il faut pas
void FileParser::newLocation(ServerInfo &srv)
{
	Location n_loc(srv.getPort());
	std::string uri;
	int brack = 0;

	bracketRegulator(brack, _buf);

	// Processing of the first line of location
	if (!_buf.compare(0, 9, "location ")){
		
		// **check validity of the line**	
		if (numberOfWords(_buf.c_str()) != 3)
			throw std::runtime_error(errParseMessage("error line : ", _buf.c_str(), "\n"));
		
		// **get the locaton name without whitespaces**
		(!_buf.compare(_buf.size() - 1, 1, "{")) ? _buf.erase(_buf.size() - 1, 1) : 0;
		uri = *wsTrim(_buf);
		uri.replace(0, 8, "");
	}
	
	// Processing of the location block
	while (brack > 0)
	{
		std::getline(_file, _buf);
		cleanLineFromSpaces(_buf);
		bracketRegulator(brack, _buf);

		if (!_buf.compare(0, 13, "allow_method "))
			n_loc.setLocation(METHO, 13, _buf);
		else if (!_buf.compare(0, 6, "index "))
			n_loc.setLocation(IDX, 6, _buf);
		else if (!_buf.compare(0, 12, "upload_path "))
			n_loc.setLocation(STORE, 12, _buf);
		else if (!_buf.compare(0, 5, "root "))
			n_loc.setLocation(ROOT, 5, _buf);
		else if (!_buf.compare(0, 4, "cgi "))
			n_loc.setLocation(CGI_EXE, 4, _buf);
		else if (!_buf.compare(0, 10, "autoindex "))
			n_loc.setLocation(AUTOIDX, 10, _buf);
		else if (!_buf.compare(0, 9, "redirect "))
			n_loc.setLocation(REDIRECT, 9, _buf);
		else if (!_buf.compare(0, 1, "}")) continue;
		else throw std::runtime_error(errParseMessage("error line : ", _buf.c_str(), ". Wrong input in block location\n"));
	}
	_bracket--;
	addNewLocationToMap(&n_loc, uri);
}

void FileParser::newServer(void)
{
	ServerInfo n_srv;
	while (_bracket > 0)
	{
		std::getline(_file, _buf);
		cleanLineFromSpaces(_buf);
		bracketRegulator(_bracket, _buf);

		if (!_buf.compare(0, 9, "location "))	
			newLocation(n_srv);
		else if (!_buf.compare(0, 7, "listen "))
			n_srv.setServer(LIS, 7, _buf);
		else if (!_buf.compare(0, 6, "error "))
			n_srv.setServer(ERR, 6, _buf);
		else if (!_buf.compare(0, 12, "server_name "))
			n_srv.setServer(SRV_N, 12, _buf);
		else if (!_buf.compare(0, 9, "max_body "))
			n_srv.setServer(BODY, 9, _buf);
		else if (!_buf.compare(0, 5, "host "))
			n_srv.setServer(HOST, 5, _buf);
		else if (!_buf.compare(0, 1, "}")) continue;
		else std::runtime_error(errParseMessage("error line : ", _buf.c_str(), ". Not accepting this input\n"));
	}
	addNewServerToMap(n_srv);
}

void FileParser::addNewServerToMap(ServerInfo &srv)
{
	int s_port = srv.getPort();
	srv.setSrvLocation()->insert(_m_loc.begin(), _m_loc.end());
	std::map<int, std::vector<ServerInfo> >::iterator it = _m_srv.find(s_port);
		
	if (it == _m_srv.end()){
		std::vector<ServerInfo> v_srv(1, srv);
		_m_srv.insert(std::pair<int, std::vector<ServerInfo> >(s_port, v_srv));
	}
	else
		it->second.push_back(srv);
	_m_loc.clear();
}

void FileParser::addNewLocationToMap(Location *loc, std::string const &name)
{
	std::map<std::string, Location>::iterator it = _m_loc.find(name);
	
	if (it == _m_loc.end())
		_m_loc.insert(std::pair<std::string, Location>(name, *loc));
	else
		throw std::runtime_error(errParseMessage("Error : location ", name.c_str(), " already exist\n"));
}
