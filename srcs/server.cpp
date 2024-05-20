/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:50:12 by tlorne            #+#    #+#             */
/*   Updated: 2024/05/20 17:27:08 by motoko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "server_manager.hpp"
#include "request.hpp"
#include "connection.hpp"

static int i = 0;

Server::Server(ServerManager &manager, std::string server_block, std::vector<std::string> location_block, Config  &config) {
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

	setPort(server_block);

	memset(&_server_addr, '\0', sizeof(struct sockaddr_in));
	_server_addr.sin_family          = AF_INET;
	_server_addr.sin_port            = htons(this->_port);
	_server_addr.sin_addr.s_addr     = htonl(INADDR_ANY);

	if (bind(_fd, (struct sockaddr *)&_server_addr, sizeof(_server_addr)) == -1) {
		std::cerr << "Bind failed: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	if (listen(_fd, 256) == -1) {
		std::cerr << "Listen failed: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
 
	fillVectorLocation(location_block);
}

Server::~Server() {
}

void    Server::fillVectorLocation(std::vector<std::string> location_block)
{
    std::vector<std::string>::iterator it;
	for (it = location_block.begin(); it != location_block.end(); it++) {
        this->_location.push_back(new Location(*it));
    }
}

void	Server::setPort(std::string server_block)
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

void	Server::addConnection(int client_fd, std::string client_ip, int client_port) {
    Connection *client = new Connection(client_fd, client_ip, client_port, *this);

    _connections.insert(std::make_pair(client_fd, client));
	this->_manager->setFd(client_fd, "_read_set");
	this->_manager->setFd(client_fd, "_write_set");

	 std::cout << RED << "READ SET" << RESET << std::endl;
	ft::displayFdSet(this->_manager->getFdReadSet());
	std::cout << RED << "WRITE SET" << RESET << std::endl;
	ft::displayFdSet(this->_manager->getFdWriteSet());
}

bool	Server::hasNewConnection(int fd, fd_set &set) {
	return (FD_ISSET(fd, &set));
}

void	Server::acceptNewConnection() {
	int	client_fd;
	struct sockaddr_in client_addr;

	socklen_t addrlen = sizeof(_server_addr);

	if ((client_fd = accept(_fd, (struct sockaddr *)&client_addr, (socklen_t*)&addrlen)) == -1) {
		std::cerr << "Accept failed: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	std::cout << RED << "CONNECTION ACCEPTED: " << i++ << std::endl;
	//if (!hasNewConnection(client_fd, this->_manager->getFdReadSet()) ) {
		struct timeval timeout;
		timeout.tv_sec = 3;
		timeout.tv_usec = 0;

		if (setsockopt(client_fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) == -1) {
			std::cerr << "Failed to set receive timeout" << std::endl;
			exit(EXIT_FAILURE);
		}

		int flags = fcntl(client_fd, F_GETFL, 0);
		if (flags == -1) {
			std::cerr << "Failed to get socket flags\n";
			close(client_fd);
			exit(EXIT_FAILURE);
		}

		if (fcntl(client_fd, F_SETFL, flags | O_NONBLOCK) == -1) {
			std::cerr << "Listen failed: " << strerror(errno) << std::endl;
			close(client_fd);
			exit(EXIT_FAILURE);
		}

		char client_ip[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
		int client_port = ntohs(client_addr.sin_port);

		addConnection(client_fd, client_ip, client_port);
	//}
}

void	Server::runRecvAndSolve(Connection &connection) {
	try {
		connection.recvRequest();
		connection.solveRequest();
	} catch (std::exception &e) {
		std::cerr << "recvRequest error!!!" << std::endl;
	}
}

void	Server::run() {
	if (this->_connections.size() >= (1024 / this->_manager->getServer().size()))
	{
		std::cout << "too many connection, old connection must be closed" << std::endl;
		return ;
	}

	if (FD_ISSET(this->_fd, &this->_manager->getFdReadSetCopy())) {
		acceptNewConnection();
	}

	std::map<int, Connection *>::iterator it = _connections.begin();
	while (it != _connections.end())
	{
		std::map<int, Connection *>::iterator it2 = it;
		int fd = it2->first;

		if (FD_ISSET(fd, &this->_manager->getFdReadSetCopy())) {
			runRecvAndSolve(*(it2->second));
			removeFromSet(fd);
			close(fd);
		}
		it++;
	}
}

void	Server::removeFromSet(int fd)
{
	if (FD_ISSET(fd, &this->_manager->getFdReadSet()))
		FD_CLR(fd, &this->_manager->getFdReadSet());
	if (FD_ISSET(fd, &this->_manager->getFdWriteSet()))
		FD_CLR(fd, &this->_manager->getFdWriteSet());
    //this->_connections.erase(fd);
}

int	Server::getPort()
{
	return (this->_port);
}

int	Server::getFd()
{
	return (this->_fd);
}

std::vector<Location *>	&Server::getLocation()
{
	return (this->_location);
}
