/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:50:16 by tlorne            #+#    #+#             */
/*   Updated: 2024/05/17 14:53:56 by motoko           ###   ########.fr       */
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
#include "connection.hpp"
#include "config.hpp"

#include "libft.hpp"
#include "webserv_macro.hpp"
#include "utils.hpp"

class ServerManager;
class Request;

class Server {
	public:
		Server();
		Server(ServerManager &manager, std::string server_block, std::vector<std::string> location_block, Config &config);
		~Server();

		void    run();

		/* == getter == */
		int		getPort();
		int		getFd();

	private:
		void		recvRequest(Connection &connection);
		void		solveRequest(Connection &connection);
		void		executeGet(Connection &connection);
		void		runRecvAndSolve(Connection &connection);
		//bool		parseStartLine(Connection &connection, Request &request);
		//bool		parseHeader(Connection &connection, Request &request);
		void		completeServer(std::string server_block);
		bool		hasNewConnection();
		void		acceptNewConnection();
		void		addConnection(int client_fd, std::string client_ip, int client_port);
		void    	completeVectorLocation(std::vector<std::string> location_block);
		std::string	createFilePath(std::string root_path, std::string relativ_path);
		void		initiateResponse(Connection &connection, Location &loc);
		void		closestMatch(Connection &connection);
		

		Config  		*_config;
		ServerManager	*_manager;

		std::vector<Location *>    _location;

		std::string 		_serveur_name;
		std::string 		_host;
		int 				_port;
		int 				_fd;
		struct sockaddr_in	_server_addr;

		std::map<int, Connection *>    _connections;

        /*
		void	handleConnection();

        int _request_uri_limit_size;
        int _limit_client_body_size;

        std::string _default_error_page;
        queue<Response>	_responses;

		bool	hasException(int client_fd);
		void	closeConnection(int	client_fd);
		bool	isSendable(int	client_fd) ?;
		void	sendResponse(Response response);

		void	hasRequest(int client_fd);

		void	executeAutoindex();
		void	executeGet(Request request);
		void	executeHead(Request request);
		void	executePut(Request request);
		void	executePost(Request request);
		void	executeDelete(Request request);
		void	executeOptions(Request request);
		void	executeTrace(Request request);

		char	**createCGIEnv();
		void	executeCGI(Request request);
		void	createResponse(int status_code);
		*/
};

#endif

