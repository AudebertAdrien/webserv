/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:51:12 by tlorne            #+#    #+#             */
/*   Updated: 2024/05/08 17:21:31 by motoko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"
#include "webserv_macro.hpp"

Request::Request(Connection &connection, Server &server)
{
    std::cout << "Request constructeur called : " << server.getFd() << std::endl;
	this->_connection = &connection;
	this->_server = &server;
}

Request::~Request()
{
    std::cout << "Request destructeur called" << std::endl;
}

void	Request::addMethod(std::string &method) {
	if (method == "GET")
		_method = GET;
}

/*
void	addContent(std::string content) {
}

void	addOrigin(std::string origin) {
}

void	addHeader(std::string header) {
}

void	isValidHeader(std::string header) {
}
*/

Connection	Request::getConnection() const
{
	return (*_connection);
}

Server	Request::getServer() const
{
	return (*_server);
}

std::string	Request::getContent() const
{
	return (this->_content);
}

std::map<std::string, std::string>	Request::getHeaders() const
{
	return (this->_headers);
}

Method	Request::getMethod() const
{
	return (_method);
}

/*
Request Request::getLocation() const
{
}

Request Request::getUri() const
{
}

Request Request::getUriType() const
{
}

Request Request::getHeaders() const
{
}

Request Request::getTransferType() const
{
}

Request Request::getContent() const
{
}

Request Request::getOrigin() const
{
}
*/
