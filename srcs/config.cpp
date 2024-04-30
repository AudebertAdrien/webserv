/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:46:05 by tlorne            #+#    #+#             */
/*   Updated: 2024/04/30 14:41:19 by motoko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.hpp"
#include "webserv_macro.hpp"

Config::Config() {
	std::cout << "Config default constructor" << std::endl;
}

Config::Config(std::string config_block, char **env) {
	std::cout << "Config with params" << std::endl;
	
	std::cout << BLUE << "\nConfig_block : \n" << RESET << config_block << std::endl;
}

Config::~Config() {
	std::cout << "Config destructor" << std::endl;
}
