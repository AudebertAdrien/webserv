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
#include <string>
#include <sstream>
#include <map>

class Location
{
	public:
        Location();
		Location(std::string location);
        ~Location();
		//	getUri() const;
		std::string	getRootPath() const;
		std::vector<std::string>	getAllowMethod() const;
		void	fullFillLocation(std::vector<std::string> lines);
		void	handleFirstLine(std::string line);
		void	handleBody(std::string line);

        //####### GETTER ########

        std::vector<std::string>    getInfo(std::string key);
        std::string getOpt() const;
        std::string getLocMatcUhri() const;
        int getIsMulti() const;
        // Useless fonction, just to show location
        void    showLocation();
        // to erease 
		//std::string	getAuthBasicRealm() const;
        //std::map<std::string , std::string> getAurhBasicFile() const;
        //    getIndex() const;
        //    getCgi() const;
        //bool    getAutoindex() const;

		
	private:
		//_uri;
        std::string _opt_modif;
        std::string _loc_match_uri;
        std::string _root_path;
        std::vector<std::string>    _allow_method;
        //std::string _auth_basic_realm;
        std::multimap<std::string , std::string> _body_file;
        int _is_multi;
        //_index;
        //_cgi;
        //bool    _autoindex;


};

#endif
