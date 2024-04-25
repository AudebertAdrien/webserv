/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motoko <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 17:31:26 by motoko            #+#    #+#             */
/*   Updated: 2024/04/25 17:50:18 by motoko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerManager.hpp"
#include "libft.hpp"
#include "Webserv_macro.hpp"

ServerManager::ServerManager() {
	std::cout << "ServerManager default constructor" << std::endl;
}

ServerManager::~ServerManager() {
	std::cout << "ServerManager destructor" << std::endl;
}

void	ServerManager::createServer(const std::string &configuration_file_path, char **env) {
	std::string	config_string = ft::getStringFromFile(configuration_file_path);
	//std::cout << RED << "getStringFromLine : \n" << RESET << config_string << std::endl;

	std::string config_block;
	std::vector<std::string> server_strings;

	splitConfigString(config_string, config_block, server_strings);
	std::cout << "config_block : " << config_block << std::endl;
}

void	ServerManager::splitConfigString(std::string &config_string, std::string &config_block, std::vector<std::string> &server_strings) {
	std::cout << RED << "splitConfigString" << RESET << std::endl;

	std::istringstream	iss(config_string);
	std::stringstream	server_ss;

	bool inside_server_block = false;

	std::string line;
	while (std::getline(iss, line)) {
		if (line == "server {") {
			inside_server_block = true;
			server_ss.str("");
			server_ss.clear();
		}
		else if (inside_server_block && line == "}") {
			inside_server_block = false;
			server_strings.push_back(server_ss.str());
		}
		else if (inside_server_block) {
            server_ss << line << std::endl;
        }
		else {
			config_block += line + "\n";
		}
	}
}
