/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motoko <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 16:28:39 by motoko            #+#    #+#             */
/*   Updated: 2024/04/30 14:14:15 by motoko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>     // For input/output
#include <cstring>      // For string functions
#include <cstdlib>      // For exit() function
#include <unistd.h>     // For read(), write(), close() functions
#include <netinet/in.h> // For sockaddr_in structure
#include <arpa/inet.h>  // For htons() function
#include <cerrno>
#include <sstream>

#include "server_manager.hpp"

#include "webserv_macro.hpp"

//#define PORT 8080
//#define BUFFER_SIZE 1024
//#define TIMEOUT_SEC 5

int main(int ac, char **av, char **env) 
{
	ServerManager	manager;

	if (ac > 2) {
		std::cout << "error : There are too many arguments!" << std::endl;
	}

	try {
		manager.createServer(av[1], env);

	} catch (std::exception &e) {
		std::cout << RED << "error :" << e.what() << RESET << std::endl;
	}
	

	/*
	int	server_fd, new_socket;

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		std::cerr << "Socket creation failed: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	int	opt = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
		std::cerr << "Setsockopt SO_REUSEADDR failed: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in address;

	address.sin_family			= AF_INET;	
	address.sin_addr.s_addr		= htonl(INADDR_ANY);
	address.sin_port			= htons(PORT);
	

	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == -1) {
		std::cerr << "Bind failed: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}


	if (listen(server_fd, 3) == -1) {
		std::cerr << "Listen failed: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	socklen_t addrlen = sizeof(address); 
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) == -1) {
		std::cerr << "Accept failed: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	struct timeval timeout;
	timeout.tv_sec = TIMEOUT_SEC;
	timeout.tv_usec = 0;
	if (setsockopt(new_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) == -1) {
		std::cerr << "Failed to set receive timeout" << std::endl;
		exit(EXIT_FAILURE);
	}

	close(server_fd);

	std::string header = "HTTP/1.1 200 OK\r\n";
	std::string body = "Hello from server!!! Here Adrien\n";
	std::ostringstream oss;
	oss << header << "Content-Length: " << body.length() << "\r\n\r\n" << body;
	std::string response = oss.str();

	if (send(new_socket, response.c_str(), response.length(), 0) == -1) {
		std::cerr << "Send failed: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	int bytes_received = 0;
	while (!bytes_received) {
		char buffer[1024];
		bytes_received = recv(new_socket, buffer, sizeof(buffer), 0);
		if (bytes_received == -1) {
			std::cerr << "Error in receiving data" << std::endl;
		} else {
			buffer[bytes_received] = '\0';
			std::cout << "Received " << bytes_received << " bytes: " << buffer << std::endl;
		}
	}

	close(new_socket);
	*/

	return (0);
}
