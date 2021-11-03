/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hherin <hherin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/27 13:30:15 by hherin            #+#    #+#             */
/*   Updated: 2021/06/03 17:40:00 by hherin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

# include "ServerInfo.hpp"
# include "../utils/utils.hpp"
# include <string>
# include <cctype>  // function isspace
# include <iostream>


int	cleanLineFromSpaces(std::string &buf);

int	bracketRegulator(int &bracket, std::string const &buf);

// Construct error messages for the parser
std::string const &errParseMessage(char const *begErrMsg, char const *errorToAdd, char const *endErrMsg);

// Count how many words are in buf
size_t numberOfWords(char const *buf);

// Remove whitespaces of given buffer and return its pointer
std::string *wsTrim(std::string &buf);

// Push each word of n in the vector v
void setStringArray(char const *n, std::vector<std::string> &v);

#endif