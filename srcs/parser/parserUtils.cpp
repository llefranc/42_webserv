/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parserUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heleneherin <heleneherin@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/27 13:29:16 by hherin            #+#    #+#             */
/*   Updated: 2021/06/08 18:18:39 by heleneherin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.hpp"

// Clean the buf of all spaces
int	cleanLineFromSpaces(std::string &buf)
{
	int i = 0;
	while (isspace(buf[i]))
		buf.erase(0, 1);

	if (buf.size()){
		i = buf.size() - 1;
		while (isspace(buf[i]))
			buf.erase(i, 1);
	}
	return 0;
}

// 
int	bracketRegulator(int &bracket, std::string const &buf)
{
	size_t pos = -1; 
	while ( (pos = buf.find_first_of('{', pos + 1)) != std::string::npos) 
		bracket++;
	
	pos = -1;
	while ( (pos = buf.find_first_of('}', pos + 1)) != std::string::npos)
		bracket--;
	return 1;
}

// Return full error message for the parser
std::string const &errParseMessage(char const *begErrMsg, char const *errorToAdd, char const *endErrMsg)
{
	static std::string fullErrMsg;

	fullErrMsg.clear();
	fullErrMsg = begErrMsg;
	fullErrMsg += errorToAdd;
	fullErrMsg += endErrMsg;

	return fullErrMsg;	
}

// Count words of given buffer
size_t numberOfWords(char const *buf)
{
	int word = 0, index = 0;
	while (buf[index] != '\0'){
		while (isspace(buf[index]))
			index++;
		int tmp = index;
		while (isalnum(buf[index]) || ispunct(buf[index]))
			index++;
		if (tmp !=  index)
			word++;
	}
	return word;
}

// erase all whitespaces in buf
std::string *wsTrim(std::string &buf)
{
    if (buf.empty())
        return &buf;
    for (std::string::iterator it = buf.end(); it != buf.begin(); --it)
        if (isspace(*it))
            buf.erase(it);            
    return &buf;
}

// Split a char array into a vector of word
void setStringArray(char const *n, std::vector<std::string> &v) 
{
    char *tmp = strdup(n);
    char *token = strtok(tmp, "\t\v\f\r ");

    while (token){
		
        v.push_back(token);
        token = strtok(NULL, "\t\v\f\r ");
    }
    free(tmp);
}
