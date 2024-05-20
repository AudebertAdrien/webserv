/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:51:25 by tlorne            #+#    #+#             */
/*   Updated: 2024/04/22 12:51:27 by tlorne           ###   ########.fr       */
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
        handleCgi(fp, this->_connection->getFd());
    }
	this->_body = generateResponse(fp, rel);
	std::ostringstream oss;
	oss << this->_header << this->_body;
	this->_response = oss.str();
 }

 void   Response::sendResp(int fd)
 {
    if (send(fd, this->_response.c_str(), this->_response.length(), MSG_NOSIGNAL) == -1)
		std::cerr << "Send failed: " << strerror(errno) << std::endl;
 }

 /* == getter == */
Connection*		Response::getConnection() const
{
    return (this->_connection);
}
