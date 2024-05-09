/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:51:07 by tlorne            #+#    #+#             */
/*   Updated: 2024/05/09 17:28:07 by motoko           ###   ########.fr       */
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

enum Method { DEFAULT, GET, HEAD, POST, PUT, DELETE, OPTIONS, TRACE };

class Request
{
	public:
		Request(Connection &connection, Server &server);
        ~Request();

		Connection		getConnection() const;
		Server			getServer() const;

		/* == getter == */
		std::map<std::string, std::string>	getHeader() const;
		Method								getMethod() const;
		std::string							getContent() const;
		/*
		Location		getLocation() const;
		std::string		getUri() const;
		Uri_type		getUriType() const;


		Transfert_type	getTransferType() const;
		std::string		getOrigin() const;

		//isOverTime();

		void	addOrigin(std::string origin);
		bool	isValidHeader(std::string header);
		*/
		
		/*== setter ==*/
		void	addMethod(std::string &line);
		void	addHeader(std::string &line);
		void	addContent(std::string &content);

	private:
		//enum    Method;
		/*
        enum    Uri_type;
        enum    Transfert_type;
		*/

		Connection		*_connection;
		Server			*_server;
		//Location		_location;
		
		Method			_method;

		std::map<std::string, std::string> _headers;
		std::string		_content;

		/*
		std::string		_uri;
		//Uri_type		_uri_type;


		//Transfert_type	_transfer_type;
		std::string		_origin;

		//_start;
		*/
};

#endif
