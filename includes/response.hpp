/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:51:31 by tlorne            #+#    #+#             */
/*   Updated: 2024/05/27 16:47:36 by motoko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <iostream>
#include <map>
#include <sys/wait.h>

#include "connection.hpp"
#include "server.hpp"
#include "location.hpp"
#include "request.hpp"
#include "utils.hpp"

class Connection;
class Server;
class Location;

class Response
{
	public:
		Response(Connection &connection, Server &server);
        ~Response();

		void	createResponse(std::string fp, std::string status);
		void	sendResponse(int fd);

		void	handleCGI(std::string fp, int client_fd);
		void	execCGI(int client_fd, std::string method, std::string path, std::string param);
		void	listOfDirectory(std::string fp);

		/* == getter == */
		Connection*		getConnection() const;

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

