/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:50:51 by tlorne            #+#    #+#             */
/*   Updated: 2024/05/17 14:58:43 by motoko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "connection.hpp"
#include "request.hpp"

Connection::Connection(int client_fd, std::string client_ip, int client_port)
{
    this->_fd = client_fd;
    this->_client_ip = client_ip;
    this->_client_port = client_port;
}

Connection::~Connection() {
}

/* == setter == */
void	Connection::setRequest(Request *new_request) {
	this->_request = new_request;	
}

/* == getter == */
Request*	Connection::getRequest() const {
	return (this->_request);
}

int Connection::getFd() const
{
    return (this->_fd);
}

std::string	Connection::getClientIp() const
{
	return (this->_client_ip);
}

int	Connection::getClientPort() const
{
	return (this->_client_port);
}
