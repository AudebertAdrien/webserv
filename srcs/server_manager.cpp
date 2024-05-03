/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_manager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motoko <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 17:31:26 by motoko            #+#    #+#             */
/*   Updated: 2024/05/02 19:26:41 by motoko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server_manager.hpp"
#include "server.hpp"
#include "webserv_macro.hpp"

ServerManager::ServerManager() {
	std::cout << "ServerManager default constructor" << std::endl;
	//FD_ZERO(&this->_read_set);
}

ServerManager::~ServerManager() {
	std::cout << "ServerManager destructor" << std::endl;
}

void	ServerManager::createServer(const std::string &configuration_file_path, char **env) {
	std::string	config_string = ft::getStringFromFile(configuration_file_path);

	std::string config_block;
	std::vector<std::string> server_strings;
	if (!splitConfigString(config_string, config_block, server_strings)) {
		throw (std::invalid_argument("Failed to split configuration string"));
	}
	
	_config = Config(config_block, env);
	
	ft::display_vector(server_strings);

	for (size_t i = 0; i < server_strings.size(); i++) {
		std::string server_block;
		std::vector<std::string> location_block;

		if (!splitServerString(server_strings[i], server_block, location_block)) {
			throw (std::invalid_argument("Failed to split server string"));
		}

		ft::display_vector(location_block);
		this->_servers.push_back(Server(*this, server_block, location_block, this->_config));
		//Server(*this, server_block, location_block, this->_config);
		//push back dans le vector dans _servers avec const param de serveur;
	}
}

bool	ServerManager::splitServerString(std::string &server_strings, std::string &server_block, std::vector<std::string> &location_block) {
	std::cout << YELLOW << "splitServerString" << RESET << std::endl;

	std::istringstream	iss(server_strings);
	std::stringstream	location_ss;

	bool inside_location_block = false;

	std::string line;
	while (std::getline(iss, line)) 
	{
		std::istringstream iss2(line);
    	std::string fw;
		iss2 >> fw;
		if (line == "	location {" || fw == "location") { 
			inside_location_block = true;
			location_ss.str("");
			location_ss.clear();
			location_ss << line << std::endl;
		}
		else if (inside_location_block && line == "	}") {
			inside_location_block = false;
			location_ss << line << std::endl;
			location_block.push_back(location_ss.str());
		}
		else if (inside_location_block) {
			if (!line.empty())
				location_ss << line << std::endl;
		}
		else {
			if (!line.empty())
				server_block += line + "\n";	
		}
	}
	return (true);
}

bool	ServerManager::splitConfigString(std::string &config_string, std::string &config_block, std::vector<std::string> &server_strings) {
	std::cout << YELLOW << "splitConfigString" << RESET << std::endl;

	std::istringstream	iss(config_string);
	std::stringstream	server_ss;

	bool inside_server_block = false;

	std::string line;
	while (std::getline(iss, line)) {
		if (line == "server {") {
			inside_server_block = true;
			server_ss.str("");
			server_ss.clear();
			server_ss << line << std::endl;
		}
		else if (inside_server_block && line == "}") {
			inside_server_block = false;
			server_ss << line << std::endl;
			server_strings.push_back(server_ss.str());
		}
		else if (inside_server_block) {
			if (!line.empty()) 
            	server_ss << line << std::endl;
        }
		else {
			if (!line.empty()) 
				config_block += line + "\n";
		}
	}
	return (true);
}


void printFdSet(fd_set &fds, int max_fd) {
    std::cout << "Descripteurs de fichiers présents dans l'ensemble :" << std::endl;
    for (int i = 0; i < max_fd; ++i) {
		//std::cout << "ok" << std::endl;
        if (FD_ISSET(i, &fds)) {
            std::cout << " - " << i << std::endl;
        }
    }
}

void	ServerManager::runServer()
{
	std::cout << "######################### RUN SERVERS ########################" << std::endl;
	
	this->_max_fd = 0;
	std::cout << "max_fd = " << this->_max_fd << std::endl;
	FD_ZERO(&this->_read_set);
	printFdSet(this->_read_set, this->_max_fd + 1);
	std::vector<Server>::iterator	it = this->_servers.begin();
	this->_max_fd = 0;
	std::cout << "server listen on port :" << std::endl;
	while (it != this->_servers.end())
	{
		std::cout << "######################################3ok" << std::endl;
		int	server_fd = it->getFd();
		std::cout <<  "server fd vaut " <<server_fd << std::endl;
		FD_SET(server_fd, &this->_read_set);
		//FD_SET(8080, &this->_read_set);
		//printFdSet(this->_read_set, this->_max_fd + 1);
		if (server_fd > this->_max_fd)
			this->_max_fd = server_fd;
		//printFdSet(this->_read_set, this->_max_fd + 1);
		it++;
	}
	std::cout << "max_fd = " << this->_max_fd << std::endl;
	printFdSet(this->_read_set, this->_max_fd + 1);
	//this->_read_copy_set = this->_read_set;
	//printFdSet(this->_read_copy_set, this->_max_fd + 1);
	//this->_max_fd++;
	//*/
	/*it = this->_servers.begin();
	while (true)
	{
		it = this->_servers.begin();
		std::cout << "######################### in the boucle ########################" << std::endl;
		while (it != this->_servers.end())
		{
			std::cout << "######################### ICI ########################" << std::endl;
			it->run();
			std::cout << "######################### ICI 3 ########################" << std::endl;
			it++;
		}
	}
	*/

	timeval timeout;
	timeout.tv_sec = 5;
	timeout.tv_usec = 0;
//*/

	while (true)
	{
		
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;
		//it = this->_servers.begin();
		printFdSet(this->_read_set, this->_max_fd + 1);

		std::cout << "######################### in the boucle ########################" << std::endl;
		this->_read_copy_set = this->_read_set;
		std::cout << "######################### copy ########################" << std::endl;
		printFdSet(this->_read_copy_set, this->_max_fd + 1);

		int	act = select(this->_max_fd + 1, &this->_read_copy_set, NULL, NULL, &timeout);
		std::cout << "act vaut : " << act << std::endl;

		if (act == -1)
		{
			std::cout << RED << "act vaut : " << act << RESET << std::endl;
			std::cerr << "Error in select" << std::endl;
		}
		else if (act == 0)
		{
			std::cout << YELLOW << "act vaut : " << act << RESET << std::endl;
			continue ;
		}
		std::cout << GREEN << "act vaut : " << act << RESET << std::endl;
		it = this->_servers.begin();
		while (it != this->_servers.end())
		{
			std::cout << "######################### ICI ########################" << std::endl;
			int	server_fd = it->getFd();
			if (FD_ISSET(server_fd, &this->_read_copy_set))
			{
				std::cout << "######################### ICI 3 ########################" << std::endl;
				it->run();
			}
			it++;
		}
	}
	//*/
}
