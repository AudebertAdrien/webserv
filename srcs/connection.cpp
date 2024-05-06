/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:50:51 by tlorne            #+#    #+#             */
/*   Updated: 2024/04/22 12:50:53 by tlorne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "connection.hpp"
#include "webserv_macro.hpp"

Connection::Connection(int client_fd, std::string client_ip, int client_port)
{
    std::cout << "Connection, constructeur called" << std::endl;

    this->_fd = client_fd;
    this->_client_ip = client_ip;
    this->_client_port = client_port;
}

Connection::~Connection()
{
    std::cout << "Connection, destructeur called" << std::endl;
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