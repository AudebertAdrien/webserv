/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:50:58 by tlorne            #+#    #+#             */
/*   Updated: 2024/05/20 15:59:59 by motoko           ###   ########.fr       */
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

#define REQ_BUFFER_SIZE 500

class Connection
{
	public:
		Connection(int client_fd, std::string client_ip, int client_port, Server &server);
        ~Connection();

		void	recvRequest();
		void	solveRequest();

		bool	sendData(const std::string& data);
		void	handleBytesReceived(int bytes_received);
		bool	parseStartLine();
		bool	parseHeader();
		bool	parseBody();
		bool	findHeaderEnd();
		void	processRequestLine();
		void	processHeaders();
		void	clearBuffer();


		/* == setter == */
		void			setRequest(Request *new_request);
		void			setRespons(Response *new_respons);

		/* == getter == */
		Request*		getRequest() const;
		Response*		getRespons() const;

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

		char		_buffer[1000];
};

#endif
