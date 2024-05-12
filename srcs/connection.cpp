/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:50:51 by tlorne            #+#    #+#             */
/*   Updated: 2024/05/12 18:17:42 by motoko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "connection.hpp"

Connection::Connection() {
    //std::cout << "Connection default constructor" << std::endl;
}

Connection::Connection(int client_fd, std::string client_ip, int client_port)
{
    //std::cout << "Connection constructor with params called" << std::endl;
    this->_fd = client_fd;
    this->_client_ip = client_ip;
    this->_client_port = client_port;
}

Connection::~Connection()
{
    //std::cout << "Connection, destructeur called : " << this->getFd() << std::endl;
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
