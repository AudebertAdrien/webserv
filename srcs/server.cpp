/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:50:12 by tlorne            #+#    #+#             */
/*   Updated: 2024/05/02 15:22:45 by motoko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "server_manager.hpp"
#include "webserv_macro.hpp"

void    Server::completeVectorLocation(std::vector<std::string> location_block)
{
    std::vector<std::string>::iterator it;
	for (it = location_block.begin(); it != location_block.end(); it++) {
        this->_location.push_back(Location(*it));
    }
}

Server::Server()
{
    std::cout << "Server default constructor" << std::endl;
}

Server::Server(ServerManager manager, std::string server_block, std::vector<std::string> location_block, Config  config)
{
    std::cout << "Server with params constructor" << std::endl;

    this->_config = config;
    //parse serveur block pour obtenir serveur_name, host, port, fd.

    // une fonction pour transorfer vector string location en vecteur location;
    //std::cout << BLUE << "#######################Beg test LOCATION #########################"  << RESET << std::endl;
    completeVectorLocation(location_block);

    // idem avec config.
}


Server::~Server() {
	std::cout << "Server destructor" << std::endl;
}
