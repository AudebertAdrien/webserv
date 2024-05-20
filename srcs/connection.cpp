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
#include "server.hpp"
#include "response.hpp"

Connection::Connection(int client_fd, std::string client_ip, int client_port, Server &server)
{
    this->_fd = client_fd;
    this->_client_ip = client_ip;
    this->_client_port = client_port;
    this->_server = &server;
}

Connection::~Connection() {
}

/* == setter == */
void	Connection::setRequest(Request *new_request) {
	this->_request = new_request;	
}

void	Connection::setRespons(Response *new_respons) {
	this->_response = new_respons;	
}

/* == getter == */
Request*	Connection::getRequest() const {
	return (this->_request);
}

Response*	Connection::getRespons() const{
	return (this->_response);
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

void	Connection::recvRequest() {
	Request	*request = new Request(*this, *this->_server);			

	int bytes_received = 0;
	while (!bytes_received) {
		char buffer[1200];

		bytes_received = recv(this->_fd, buffer, sizeof(buffer), 0);
		if (bytes_received == -1) {
			std::cerr << "Error in receiving data ######" << std::endl;
			break ;
		} else {
			std::string http_request(buffer);

			size_t header_end = http_request.find("\r\n\r\n");
			if (header_end == std::string::npos) {
				std::cout << "header end not found" << std::endl;
			}
			std::string header = http_request.substr(0, header_end);
			std::string body = http_request.substr(header_end + 4);

			std::istringstream	iss(header);
			std::string line;
			bool isFirstLine = true;

			while (getline(iss, line)) {
				if (isFirstLine) {
					isFirstLine = false;
					std::cout << RED << line << RESET << std::endl;
					request->addMethod(line);
				} else {
					request->addHeader(line);
				}
			}
			if (body.length())
				request->addContent(body);
			
			std::cout << YELLOW << request->getContent() << RESET << std::endl;

			std::cout << "############" << std::endl;
		}
	}
	setRequest(request);
}

void	Connection::initiateResponse(Location &loc)
{
	Response	*rep = new Response(*this, *this->_server);
	setRespons(rep);
	std::string file_path = createFilePath(loc.getRootPath(), this->_request->getRelativPath());
	std::cout << GREEN << file_path << RESET << std::endl;
	this->_response->generateResp(file_path, this->_request->getRelativPath());
	this->_response->sendResp(this->_fd);
}

void	Connection::closestMatch()
{
	int	index = findClosestStringIndex(this->_request->getRelativPath(), this->_server->getLocation());
	std::cout << RED << "index vaut = " << RESET << index << std::endl;

	if (index != -1)
		initiateResponse(*this->_server->getLocation()[index]);
	else
	{
		std::cout << GREEN <<"####### solveRequest ######" << RESET << std::endl;
		std::string header = "HTTP/1.1 200 OK\r\n";
		std::string body = "Hello from server!!! Here Adrien\n";
		std::ostringstream oss;
		oss << header << "Content-Length: " << body.length() << "\r\n\r\n" << body;
		std::string response = oss.str();
		if (send(this->_fd, response.c_str(), response.length(), 0) == -1)
			std::cerr << "Send failed: " << strerror(errno) << std::endl;
	}
}

void	Connection::solveRequest() {
	ft::displayMap(this->_request->getHeader());
	std::cout << this->_request->getContent() << std::endl;

    std::vector<Location *>::iterator	it = this->_server->getLocation().begin();

    while (it != this->_server->getLocation().end())
	{
		if (this->_request->getRelativPath() == (*it)->getLocMatcUhri())
		{
            //this->_response = new Response();
            if (this->_request->getMethod() == GET)
            {
                std::cout << "==GET==" << std::endl;
                initiateResponse(*(*it));
            }
            if (this->_request->getMethod() == POST)
            {
                std::cout << "==POST==" << std::endl;
                std::cout << GREEN <<"####### solveRequest ######" << RESET << std::endl;
                std::string header = "HTTP/1.1 200 OK\r\n";
                std::string body = "Hello from server!!! Here Adrien\n";
                std::ostringstream oss;
                oss << header << "Content-Length: " << body.length() << "\r\n\r\n" << body;
                std::string response = oss.str();
                if (send(this->_fd, response.c_str(), response.length(), 0) == -1)
                std::cerr << "Send failed: " << strerror(errno) << std::endl;

            }
            return ;
		}
		it++;
	}
    //std::cout << "pas de match parfait" << std::endl;
	closestMatch();
}

