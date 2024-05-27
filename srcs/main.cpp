/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motoko <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 16:28:39 by motoko            #+#    #+#             */
/*   Updated: 2024/05/27 15:21:03 by motoko           ###   ########.fr       */
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

int main(int ac, char **av) 
{
	ServerManager	manager;

	if (ac > 2) {
		std::cout << "error : There are too many arguments!" << std::endl;
	}

	try {
		manager.createServer(av[1]);
		manager.runServer();

	} catch (std::exception &e) {
		std::cout << RED << "error :" << e.what() << RESET << std::endl;
	}
	return (0);
}
