/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:50:16 by tlorne            #+#    #+#             */
/*   Updated: 2024/04/29 17:59:32 by motoko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
//#include <map>
//#include "Connection.hpp"
//#include "location.hpp"
#include "server_manager.hpp"
//#include "config.hpp"
//#include "Response.hpp"

//class Config;
//class Connection;
//class Location;
class ServerManager;
//class Response;

class Server {
	public:
		Server(ServerManager manager, std::string server_block, std::vector<std::string> location_block, std::string config);
		~Server();
		//void    completeVectorLocation(std::vector<std::string> location_block);
		//void    run();

	private:
		//ServerManager   _manager;
		//Config  _config;
		//std::vector<Location>    _location;
		std::string _serveur_name;
		std::string _host;
		int _port;
		int _fd;
        //int _request_uri_limit_size;
        //int _limit_client_body_size;
        //std::string _default_error_page;
        /*std::map<int, Connection>    _connections;
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
		void	acceptNewConnection();*/
};

#endif

