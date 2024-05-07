/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:51:07 by tlorne            #+#    #+#             */
/*   Updated: 2024/05/07 15:05:09 by motoko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <iostream>
#include <map>
#include "connection.hpp"
#include "server.hpp"
#include "location.hpp"

class Connection;
class Server;
class Location;

class Request
{
	public:
		Request(Connection &connection, Server &server);
        ~Request();

		Connection		getConnection() const;
		Server			getServer() const;

		/*
		Location		getLocation() const;
		Method			getMethod() const;
		std::string		getUri() const;
		Uri_type		getUriType() const;

		map<std::string , std::string>	getHearders() const;

		Transfert_type	getTransferType() const;
		std::string		getContent() const;
		std::string		getOrigin() const;

		//isOverTime();

		void	addContent(std::string content);
		void	addOrigin(std::string origin);
		void	addHeader(std::string header);
		bool	isValidHeader(std::string header);
		*/
		
	private:
		/*
		enum    Method;
        enum    Uri_type;
        enum    Transfert_type;
		*/

		Connection		*_connection;
		Server			*_server;
		//Location		_location;
		
		/*
		//Method			_method;
		std::string		_uri;
		//Uri_type		_uri_type;

		std::map<std::string , std::string>	_headers;

		//Transfert_type	_transfer_type;
		std::string		_content;
		std::string		_origin;

		//_start;
		*/
};

#endif
