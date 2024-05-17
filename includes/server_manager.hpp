/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_manager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:50:02 by tlorne            #+#    #+#             */
/*   Updated: 2024/05/17 18:20:31 by motoko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_MANAGER_HPP
# define SERVER_MANAGER_HPP

#include <iostream>
#include <vector>
#include <sstream>
#include <sys/select.h>
#include <unistd.h> 

#include "config.hpp"
#include "location.hpp"

#include "libft.hpp"
#include "webserv_macro.hpp"

class Server;

class ServerManager {
	public:
		ServerManager();
		~ServerManager();

		int 	getMaxFd() const;
		fd_set	getFdSet() const;
		int		getNbServers() const ;
		std::vector<Server *>	getServer();

		void	resetMaxFd();

		void    runServer();
		void	createServer(const std::string &configuration_file_path, char **env);
		bool	splitConfigString(std::string &config_string, std::string &config_block, std::vector<std::string> &server_string);
		bool	splitServerString(std::string &server_string, std::string &server_block, std::vector<std::string> &location_string);

		//Config  getConfig() const;
		//void    setType//pour les enum
		//void    setConfig(Config config);
		//void    setMaxFd(int fd);
		
		void	setFd(int fd, std::string fd_type);
		//fdZero(setType);
		//fdClear(fd, setType);
		//fdIsset(int fd, setType);
		//fdCopy(setType);*/
		
		//void    exitServer();

	private:
		std::vector<Server *>	_servers;
		Config					*_config;

		int		_nb_servers;
		int 	_max_fd;
		fd_set  _read_set;
		fd_set  _read_copy_set;
		fd_set  _write_set;
		fd_set  _write_copy_set;

		/*
		fd_set  _error_set;
		fd_set  _error_copy_set;
		bool    isValidConfigBlock(config_block);
		bool    isValidServerBlock(server_block);
		bool    isValidLocationBlock(location_block);
		*/
};

#endif
