/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationObj.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucaslefrancq <lucaslefrancq@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 15:08:11 by hherin            #+#    #+#             */
/*   Updated: 2021/06/21 16:32:33 by lucaslefran      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONOBJ_HPP
# define LOCATIONOBJ_HPP

# include <string>
# include <vector>
# include <map>
# include "ServerInfo.hpp"
# include "../utils/utils.hpp"
# include "parser.hpp"

class ServerInfo;

class Location
{
	private:
		/*	===========================================
			================= Variables ===============
		*/
		int _port;
		bool _autoindex;
		std::string _root;
		std::string _upload_path;
		std::string _redirect;
		std::vector<std::string> _allow_methd;
		std::vector<std::string> _index;
		std::map<std::string, std::string> _cgi_exe;
		
	public:
		/*	===========================================
			=============== Coplien Form ==============
		*/
		
		Location(int);
		Location(Location const &);
		Location &operator=(Location const &);
		~Location();

		/*	===========================================
			================= GETTERS =================
		*/

        int const &getPort() const;
		std::string const &getRoot() const;
		std::string const &getUploadPath() const;
		std::vector<std::string> const &getMethods() const;
		std::vector<std::string> const &getIndex() const;
		std::map<std::string, std::string> const &getCgiExe() const;
		std::string const &getRedirect() const;
		bool const &getAutoIndex() const;
		
		/*	===========================================
			================= SETTERS =================
		*/
		void	setLocation(int nb, int const &pos, std::string const &buf);
	
		/*	===========================================
			================= METHODS =================
		*/

		void	printLocation(const std::string& locName) const;


	private:	
		void setRoot(char const*);
		void setUploadStore(char const*);
		void setMethods(char const*);
		void setIndex(char const*);
		void setCgiExe(char const *);
		void setAutoIndex(char const *);
		void setRedirect(char const *);
		
		/*	===========================================
			============ Private methods ==============
		*/
		template <class T>
		void myswap(T t1, T t2);
};


#endif