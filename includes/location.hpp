/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:50:34 by tlorne            #+#    #+#             */
/*   Updated: 2024/05/12 19:26:00 by motoko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <map>

#include "webserv_macro.hpp"

class Location
{
	public:
        Location();
		Location(std::string location);
        ~Location();

		void	fullFillLocation(std::vector<std::string> lines);
		void	handleFirstLine(std::string line);
		void	handleBody(std::string line);

        /* == getter == */
		std::string					getRootPath() const;
        std::string 				getOpt() const;
        std::string 				getLocMatcUhri() const;
		std::vector<std::string>	getAllowMethod() const;
        std::vector<std::string>    getInfo(std::string key);
        int		getIsMulti() const;
        void    showLocation();

		//	getUri() const;
		//std::string	getAuthBasicRealm() const;
        //std::map<std::string , std::string> getAurhBasicFile() const;
        //    getIndex() const;
        //    getCgi() const;
        //bool    getAutoindex() const;
		
	private:
        int 		_is_multi;
        std::string _opt_modif;
        std::string _loc_match_uri;
        std::string _root_path;
        std::vector<std::string>    _allow_method;
        std::multimap<std::string , std::string> _body_file;

		//_uri;
        //_index;
        //_cgi;
        //bool    _autoindex;
        //std::string _auth_basic_realm;
};

#endif
