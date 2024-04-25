/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motoko <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 16:28:39 by motoko            #+#    #+#             */
/*   Updated: 2024/04/24 17:18:34 by motoko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>     // For input/output
#include <cstring>      // For string functions
#include <cstdlib>      // For exit() function
#include <unistd.h>     // For read(), write(), close() functions
#include <netinet/in.h> // For sockaddr_in structure
#include <arpa/inet.h>  // For htons() function
#include <cerrno>

#define PORT 8080
#define BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1"

#define RED "\e[31m"
#define RESET "\e[0m"

int main(void) {
	std::cout << "Hello I'm the Client" << std::endl;

	int sock, valread;

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		std::cerr << "Socket creation failed: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	if (setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, 1, sizeof(int)) == -1) {
		std::cerr << "Setsockopt SO_KEEPALIVE failed: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	struct timeval timeout;
    timeout.tv_sec = 5; 
    timeout.tv_usec = 0;
	if (setsockopt(new_socket, SOL_SOCKET, SOL_RCVTIMEO, 1, sizeof(int)) == -1) {
		std::cerr << "Setsockopt SO_REUSEADDR failed: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in serv_addr;
	
	serv_addr.sin_family		= AF_INET;
	serv_addr.sin_addr.s_addr 	= inet_addr(SERVER_IP);
	serv_addr.sin_port			= htons(PORT);

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
		std::cerr << "Connection failed: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	char	buffer[BUFFER_SIZE] = {0};
	if ((valread = read(sock, buffer, BUFFER_SIZE)) == -1) {
		std::cerr << "Read error: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	std::cout << "Message fron server: " << buffer << std::endl;

    const char *message = "Hello from client!";

	int bytes_sent = send(sock, message, strlen(message), 0);
	if (bytes_sent == -1) {
		std::cerr << "Error in sending data" << std::endl;
	} else {
		std::cout << "Sent " << bytes_sent << " bytes: " << message << std::endl;
	}

	close(sock);
	return (0);
}
