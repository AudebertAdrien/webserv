/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:50:58 by tlorne            #+#    #+#             */
/*   Updated: 2024/05/22 14:43:48 by motoko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP
# define CONNECTION_HPP

#include <iostream>
#include <map>
#include <sys/socket.h>

#include "libft.hpp"
#include "utils.hpp"
#include "webserv_macro.hpp"

class Request;
class Server;
class Response;

#define REQ_BUFFER_SIZE 500

class Connection
{
	public:
		Connection(int client_fd, std::string client_ip, int client_port, Server &server);
        ~Connection();

		void	recvRequest();
		void	solveRequest();

		bool	parseStartLine();
		bool	parseHeader();
		bool	parseBody();

		/* == getter == */
		Request*		getRequest() const;
		Response*		getRespons() const;

		int				getFd() const;
		timeval			getLastRequest() const;
		std::string		getClientIp() const;
		int				getClientPort() const;
		
	private:
		void			initiateResponse(Location &loc);
		void			closestMatch();

		Server		*_server;
		Request		*_request;
		Response 	*_response;

		int 		_fd;
        std::string	_client_ip;
        int			_client_port;
        timeval 	_last_request;

		char		_buffer[1000];
};

#endif
