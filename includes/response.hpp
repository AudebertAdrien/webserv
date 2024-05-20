/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:51:31 by tlorne            #+#    #+#             */
/*   Updated: 2024/04/22 12:51:32 by tlorne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <iostream>
#include <map>
#include "connection.hpp"
#include "server.hpp"
#include "location.hpp"

class Connection;
class Server;
class Location;

class Response
{
	public:
		Response(Connection &connection, Server &server);
        ~Response();

		void	generateResp(std::string fp, std::string rel);
		void	sendResp(int fd);
		/* Connection	getConnection() const;
		int	getStatusCode() const;
		std::string	getStatusDescritpion() const;
		map<std::string , std::string>	getHearders() const;
		Transfert_type	getTransferType() const;
		std::string	getContent() const;
		void	addHeader(std::string header_key, std::string header_value);
		c_str() ??
		make_status ?? */
		

		
	private:
		Connection	*_connection;
		Server		*_server;
		std::string	_header;
		std::string	_body;
		std::string	_response;
        /* enum    Transfert_type;
		std::map<int, vector<string>>	_status
		Connection	_connection;
		int	_status_code;
		std::string	_status_description;
		std::map<std::string , std::string>	_headers;
		Transfert_type	_transfer_type;
		std::string	_content; */

};

#endif

