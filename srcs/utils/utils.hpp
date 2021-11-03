/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/10 14:19:48 by llefranc          #+#    #+#             */
/*   Updated: 2021/06/17 19:36:49 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

# include <cctype>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <unistd.h>

#include "../parser/LocationObj.hpp"
#include "../server/msg_format/StatusLine.hpp"

class Location; 
class ServerInfo;
class StatusLine;

// Converts a char upper case to lower case
char asciiToLower(char in);

// Return a string containing a number (equivalent to atoi)
std::string convertNbToString(size_t nb);

// Return a vector of string containing a line splitted with a sep (sep included)
std::vector<std::string> stringDelimSplit(std::string const &str, const char *delim);

// Return a vector of string containing a line splitted with a sep
std::vector<std::string> splitWithSep(std::string line, char sep);

// Search inside a map of location names /location block the best possible match with locName. Return 
// std::pair<empty string, null ptr> if no match.
std::pair<const std::string, const Location*> 
		matchLocation(const std::map<std::string, Location> *loc, const std::string& locName);

// srv = list of virtual server for one port, names.first = name of virtual server, names.second = location name
// Return the best possible location block for a specific host+URI. Return NULL if no match
std::pair<const std::string, const Location*>
		locationSearcher(const std::vector<ServerInfo> *srv, std::pair<std::string, std::string> const &names);

// Returns a pointer to the virtual server block that matches hostValue from host header field. 
// If no match, returns NULL.
const ServerInfo* findVirtServ(const std::vector<ServerInfo>* infoVirServs, const std::string& hostValue);

// If the URI ends by an extenstion, returns the dot position. Else returns std::string::npos
size_t isExtension(const std::string& uri);

// Compare the end of the URI, and it ends with an extension:
// 	- if the extension is ".cgi", returns an allocated std::string with ".cgi"
//	- if the extension matches an extension from location block passed as argument,
//	  returns an allocated std::string with the name of the executable to run for this file
//  - in all other cases, returns NULL.
std::string* getCgiExecutableName(const std::string& uri, const Location* loc);

// Prints the time of the day and the msg on std::cout
void printLog(const std::string &msg, const std::string& addInfo = "");

// Split the path string into :
// first = path to the requested file | second = name of executable
std::pair<std::string, std::string> *SplitPathForExec(std::string const &path);

void autoIndexDisplayer(std::string const &uri, std::string* displayList);

#endif