/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:50:58 by tlorne            #+#    #+#             */
/*   Updated: 2024/05/13 12:56:18 by motoko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP
# define CONNECTION_HPP

#include <iostream>
#include <map>

#include "webserv_macro.hpp"

class Request;

class Connection
{
	public:
		Connection();
		Connection(int client_fd, std::string client_ip, int client_port);
        ~Connection();

		/* == setter == */
		void			setRequest(Request *new_request);

		/* == getter == */
		Request*		getRequest() const;

		int				getFd() const;
		timeval			getLastRequest() const;
		std::string		getClientIp() const;
		int				getClientPort() const;

		
	private:
		Request		*_request;

		int 		_fd;
        timeval 	_last_request;
        std::string	_client_ip;
        int			_client_port;
};

#endif
