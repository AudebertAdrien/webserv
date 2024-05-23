/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_manager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motoko <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 17:31:26 by motoko            #+#    #+#             */
/*   Updated: 2024/05/22 16:34:57 by motoko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server_manager.hpp"
#include "server.hpp"

ServerManager::ServerManager() {
	this->_max_fd = -1;
}

ServerManager::~ServerManager() {
	std::vector<Server *>::iterator it;
	for (it = _servers.begin() ; it != _servers.end() ; ++it) {
		delete *it;
	}
	delete	_config;
}

void	ServerManager::createServer(const std::string &configuration_file_path, char **env) {
	std::string	config_string = ft::getStringFromFile(configuration_file_path);

	std::string config_block;
	std::vector<std::string> server_strings;
	if (!splitConfigString(config_string, config_block, server_strings)) {
		throw (std::invalid_argument("Failed to split configuration string"));
	}
	
	this->_config = new Config(config_block, env);
	
	for (size_t i = 0; i < server_strings.size(); i++) {
		std::string server_block;
		std::vector<std::string> location_block;

		if (!splitServerString(server_strings[i], server_block, location_block)) {
			throw (std::invalid_argument("Failed to split server string"));
		}
		this->_servers.push_back(new Server(*this, server_block, location_block, *this->_config));
	}
}

void	ServerManager::resetMaxFd()
{
	for (int i = 0; i < 1024; ++i)
	{
		if (FD_ISSET(i, &this->_read_set) && i > this->_max_fd) 
            this->_max_fd = i;
	}
}

void	ServerManager::setFd(int fd, std::string fd_type) {
	if (fd_type == "_read_set")
		FD_SET(fd, &this->_read_set);
	if (fd_type == "_write_set")
		FD_SET(fd, &this->_write_set);
}

void	ServerManager::runServer()
{
	std::cout << "######################### RUN SERVERS ########################" << std::endl;

	std::vector<Server *>::iterator	it = this->_servers.begin();

	FD_ZERO(&(this->_read_set));
	FD_ZERO(&(this->_read_copy_set));
	FD_ZERO(&(this->_write_set));
	FD_ZERO(&(this->_write_copy_set));
	while (it != this->_servers.end())
	{
		FD_SET((*it)->getFd(), &(this->_read_set));
		it++;
	}

	resetMaxFd();
	int cnt;
	struct timeval timeout;
	
	while(true) {
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;

		this->_read_copy_set = this->_read_set;
		this->_write_copy_set = this->_write_set;
		if ((cnt = select(this->_max_fd + 1, &this->_read_copy_set, &this->_write_copy_set, NULL, &timeout)) == -1){
			std::cerr << "Select failed: " << strerror(errno) << std::endl;
			exit(EXIT_FAILURE);
		}
		else if (cnt == 0) {
			continue;
		}
		std::vector<Server *>::iterator it;
		for (it = _servers.begin() ; it != _servers.end() ; ++it) {
			(*it)->run();
		} 
		resetMaxFd();
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

int	ServerManager::getMaxFd() const
{
	return (this->_max_fd);
}

fd_set &	ServerManager::getFdReadSet()
{
	return (this->_read_set);
}

fd_set &	ServerManager::getFdReadSetCopy()
{
	return (this->_read_copy_set);
}

fd_set &	ServerManager::getFdWriteSet()
{
	return (this->_write_set);
}

std::vector<Server *>	ServerManager::getServer()
{
	return (this->_servers);
}
