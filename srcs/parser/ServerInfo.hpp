/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerInfo.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heleneherin <heleneherin@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/09 15:42:16 by hherin            #+#    #+#             */
/*   Updated: 2021/06/08 16:42:42 by heleneherin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERINFO_HPP
# define SERVERINFO_HPP

# include <vector>
# include <map>
# include <string>
# include <cstring>
# include <algorithm>
# include "../utils/utils.hpp"
# include "LocationObj.hpp"

class Location; 

class ServerInfo
{	
	private:
		/*	===========================================
			================= Variables ===============
		*/
		int _port;
		std::string _error_path;
		std::vector<std::string> _names;
		std::string _host;
		int _max_cli_body;
		std::map<std::string, Location> _location;
		

	public:
		/*	===========================================
			=============== Coplien Form ==============
		*/
		ServerInfo();
		ServerInfo(ServerInfo const &);
		ServerInfo &operator=(ServerInfo const &);
		~ServerInfo();
		
		/*	===========================================
			================= GETTERS =================
		*/
		int const &getMaxClientsBS() const;	
		std::string const &getError() const;
		std::string const &getHost() const;
		std::vector<std::string> const &getNames() const;
		int const &getPort() const;
		std::map<std::string, Location> const *getLocation() const;
		
		/*	===========================================
			================= SETTERS =================
		*/
		void setServer(int, int const&, std::string const&);
		std::map<std::string, Location> *setSrvLocation();
		
	private:
		void setMaxClientsBS(char const*);
		void setError(char const*);
		void setNames(char const*);
		void setPort(char const*);
		void setHost(char const *);
		
		/*	===========================================
			============ Private methods ==============
		*/
		template <class T>
		void myswap(T t1, T t2);
};


#endif