/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:50:16 by tlorne            #+#    #+#             */
/*   Updated: 2024/05/29 16:10:06 by motoko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h> 
#include <fcntl.h>
#include <sys/socket.h>
#include <cerrno>
#include <arpa/inet.h>

#include <vector>
#include <map>

#include "location.hpp"
#include "config.hpp"

#include "libft.hpp"
#include "utils.hpp"
#include "webserv_macro.hpp"

class ServerManager;
class Request;
class Connection;

class Server {
	public:
		Server();
		Server(ServerManager &manager, std::string server_block, std::vector<std::string> location_block, Config &config);
		~Server();

		void    run();

		/* == getter == */
		int						getPort();
		int						getRedirectStatusCode();
		std::string				getRedirectLink();
		int						getFd();
		std::vector<Location *>	&getLocation();

	private:
		void		runRecvAndSolve(Connection &connection);
		void		acceptNewConnection();
		void		addConnection(int client_fd, std::string client_ip, int client_port);
		void		removeFromSet(int fd);
		void		set(std::string server_block);

		Config  		*_config;
		ServerManager	*_manager;

		std::vector<Location *>    _location;

		int					_redirect_status_code;
		std::string			_redirect_link;

		std::string 		_serveur_name;
		std::string 		_host;
		int 				_port;
		int 				_fd;
		struct sockaddr_in	_server_addr;

		std::map<int, Connection *>    _connections;
};

#endif

