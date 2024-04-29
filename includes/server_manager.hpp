/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_manager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:50:02 by tlorne            #+#    #+#             */
/*   Updated: 2024/04/22 12:50:03 by tlorne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_MANAGER_HPP
# define SERVER_MANAGER_HPP

#include <iostream>
#include <map>
#include <vector>
#include "Connection.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include "Config.hpp"
#include "Response.hpp"

class Config;
class Connection;
class Location;
class Response;

class ServerManager
{
	public:
		ServerManager();
        ~ServerManager();
        void    setType//pour les enum
        Config  getConfig() const;
        int getMaxFd() const;
        void    setConfig(Config config);
        void    setMaxFd(int fd);
        fdSet(int fd, setType);
        fdZero(setType);
        fdClear(fd, setType);
        fdIsset(int fd, setType);
        fdCopy(setType);
        void    exitServer();
        void    createServer();
        void    runServer();

		
	private:
        std::vector<Server>  _servers;
        Config  _config;
        int _max_fd;
        fd_set  _read_set;
        fd_set  _read_copy_set;
        fd_set  _write_set;
        fd_set  _write_copy_set;
        fd_set  _error_set;
        fd_set  _error_copy_set;
        bool    splitConfigString(config_string, config_block, server_string vector);
        bool    splitServerString(server_string, server_block, location_block vector);
        bool    isValidConfigBlock(config_block);
        bool    isValidServerBlock(server_block);
        bool    isValidLocationBlock(location_block);
};

#endif


