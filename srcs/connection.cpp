/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:50:51 by tlorne            #+#    #+#             */
/*   Updated: 2024/05/20 16:01:43 by motoko           ###   ########.fr       */
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

bool	Connection::parseStartLine() {
	std::cout << "parseStartLine" << std::endl;
	std::string http_request = _buffer;

	size_t header_end = http_request.find("\r\n\r\n");

	std::cout << "Header end toto : " << header_end << std::endl;
	if (header_end == std::string::npos) {
		std::cout << "Header end not found 1" << std::endl;
	}
	std::string header = http_request.substr(0, header_end);

	std::istringstream	iss(header);
	std::string first_line;
	std::getline(iss, first_line);

	std::cout << RED << first_line << RESET << std::endl;
	this->_request->addMethod(header);
	this->_request->setPhase(Request::ON_HEADER);
	return true;
}

bool	Connection::parseHeader() {
	std::cout << "parseHeader" << std::endl;
	std::string http_request = _buffer;

	size_t header_end = http_request.find("\r\n\r\n");
	if (header_end == std::string::npos) {
		std::cout << "Header end not found 2" << std::endl;
	}

	std::string header = http_request.substr(0, header_end);

	std::istringstream	iss(header);
	std::string line;

	std::getline(iss, line);
	while (std::getline(iss, line)) {
		this->_request->addHeader(line);
	}

	std::cout << "this->_request._header[Content-Length]" << ft::findKeyInMap(this->_request->getHeader(), "Content-Length") << std::endl; 

	if (ft::findKeyInMap(this->_request->getHeader(), "Content-Length")) {
		std::cout << "!= npos : " << header_end << std::endl;
		std::string body = http_request.substr(header_end + 4);
		this->_request->addContent(body);
		_buffer[0] = 0;
		this->_request->setPhase(Request::ON_BODY);
	}
	return true;
}

bool	Connection::parseBody() {
	std::cout << "parseBody" << std::endl;
	std::string rest(_buffer);

	_request->addContent(rest);
	return true;
}

void	Connection::recvRequest() {
	this->_request = new Request(*this, *this->_server);			

	int bytes_received = 1;
	while (bytes_received > 0) {

		//bytes_received = recv(this->_fd, buffer, sizeof(buffer), MSG_DONTWAIT);
		bytes_received = recv(this->_fd, this->_buffer, sizeof(this->_buffer), 0);
		std::cout << "FUCK THAT BYTES: " << bytes_received << std::endl;
		if (bytes_received == -1) {
			std::cerr << "Error in receiving data ######" << std::endl;
			break;
		} else if (bytes_received == 0) {
			std::cerr << "Connection close" << std::endl;
			break;
		} else {
			this->_buffer[bytes_received] = '\0';

			if (this->_request->getPhase() == Request::READY) {
				parseStartLine();
			}

			if (this->_request->getPhase() == Request::ON_HEADER) {
 				parseHeader();
			}

			if (this->_request->getPhase() == Request::ON_BODY) {
				parseBody();
			}
		}
	}
} 

/*
void	Connection::recvRequest() {	
	Request	*request = new Request(*this, *this->_server);			

	int bytes_received = 1;
	while (bytes_received > 0) {
		char buffer[500];

		std::cout << "recvRequest" << std::endl;
		bytes_received = recv(this->_fd, buffer, sizeof(buffer), 0);
		std::cout << "bytes_received: " << bytes_received << std::endl;
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
*/

bool Connection::sendData(const std::string& data) {
	size_t totalBytesSent = 0;
	size_t dataLength = data.length();
	const char* dataPtr = data.c_str();

	while (totalBytesSent < dataLength) {
		ssize_t bytesSent = send(this->_fd, dataPtr + totalBytesSent, dataLength - totalBytesSent, MSG_NOSIGNAL);
		if (bytesSent == -1) {
			std::cerr << "Failed to send data: " << strerror(errno) << std::endl;
			return false;
		}
		std::cout << "bytesSent: " << bytesSent << std::endl;
		totalBytesSent += bytesSent;
		std::cout << "totalBytesSent: " << totalBytesSent << std::endl;
	}

    return true;
}

void	Connection::initiateResponse(Location &loc)
{
	Response	*rep = new Response(*this, *this->_server);
	setRespons(rep);
	std::string file_path = createFilePath(loc.getRootPath(), this->_request->getRelativPath());
	std::cout << GREEN << file_path << RESET << std::endl;
	this->_response->generateResp(file_path, this->_request->getRelativPath());
	this->_response->sendResp(this->_fd);
	/* //std::string header = "HTTP/1.1 200 OK\r\nConnection: Keep-Alive\r\n";
	std::string header = "HTTP/1.1 200 OK\r\n";
	std::string body = generateResponse(file_path, this->_request->getRelativPath());
	std::ostringstream oss;
	oss << header << body;
	std::string response = oss.str();
	//std::cout << RED <<oss.str() << RESET << std::endl;
	//if (send(this->_fd, response.c_str(), response.length(), MSG_NOSIGNAL) == -1)
	//	std::cerr << "Send failed: " << strerror(errno) << std::endl;
	sendData(response); */

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

