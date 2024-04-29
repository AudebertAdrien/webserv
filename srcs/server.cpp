/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:50:12 by tlorne            #+#    #+#             */
/*   Updated: 2024/04/29 17:53:15 by motoko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

/*void    Server::completeVectorLocation(std::vector<std::string> location_block)
{

}*/

Server::Server(ServerManager manager, std::string server_block, std::vector<std::string> location_block, std::string config)
{
    std::cout << "Server default constructor" << std::endl;
    //this->_manager = manager;
    //parse serveur block pour obtenir serveur_name, host, port, fd.

    // une fonction pour transorfer vector string location en vecteur location;
    //completeVectorLocation(location_block)

    // idem avec config.
}


Server::~Server() {
	std::cout << "Server destructor" << std::endl;
}
