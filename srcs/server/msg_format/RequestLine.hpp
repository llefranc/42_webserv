/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestLine.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/06 16:51:26 by lucaslefran       #+#    #+#             */
/*   Updated: 2021/06/08 17:03:26 by llefranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTLINE_HPP
#define REQUESTLINE_HPP

#include "../../includes/webserv.hpp"
#include <iostream>

class RequestLine
{
	private:
	
		/* ------------------------------------------------------------- */
		/* ------------------------- ATTRIBUTES ------------------------ */
	
		int			_method;	// Enum methods { GET, HEAD, PUT, POST, DELETE };
		std::string _path;		// Contains the path where search / put the file
		std::string _query;		// Contains args coming after first '?' in URI


	public:

		/* ------------------------------------------------------------- */
		/* ------------------------ COPLIEN FORM ----------------------- */

		RequestLine() : _method(-1) {}
		RequestLine(const RequestLine& c) :
				_method(c._method), _path(c._path), _query(c._query) {}
		~RequestLine() {}
		RequestLine& operator=(RequestLine a)
		{
			swap(*this, a);
			return *this;
		}
		
		
		/* ------------------------------------------------------------- */
		/* --------------------------- GETTERS ------------------------- */

		int getMethod() const { return _method; }
		const std::string& getPath() const { return _path; }
		const std::string& getQuery() const { return _query; }


		/* ------------------------------------------------------------- */
		/* --------------------------- SETTERS ------------------------- */
		
		void setMethod(int method) { _method = method; }
		void setPath(const std::string& path) { _path = path; }
		void setQuery(const std::string& query) { _query = query; }
		

		/* ------------------------------------------------------------- */
		/* -------------------------- METHODS -------------------------- */

		// Clear RequestLine object
		void clear()
		{
			_method = -1;
			_path.clear();
			_query.clear();
		}

		// Returns true if no path has been set in RequestLine object, false otherwise
		bool empty() const { return _path.empty(); }

		// Print on stdout RequestLine object
		void print() const
		{
			std::cout << "------ REQUEST LINE ------\nmet: " << _method << ", path: |" << _path
				<< "|, query: |" << _query << "|\n";
		}


		/* ------------------------------------------------------------- */
		/* --------------- NON-MEMBER FUNCTION OVERLOADS --------------- */

		friend void swap(RequestLine& a, RequestLine& b)
		{
			std::swap(a._method, b._method);
			std::swap(a._path, b._path);
			std::swap(a._query, b._query);
		}
		
}; // class RequestLine


/* ------------------------------------------------------------- */
/* -------------------- OPERATOR OVERLOADS --------------------- */

// Print the RequestLine method, path, and query if there is one
inline std::ostream& operator<<(std::ostream& stream, const RequestLine& reqLine)
{
	stream << "Method = " << reqLine.getMethod() << ", path = |" << reqLine.getPath() << "|";
	
	if (!reqLine.getQuery().empty())
		stream << ", query = |" << reqLine.getQuery() << "|";
		
	return stream;
}

#endif