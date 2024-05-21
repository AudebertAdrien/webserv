/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:51:25 by tlorne            #+#    #+#             */
/*   Updated: 2024/05/20 18:03:00 by motoko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"
#include "server.hpp"
#include "connection.hpp"

Response::Response(Connection &connection, Server &server)
{
	this->_connection = &connection;
	this->_server = &server;
}

Response::~Response()
{
}

void   Response::handleCgi(std::string fp, int client_fd)
{
	std::string script_path = fp;
	std::string query_string = "";

	if (fp.find("?") != std::string::npos) 
	{
		query_string = fp.substr(fp.find("?") + 1);
		script_path = fp.substr(0, script_path.find("?"));
	}
}

void   Response::generateResp(std::string fp, std::string rel)
{
	this->_header = "HTTP/1.1 200 OK\r\n";
	if (fp.find("cgi") != std::string::npos)
	{
		std::cout << "cgi part has to be handle : " << RESET << std::endl;
		//handleCgi(fp, this->_connection->getFd());
	}
	this->_body = generateResponse(fp, rel);
	std::ostringstream oss;
	oss << this->_header << this->_body;
	this->_response = oss.str();
}

void   Response::sendResp(int fd)
{
	size_t totalBytesSent = 0;
	size_t dataLength = this->_response.length();
	const char* dataPtr = this->_response.c_str();

	while (totalBytesSent < dataLength) {
		ssize_t bytesSent = send(fd, dataPtr + totalBytesSent, dataLength - totalBytesSent, MSG_NOSIGNAL);
		if (bytesSent == -1) {
			std::cout << "bytesSent: " << std::endl;
		}
		totalBytesSent += bytesSent;
	}
}

/* == getter == */
Connection*		Response::getConnection() const
{
	return (this->_connection);
}
