/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:50:12 by tlorne            #+#    #+#             */
/*   Updated: 2024/05/02 19:53:09 by motoko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "server_manager.hpp"
#include "webserv_macro.hpp"

#define PORT 8080
#define BUFFER_SIZE 1024
#define TIMEOUT_SEC 5

void    Server::completeVectorLocation(std::vector<std::string> location_block)
{
    std::vector<std::string>::iterator it;
	for (it = location_block.begin(); it != location_block.end(); it++) {
        this->_location.push_back(Location(*it));
    }
}

Server::Server()
{
    std::cout << "Server default constructor" << std::endl;
}

Server::Server(ServerManager manager, std::string server_block, std::vector<std::string> location_block, Config  config)
{
    std::cout << "Server with params constructor" << std::endl;

    this->_config = config;
	if ((_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
		std::cerr << "Socket creation failed: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	int opt = 1;
	if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
		std::cerr << "Setsockopt SO_REUSEADDR failed: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in server_addr;

	//ft::bzero(&server_addr, sizeof(struct sockaddr_in));
	server_addr.sin_family          = AF_INET;
	server_addr.sin_port            = htons(PORT);
	server_addr.sin_addr.s_addr     = htonl(INADDR_ANY);
    //server_addr.sin_addr.s_addr = INADDR_ANY;
	//server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

	if (bind(_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
		std::cerr << "Bind failed: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	if (listen(_fd, 256) == -1) {
		std::cerr << "Listen failed: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
 
	/*
	if (fcntl(_fd, F_SETFL, O_NONBLOCK) == -1) {
		std::cerr << "Listen failed: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
	*/

	int	client_socket; 

	socklen_t addrlen = sizeof(server_addr);
	if ((client_socket = accept(_fd, (struct sockaddr *)&server_addr, (socklen_t*)&addrlen)) == -1) {
		std::cerr << "Accept failed: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	struct timeval timeout;
	timeout.tv_sec = TIMEOUT_SEC;
	timeout.tv_usec = 0;
	if (setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) == -1) {
		std::cerr << "Failed to set receive timeout" << std::endl;
		exit(EXIT_FAILURE);
	}

	close(_fd);

	std::string header = "HTTP/1.1 200 OK\r\n";
	std::string body = "Hello from server!!! Here Adrien\n";
	std::ostringstream oss;
	oss << header << "Content-Length: " << body.length() << "\r\n\r\n" << body;
	std::string response = oss.str();

	if (send(client_socket, response.c_str(), response.length(), 0) == -1) {
		std::cerr << "Send failed: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	int bytes_received = 0;
	while (!bytes_received) {
		char buffer[1024];
		bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
		if (bytes_received == -1) {
			std::cerr << "Error in receiving data" << std::endl;
		} else {
			buffer[bytes_received] = '\0';
			std::cout << "Received " << bytes_received << " bytes: " << buffer << std::endl;
		}
	}

     close(client_socket);

    //parse serveur block pour obtenir serveur_name, host, port, fd.

    // une fonction pour transorfer vector string location en vecteur location;
    //std::cout << BLUE << "#######################Beg test LOCATION #########################"  << RESET << std::endl;
    //completeVectorLocation(location_block);

    // idem avec config.
}


Server::~Server() {
	std::cout << "Server destructor" << std::endl;
}

