/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:51:07 by tlorne            #+#    #+#             */
/*   Updated: 2024/05/22 14:17:19 by motoko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <iostream>
#include <map>

#include "location.hpp"

class Server;
class Connection;

#define MAX_BUFFER_SIZE 1024

class Request
{
	public:
		enum Phase { READY, ON_HEADER, ON_BODY, COMPLETE };

		Request(Connection &connection, Server &server);
        ~Request();

		Connection		getConnection() const;
		Server			getServer() const;

		/* == getter == */
		std::map<std::string, std::string>	getHeader() const;
		Method			getMethod() const;
		std::string		getContent() const;
		std::string		getRelativPath() const;
		Location		getLocation() const;
		std::string		getUri() const;
		URIType			getUriType() const;
		TransferType	getTransferType() const;
		Request::Phase	getPhase() const;

		/*== setter ==*/
		void	addMethod(std::string &line);
		void	addHeader(std::string &line);
		void	addContent(std::string &content);

		void	setPhase(Phase new_phase);

	private:
		Connection		*_connection;
		Server			*_server;
		Location		*_location;

		Method			_method;
		URIType			_uri_type;
		TransferType	_transfer_type;
		Phase			_phase;

		std::map<std::string, std::string> _headers;
		std::string		_relativ_path;
		std::string		_content;

		std::string		_uri;
		std::string		_origin;
};

#endif
