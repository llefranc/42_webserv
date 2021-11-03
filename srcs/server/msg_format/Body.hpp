/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Body.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heleneherin <heleneherin@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/06 12:32:36 by lucaslefran       #+#    #+#             */
/*   Updated: 2021/06/14 09:59:48 by heleneherin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BODY_HPP
#define BODY_HPP

#include "../../includes/webserv.hpp"

class Body
{
	private:

		/* ------------------------------------------------------------- */
		/* ------------------------- ATTRIBUTES ------------------------ */
		
			bool								_recv;		// Indicates when request line + headers have been received
			size_t								_size;		// Content-length size
			long								_maxSize;	// Max octets that Body object can receive
			std::string							_buff;		// Buffer containing the payload


	public:

		/* ------------------------------------------------------------- */
		/* ------------------------ COPLIEN FORM ----------------------- */

		Body() : _recv(false), _size(), _maxSize(-1) {}
		Body(const Body& c) :
				_recv(c._recv), _size(c._size), _maxSize(c._maxSize), _buff(c._buff) {}
		~Body() {}
		Body& operator=(Body a)
		{
			swap(*this, a);
			return *this;
		}
		
		
		/* ------------------------------------------------------------- */
		/* --------------------------- GETTERS ------------------------- */

		const std::string& getBody() const { return _buff; }
		size_t getSize() const { return _size; }
		size_t getMaxSize() const { return _maxSize; }


		/* ------------------------------------------------------------- */
		/* --------------------------- SETTERS ------------------------- */
		
		void setSize(size_t size) { _size = size; }
		void setMaxSize(long maxSize) { _maxSize = maxSize; }
		void setBuff(std::string const &buf) { _buff = buf; }
		void startReceiving() { _recv = true; }


		/* ------------------------------------------------------------- */
		/* --------------------------- METHODS ------------------------- */

		// Returns true or false depending if request line + all headers have been received
		bool isReceiving() const { return _recv; }
		
		// Reset the Body object
		void clear()
		{
			_recv = false;
			_size = 0;
			_buff.clear();
		}
		
		// Append buffer received from client until content-length octets have been received.
		int recvBuffer(const std::string& buffer, size_t index, size_t lenToRead)
		{
			// maxSize will be evaluated only if previously set using setMaxSize before, because 
			// default constructor initilized it to -1
			if (_maxSize >= 0 && (_maxSize -= lenToRead) < 0)
				return -1;

			_buff.append(buffer, index, _size);
			_size -= lenToRead;
			return 0;
		}
		
	
		/* ------------------------------------------------------------- */
		/* --------------- NON-MEMBER FUNCTION OVERLOADS --------------- */

		friend void swap(Body& a, Body& b)
		{
			std::swap(a._recv, b._recv);
			std::swap(a._size, b._size);
			std::swap(a._maxSize, b._maxSize);
			std::swap(a._buff, b._buff);
		}
		
}; // class Body

#endif