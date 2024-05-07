/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:50:12 by tlorne            #+#    #+#             */
/*   Updated: 2024/05/07 17:18:15 by motoko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "server_manager.hpp"
#include "webserv_macro.hpp"

#define PORT 8080
#define BUFFER_SIZE 1024
#define TIMEOUT_SEC 5

Server::Server()
{
    //std::cout << "Server default constructor" << std::endl;
}

Server::~Server() {
	//std::cout << "Server destructor" << std::endl;
}

void    Server::completeVectorLocation(std::vector<std::string> location_block)
{
    std::vector<std::string>::iterator it;
	for (it = location_block.begin(); it != location_block.end(); it++) {
        this->_location.push_back(new Location(*it));
    }
}

void	Server::completeServer(std::string server_block)
{
	std::istringstream iss(server_block);
    std::string word;

	iss >> word;
	while (word != "listen")
		iss >> word;
	iss >> word;
	word.resize(word.size() - 1);
	this->_port = atoi(word.c_str());
}

Server::Server(ServerManager &manager, std::string server_block, std::vector<std::string> location_block, Config  &config)
{
    //std::cout << "Server with params constructor" << std::endl;

    this->_config = &config;
	this->_manager = &manager;
	if ((_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
		std::cerr << "Socket creation failed: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	int opt = 1;
	if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
		std::cerr << "Setsockopt SO_REUSEADDR failed: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	completeServer(server_block);

	//ft::bzero(&server_addr, sizeof(struct sockaddr_in));
	_server_addr.sin_family          = AF_INET;
	_server_addr.sin_port            = htons(this->_port);
	_server_addr.sin_addr.s_addr     = htonl(INADDR_ANY);
    //server_addr.sin_addr.s_addr = INADDR_ANY;
	//server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

	if (bind(_fd, (struct sockaddr *)&_server_addr, sizeof(_server_addr)) == -1) {
		std::cerr << "Bind failed: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	if (listen(_fd, 256) == -1) {
		std::cerr << "Listen failed: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
 
	int flags = fcntl(_fd, F_GETFL, 0);
    if (flags == -1) {
        std::cerr << "Failed to get socket flags\n";
        close(_fd);
		exit(EXIT_FAILURE);
    }

	/*
	if (fcntl(_fd, F_SETFL, flags | O_NONBLOCK) == -1) {
		std::cerr << "Listen failed: " << strerror(errno) << std::endl;
        close(_fd);
		exit(EXIT_FAILURE);
	}
	*/
}

int	Server::getPort()
{
	return (this->_port);
}

int	Server::getFd()
{
	return (this->_fd);
}

void	Server::recvRequest(Connection &connection) {

	Request	test(connection, *this);			

	int bytes_received = 0;
	while (!bytes_received) {
		char buffer[1024];
		bytes_received = recv(connection.getFd(), buffer, sizeof(buffer), 0);
		if (bytes_received == -1) {
			std::cerr << "Error in receiving data ######" << std::endl;
		} else {
			buffer[bytes_received] = '\0';
			std::cout << "Received " << bytes_received << " bytes: \n" << buffer << std::endl;
		}
	}
}

void	Server::runRecvAndSolve(Connection &connection) {
	std::cout << "runRecvAndSolve" << std::endl;

	try {
		recvRequest(connection);
	} catch (std::exception &e) {
		std::cerr << "recvRequest error!!!" << std::endl;
	}

	std::string header = "HTTP/1.1 200 OK\r\n";
	std::string body = "Hello from server!!! Here Adrien\n";
	std::ostringstream oss;
	oss << header << "Content-Length: " << body.length() << "\r\n\r\n" << body;
	std::string response = oss.str();

	if (send(connection.getFd(), response.c_str(), response.length(), 0) == -1) {
		std::cerr << "Send failed: " << strerror(errno) << std::endl;
	}
}

void	Server::addConnection(int client_fd, std::string client_ip, int client_port) {
    Connection *client = new Connection(client_fd, client_ip, client_port);

	std::cout << "Ip : " << client_ip <<  std::endl << "Port : " << client_port << std::endl;
    _connections.insert(std::make_pair(client_fd, client));
}

void	Server::acceptNewConnection() {
	int	client_fd;
	struct sockaddr_in client_addr;

	socklen_t addrlen = sizeof(_server_addr);

	if ((client_fd = accept(_fd, (struct sockaddr *)&client_addr, (socklen_t*)&addrlen)) == -1) {
		std::cerr << "Accept failed: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	std::cout << "######################### RUN = " << "_fd1 :" << this->_fd << " port1 : " << this->_port << "########################" << std::endl;
	std::cout << "client_fd: " << client_fd << std::endl; 

	struct timeval timeout;
	timeout.tv_sec = TIMEOUT_SEC;
	timeout.tv_usec = 0;
	if (setsockopt(client_fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) == -1) {
		std::cerr << "Failed to set receive timeout" << std::endl;
		exit(EXIT_FAILURE);
	}

	char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
    int client_port = ntohs(client_addr.sin_port);

	addConnection(client_fd, client_ip, client_port);

}

void	Server::run() {

	//check connection possible
	std::cout << "size : " << this->_manager->getServer().size() << std::endl;
	if (this->_connections.size() >= (1024 / this->_manager->getServer().size()))
	{
		std::cout << "too many connection, old connection must be closed" << std::endl;
	}
	acceptNewConnection();

	std::map<int, Connection *>::iterator it = _connections.begin();
	std::cout << "_connection.size() : " << _connections.size() << std::endl;
	while (it != _connections.end())
	{
		std::map<int, Connection *>::iterator it2 = it;
		int fd = it2->first;

		runRecvAndSolve(*(it2->second));
		close(fd);
		it++;
	}
}
