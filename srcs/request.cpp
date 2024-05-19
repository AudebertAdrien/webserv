/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:51:12 by tlorne            #+#    #+#             */
/*   Updated: 2024/05/19 18:03:26 by motoko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"
#include "server.hpp"
#include "connection.hpp"

Request::Request(Connection &connection, Server &server) {
	this->_connection = &connection;
	this->_server = &server;
	this->_phase = Request::READY;
}

Request::~Request() {
}

void	Request::addHeader(std::string &line) {
	std::string key, value;

	size_t pos = line.find(":");
	if (pos != std::string::npos) {
		std::string key = line.substr(0, pos);
		std::string value = line.substr(pos + 1);

		this->_headers.insert(std::make_pair(key, value));
	}
}

void	Request::addMethod(std::string &line) {
	std::istringstream iss(line);
	std::string	method;

	iss >> method;
	if (method == "GET")
		this->_method = GET;
	if (method == "POST")
		this->_method = POST;
	iss >> this->_relativ_path;
}

void	Request::addContent(std::string &content) {
	this->_content += content;	
}

void	Request::setPhase(Phase new_phase) {
	this->_phase = new_phase;
}

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

Request::Phase	Request::getPhase() const
{
	return (_phase);
}

std::string		Request::getRelativPath() const
{
	return (this->_relativ_path);
}
