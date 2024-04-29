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
#include <map>

class Location
{
	public:
		Location(location_block ?);
        ~Location();
			getUri() const;
		std::string	getRootPath() const;
			getAllowMethod() const;
		std::string	getAuthBasicRealm() const;
        map<std::string , std::string> getAurhBasicFile() const;
            getIndex() const;
            getCgi() const;
        bool    getAutoindex() const;

		
	private:
		_uri;
        std::string _root_path;
        allow_method;
        std::string _auth_basic_realm;
        std::map<std::string , std::string> _auth_basic_file;
        _index;
        _cgi;
        bool    _autoindex;


};

#endif
