/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_manager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:50:02 by tlorne            #+#    #+#             */
/*   Updated: 2024/05/20 14:47:09 by motoko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_MANAGER_HPP
# define SERVER_MANAGER_HPP

#include <iostream>
#include <vector>
#include <sstream>
#include <sys/select.h>
#include <unistd.h>
#include <csignal>

#include "config.hpp"
#include "location.hpp"

#include "libft.hpp"
#include "webserv_macro.hpp"

class Server;

#define SELECT_TIMEOUT 5

class ServerManager {
	public:
		ServerManager();
		~ServerManager();

		int 	getMaxFd() const;
		fd_set&	getFdReadSet();
		fd_set&	getFdReadSetCopy();
		fd_set&	getFdWriteSet();
		std::vector<Server *>	getServer();

		void    runServer();
		void	createServer(const std::string &configuration_file_path);
		void	setFd(int fd, std::string fd_type);
		
		
		//void    exitServer();

	private:
		bool	splitConfigString(std::string &config_string, std::string &config_block, std::vector<std::string> &server_string);
		bool	splitServerString(std::string &server_string, std::string &server_block, std::vector<std::string> &location_string);
		void	resetMaxFd();
		
		std::vector<Server *>	_servers;
		Config					*_config;

		int 	_max_fd;
		fd_set  _read_set;
		fd_set  _read_copy_set;
		fd_set  _write_set;
		fd_set  _write_copy_set;

		/*
		fd_set  _error_set;
		fd_set  _error_copy_set;
		*/
};

#endif
