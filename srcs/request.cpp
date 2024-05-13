/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:51:12 by tlorne            #+#    #+#             */
/*   Updated: 2024/05/12 19:38:17 by motoko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"
#include "server.hpp"
#include "connection.hpp"

Request::Request(Connection &connection, Server &server)
{
    //std::cout << "Request constructeur with params called : " << server.getFd() << std::endl;
	this->_connection = &connection;
	this->_server = &server;
	this->_phase = READY;
}

Request::~Request()
{
    //std::cout << "Request destructeur called" << std::endl;
}

void	Request::addMethod(std::string &line) {
	std::string	method;
	size_t pos = line.find_first_of(" \t");

	if (pos != std::string::npos) {
		method = line.substr(0, pos);
	}
	if (method == "GET")
		this->_method = GET;
	if (method == "POST")
		this->_method = POST;
}

void	Request::addHeader(std::string &line) {
	std::string key, value;
	size_t pos = line.find_first_of(" \t");
	std::cout << GREEN << line << RESET << std::endl;

	if (pos != std::string::npos) {
		key = line.substr(0, pos);
		value = line.substr(pos + 1);
	}
	this->_headers.insert(std::make_pair(key, value));
}

void	Request::addContent(std::string &content) {
	this->_content = content;	
}

void	Request::setPhase(Phase new_phase) {
	this->_phase = new_phase;
}

/*
void	addOrigin(std::string origin) {
}
*/

/*
void	isValidHeader(std::string header) {
}
*/

/* == getter */
std::map<std::string, std::string>	Request::getHeader() const
{
	return (_headers);
}

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

std::string	Request::getContent() const
{
	return (_content);
}

Location	Request::getLocation() const
{
	return (*_location);
}

std::string	Request::getUri() const
{
	return (_uri);
}

URIType	Request::getUriType() const
{
	return (_uri_type);
}

TransferType	Request::getTransferType() const
{
	return (_transfer_type);
}

std::string	Request::getOrigin() const
{
	return (_origin);
}

Request::Phase	Request::getPhase() const
{
	return (_phase);
}
