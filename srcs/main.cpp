/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motoko <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 16:28:39 by motoko            #+#    #+#             */
/*   Updated: 2024/05/09 13:07:35 by motoko           ###   ########.fr       */
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
#include <csignal>		// For signal() functions
#include <iostream>

#include "server_manager.hpp"

#include "webserv_macro.hpp"

void signalHandler(int signum) 
{
    std::cout << std::endl <<"######################### " << GREEN << "THANK FOR USING OUR WEB SERVER " << RESET <<"########################" << std::endl;
	std::cout << "######################### " << GREEN << "CLOSING SERVEURS " << std::flush;
	for (int i = 0; i < 3; i++)
	{
		std::cout << ". " << std::flush;
		sleep(1);
	}
	std::cout << RESET <<"########################" << std::endl;
	//std::cout << "#########################" << GREEN <<  " 👋🏻👋🏻 BYE BYE 👋🏾👋🏾 " << RESET << "########################" << std::endl;
    std::cout << "#########################" << GREEN <<  " 👋🏻👋 BYE BYE 👋🏾👋 " << RESET << "########################" << std::endl;
	std::cout << "#########################" <<  " MOTOKO AND THEOUCHE " << RESET << "########################" << std::endl;
	// clear tout ce qui doit etre clear
    
    exit(signum);
}

int main(int ac, char **av) 
{
	ServerManager	manager;

	signal(SIGINT, signalHandler);

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
