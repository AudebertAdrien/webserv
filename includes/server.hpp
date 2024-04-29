/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:50:16 by tlorne            #+#    #+#             */
/*   Updated: 2024/04/22 12:50:18 by tlorne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <map>
#include "Connection.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include "ServerManager.hpp"
#include "Config.hpp"
#include "Response.hpp"

class Config;
class Connection;
class Server;
class Location;
class ServerManager;
class Response;

class Server
{
	public:
		Server(ServerManager, server_block, location_block, config);
        ~Server();
		void    run();
		

		
	private:
        ServerManager   _manager;
        std::string _serveur_name;
        std::string _host;
        int _port;
        int _fd;
        int _request_uri_limit_size;
        int _limit_client_body_size;
        std::string _default_error_page;
        Config  _config;
        std::vector<Location>    _location;
        std::map<int, Connection>    _connections;
        queue<Response>	_responses;
		bool	hasException(int client_fd);
		void	closeConnection(int	client_fd);
		bool	isSendable(int	client_fd) ?;
		void	sendResponse(Response response);
		void	hasRequest(int client_fd);
		Request	recvRequest(int client_fd);
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
		bool	hasNewConnection();
		void	acceptNewConnection();

};

#endif
