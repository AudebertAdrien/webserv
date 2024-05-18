/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:50:58 by tlorne            #+#    #+#             */
/*   Updated: 2024/05/13 16:37:04 by motoko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP
# define CONNECTION_HPP

#include <iostream>
#include <map>
#include <sys/socket.h>

#include "webserv_macro.hpp"
#include "libft.hpp"
#include "utils.hpp"

class Request;
class Server;
class Response;

class Connection
{
	public:
		Connection(int client_fd, std::string client_ip, int client_port, Server &server);
        ~Connection();
		void	recvRequest();
		void	solveRequest();

		/* == setter == */
		void			setRequest(Request *new_request);

		/* == getter == */
		Request*		getRequest() const;

		int				getFd() const;
		timeval			getLastRequest() const;
		std::string		getClientIp() const;
		int				getClientPort() const;
		
	private:
		void	initiateResponse(Location &loc);
		void	closestMatch();

		Server		*_server;
		Request		*_request;
		Response 	*_response;

		int 		_fd;
        std::string	_client_ip;
        int			_client_port;
        timeval 	_last_request;
};

#endif
