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
		//this->_servers.push_back(Server(*this, server_block, location_block, this->_config));
		Server(*this, server_block, location_block, this->_config);
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
