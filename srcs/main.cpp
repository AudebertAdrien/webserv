#include <iostream>     
#include <cstring>      
#include <cstdlib>      
#include <unistd.h>     
#include <netinet/in.h> 
#include <arpa/inet.h>  
#include <cerrno>
#include <sstream>
#include <csignal>		
#include <iostream>

#include "server_manager.hpp"

#include "webserv_macro.hpp"


void clear_all()
{
	std::cout << std::endl <<"######################### " << GREEN << "THANK FOR USING OUR WEB SERVER " << RESET <<"########################" << std::endl;
	std::cout << "######################### " << GREEN << "CLOSING SERVEURS " << std::flush;
	for (int i = 0; i < 3; i++)
	{
		std::cout << ". " << std::flush;
		usleep(5000);
	}
	std::cout << RESET <<"########################" << std::endl;
    std::cout << "#########################" << GREEN <<  " 👋🏻👋 BYE BYE 👋🏾👋 " << RESET << "########################" << std::endl;
	std::cout << "#########################" <<  " MOTOKO   AND   THEOUCHE " << RESET << "########################" << std::endl;
}

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
	clear_all();
	return (0);
}
