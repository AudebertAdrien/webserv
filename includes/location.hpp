/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:50:34 by tlorne            #+#    #+#             */
/*   Updated: 2024/04/22 12:50:35 by tlorne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

#include <iostream>
#include <vector>
//#include <map>

class Location
{
	public:
		Location(std::string location);
        ~Location();
		//	getUri() const;
		std::string	getRootPath() const;
		std::vector<std::string>	getAllowMethod() const;
		//std::string	getAuthBasicRealm() const;
        //std::map<std::string , std::string> getAurhBasicFile() const;
        //    getIndex() const;
        //    getCgi() const;
        //bool    getAutoindex() const;

		
	private:
		//_uri;
        std::string _root_path;
        std::vector<std::string>    _allow_method;
        //std::string _auth_basic_realm;
        //std::map<std::string , std::string> _auth_basic_file;
        //_index;
        //_cgi;
        //bool    _autoindex;


};

#endif
