/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:51:07 by tlorne            #+#    #+#             */
/*   Updated: 2024/04/22 12:51:09 by tlorne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <iostream>
#include <map>
#include "Connection.hpp"
#include "Server.hpp"
#include "Location.hpp"

class Connection;
class Server;
class Location;

class Request
{
	public:
		Request(Connection connection, Server server, start);
        ~Request();
		Connection	getConnection() const;
		Method	getmethod() const;
		Server	getServer() const;
		Location	getLocation() const;
		std::string	getUri() const;
		Uri_type getUriType() const;
		map<std::string , std::string>	getHearders() const;
		Transfert_type	getTransferType() const;
		std::string	getContent() const;
		std::string	getOrigin() const;
		isOverTime();
		void	addContent(std::string content);
		void	addOrigin(std::string origin);
		void	addHeader(std::string header);
		bool	isValidHeader(std::string header);

		
	private:
		enum    Method;
        enum    Uri_type;
        enum    Transfert_type;
		Connection	_connection;
		Server	_server;
		Location	_location;
			_start;
		Method	_method;
		std::string	_uri;
		Uri_type	_uri_type;
		std::map<std::string , std::string>	_headers;
		Transfert_type	_transfer_type;
		std::string	_content;
		std::string	_origin;

};

#endif
