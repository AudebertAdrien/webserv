/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:46:05 by tlorne            #+#    #+#             */
/*   Updated: 2024/05/03 16:56:25 by motoko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.hpp"
#include "webserv_macro.hpp"

Config::Config() {
	//std::cout << "Config default constructor" << std::endl;
}

Config::Config(std::string config_block, char **env) {
	//std::cout << "Config with params" << std::endl;
	std::istringstream	iss(config_block);
	std::string	line;

	while (getline(iss, line)) {
		std::string key, value;

		size_t pos =  line.find_first_of(" \t");
		if (pos != std::string::npos) {
			key = line.substr(0, pos);
			value = line.substr(pos + 1);
		}

		key.erase(0, key.find_first_not_of(" \t"));
		key.erase(key.find_last_not_of(" \t") + 1);
		value.erase(0, value.find_first_not_of(" \t"));
		value.erase(value.find_last_not_of(" \t") + 1);

		if (key == "software_name")
			_software_name = value;
		if (key == "software_version")
			_software_version = value;
		if (key == "http_version") 
			_http_version = value;
		if (key == "cgi_version")
			_cgi_version = value;
	}
}

Config::~Config() {
	//std::cout << "Config destructor" << std::endl;
}

std::string Config::getSoftwareName() const {
	return this->_software_name;
}
std::string Config::getSoftwareVers() const {
	return this->_software_version;
}

std::string Config::getHttpVers() const {
	return this->_http_version;
}

std::string Config::getCgiVers() const {
	return this->_cgi_version;
}


