/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_manager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:50:02 by tlorne            #+#    #+#             */
/*   Updated: 2024/04/29 18:47:06 by motoko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_MANAGER_HPP
# define SERVER_MANAGER_HPP

#include <iostream>
#include <vector>
#include <sstream>
//#include <map>

#include "config.hpp"
#include "server.hpp"
#include "location.hpp"
//#include "connection.hpp"
//#include "Response.hpp"

#include "libft.hpp"

class Config;
class Server;
//class Connection;
class Location;
//class Response;

class ServerManager {
	public:
		ServerManager();
		~ServerManager();
		//void    setType//pour les enum
		//Config  getConfig() const;
		//int getMaxFd() const;
		//void    setConfig(Config config);
		//void    setMaxFd(int fd);
		/*fdSet(int fd, setType);
		  fdZero(setType);
		  fdClear(fd, setType);
		  fdIsset(int fd, setType);
		  fdCopy(setType);*/
		//void    exitServer();
		//void    runServer();
		void	createServer(const std::string &configuration_file_path, char **env);
		bool	splitConfigString(std::string &config_string, std::string &config_block, std::vector<std::string> &server_string);
		bool	splitServerString(std::string &server_string, std::string &server_block, std::vector<std::string> &location_string);


	private:
		std::vector<Server>  _servers;
		Config  _config;
		//int _max_fd;
		/*fd_set  _read_set;
		  fd_set  _read_copy_set;
		  fd_set  _write_set;
		  fd_set  _write_copy_set;
		  fd_set  _error_set;
		  fd_set  _error_copy_set;*/
		/*
		   bool    isValidConfigBlock(config_block);
		   bool    isValidServerBlock(server_block);
		   bool    isValidLocationBlock(location_block);*/
};

#endif
