/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:50:16 by tlorne            #+#    #+#             */
/*   Updated: 2024/05/12 17:01:16 by motoko           ###   ########.fr       */
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

class ServerManager;
class Request;

class Server {
	public:
		Server();
		Server(ServerManager &manager, std::string server_block, std::vector<std::string> location_block, Config &config);
		~Server();

		void    completeVectorLocation(std::vector<std::string> location_block);
		void    run();
		void	runRecvAndSolve(Connection &connection);
		void	recvRequest(Connection &connection);

		bool	parseStartLine(Connection &connection, Request &request);
		bool	parseHeader(Connection &connection, Request &request);

		void	completeServer(std::string server_block);

		bool	hasNewConnection();
		void	acceptNewConnection();

		void	addConnection(int client_fd, std::string client_ip, int client_port);

		/* == getter == */
		int		getPort();
		int		getFd();

	private:
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

		Location	_test;continue
        int _request_uri_limit_size;
        int _limit_client_body_size;
        std::string _default_error_page;
        queue<Response>	_responses;

		bool	hasException(int client_fd);
		void	closeConnection(int	client_fd);
		bool	isSendable(int	client_fd) ?;
		void	sendResponse(Response response);

		void	hasRequest(int client_fd);
		void	solveRequest(Request request);

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

