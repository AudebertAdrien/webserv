/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:51:12 by tlorne            #+#    #+#             */
/*   Updated: 2024/05/09 17:29:23 by motoko           ###   ########.fr       */
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

void	Request::addMethod(std::string &line) {
	std::string key, value;
	std::string	method;
	size_t pos = line.find_first_of(" \t");

	if (pos != std::string::npos) {
		method = line.substr(0, pos);
	}
	if (method == "GET")
		this->_method = GET;
}

void	Request::addHeader(std::string &line) {
	std::string key, value;
	size_t pos = line.find_first_of(" \t");

	if (pos != std::string::npos) {
		key = line.substr(0, pos);
		value = line.substr(pos + 1);
	}
	this->_headers.insert(std::make_pair(key, value));
}

void	Request::addContent(std::string &content) {
	this->_content = content;	
}


/*
void	addOrigin(std::string origin) {
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

Method	Request::getMethod() const
{
	return (_method);
}

std::map<std::string, std::string>	Request::getHeader() const
{
	return (_headers);
}

std::string	Request::getContent() const
{
	return (_content);
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


Request Request::getTransferType() const
{
}



Request Request::getOrigin() const
{
}
*/
