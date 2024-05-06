/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_manager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motoko <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 17:31:26 by motoko            #+#    #+#             */
/*   Updated: 2024/05/03 19:03:54 by motoko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server_manager.hpp"
#include "server.hpp"
#include "webserv_macro.hpp"

ServerManager::ServerManager() {
	//std::cout << "ServerManager default constructor" << std::endl;
}

ServerManager::~ServerManager() {
	//std::cout << "ServerManager destructor" << std::endl;
}

void	ServerManager::createServer(const std::string &configuration_file_path, char **env) {
	std::string	config_string = ft::getStringFromFile(configuration_file_path);

	std::string config_block;
	std::vector<std::string> server_strings;
	if (!splitConfigString(config_string, config_block, server_strings)) {
		throw (std::invalid_argument("Failed to split configuration string"));
	}
	
	_config = Config(config_block, env);
	
	//ft::display_vector(server_strings);

	for (size_t i = 0; i < server_strings.size(); i++) {
		std::string server_block;
		std::vector<std::string> location_block;

		if (!splitServerString(server_strings[i], server_block, location_block)) {
			throw (std::invalid_argument("Failed to split server string"));
		}

		//ft::display_vector(location_block);
		this->_servers.push_back(Server(*this, server_block, location_block, this->_config));
		//Server(*this, server_block, location_block, this->_config);
		//push back dans le vector dans _servers avec const param de serveur;
	}
}

void	ServerManager::runServer()
{
	std::cout << "######################### RUN SERVERS ########################" << std::endl;

	std::vector<Server>::iterator	it = this->_servers.begin();

	std::cout << "server listen on port :" << std::endl;

	FD_ZERO(&(this->_read_set));
	FD_ZERO(&(this->_write_set));
	while (it != this->_servers.end())
	{
		std::cout << it->getPort() << std::endl;		
		FD_SET(it->getFd(), &(this->_read_set));
		FD_SET(it->getFd(), &(this->_write_set));
		if (this->_max_fd < it->getFd())
			this->_max_fd = it->getFd();
		it++;
	}

	int cnt;

	struct timeval timeout;
	while(true) {
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;

		this->_read_copy_set = this->_read_set;
		this->_write_copy_set = this->_write_set;
		if ((cnt = select(this->_max_fd + 1, &_read_copy_set, &_write_copy_set, NULL, &timeout)) == -1) {
			std::cerr << "Select failed: " << strerror(errno) << std::endl;
			exit(EXIT_FAILURE);
		}
		else if (cnt == 0) {
			std::cout << "Timeout occurred\n";
			continue;
		}

		for (std::vector<Server>::iterator it = _servers.begin() ; it != _servers.end() ; ++it) {
			if (FD_ISSET(it->getFd(), &_read_copy_set) || FD_ISSET(it->getFd(), &_write_copy_set)) 
			{
				std::cout << "######################### FD_ISSET = _fd: " << it->getFd() << " port : " << it->getPort() << "########################" << std::endl;
				it->run();
			}
		}
	}
}

bool	ServerManager::splitServerString(std::string &server_strings, std::string &server_block, std::vector<std::string> &location_block) {
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
