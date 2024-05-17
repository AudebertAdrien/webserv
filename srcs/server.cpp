/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:50:12 by tlorne            #+#    #+#             */
/*   Updated: 2024/05/17 15:10:33 by motoko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "server_manager.hpp"
#include "request.hpp"

#define PORT 8080
#define BUFFER_SIZE 1024
#define TIMEOUT_SEC 5

void    Server::completeVectorLocation(std::vector<std::string> location_block)
{
    std::vector<std::string>::iterator it;
	for (it = location_block.begin(); it != location_block.end(); it++) {
        this->_location.push_back(new Location(*it));
    }
}

void	Server::completeServer(std::string server_block)
{
	std::istringstream iss(server_block);
    std::string word;

	iss >> word;
	while (word != "listen")
		iss >> word;
	iss >> word;
	word.resize(word.size() - 1);
	this->_port = atoi(word.c_str());
}

Server::Server(ServerManager &manager, std::string server_block, std::vector<std::string> location_block, Config  &config) {
    this->_config = &config;
	this->_manager = &manager;
	if ((_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
		std::cerr << "Socket creation failed: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	int opt = 1;
	if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
		std::cerr << "Setsockopt SO_REUSEADDR failed: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	completeServer(server_block);

	memset(&_server_addr, '\0', sizeof(struct sockaddr_in));
	_server_addr.sin_family          = AF_INET;
	_server_addr.sin_port            = htons(this->_port);
	_server_addr.sin_addr.s_addr     = htonl(INADDR_ANY);

	if (bind(_fd, (struct sockaddr *)&_server_addr, sizeof(_server_addr)) == -1) {
		std::cerr << "Bind failed: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	if (listen(_fd, 256) == -1) {
		std::cerr << "Listen failed: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
 
	completeVectorLocation(location_block);
}

Server::~Server() {
}

/* void	Server::recvRequest(Connection &connection) {
	Request	*request = new Request(connection, *this);			

	int bytes_received = 1;
	while (bytes_received > 0) {
		char	buffer[1000];	

		//bytes_received = recv(connection.getFd(), buffer, sizeof(buffer), MSG_DONTWAIT);
		bytes_received = recv(connection.getFd(), buffer, sizeof(buffer), 0);
		if (bytes_received == -1) {
			std::cerr << "Error in receiving data ######" << std::endl;
		} else if (bytes_received == 0) {
			std::cerr << "Connection close" << std::endl;
		} else {
			std::string header;
			std::string body;

			buffer[bytes_received] = '\0';
			if (request->getPhase() == Request::READY) {
				std::string http_request(buffer);

				std::cout << "BUFFER :" << http_request << std::endl;

				size_t header_end = http_request.find("\r\n\r\n");
				if (header_end == std::string::npos) {
					std::cout << "Header end not found1" << std::endl;
				}
				else {
					header = http_request.substr(0, header_end);
									std::cout << "header :" << header << std::endl;

					std::istringstream	iss(header);
					std::string first_line;
					std::getline(iss, first_line);

					std::cout << RED << "First Line : " << first_line << RESET << std::endl;
					request->addMethod(header);
					request->setPhase(Request::ON_HEADER);
				}
			}

			if (request->getPhase() == Request::ON_HEADER) {
				std::string http_request(buffer);

				size_t header_end = http_request.find("\r\n\r\n");
				if (header_end == std::string::npos) {
					std::cout << "Header end not found2" << std::endl;
				}
				else {
					header = http_request.substr(0, header_end);

					std::istringstream	iss(header);
					std::string line;

					std::getline(iss, line);
					while (getline(iss, line)) {
						request->addHeader(line);
					}
				}

				if (header_end != std::string::npos) {
					std::cout << "!= npos : " << header_end << std::endl;
            		body = http_request.substr(header_end + 4);
					request->addContent(body);
					request->setPhase(Request::ON_BODY);
					buffer[0] = 0;
				}
			}

			if (request->getPhase() == Request::ON_BODY) {
				std::string rest(buffer);

				request->addContent(rest);
			}
		}
	}
	connection.setRequest(request);
} */


void	Server::recvRequest(Connection &connection) {
	std::cout << "recvRequest" << std::endl;

	Request	*request = new Request(connection, *this);			

	int bytes_received = 0;
	while (!bytes_received) {
		char buffer[1324];

		bytes_received = recv(connection.getFd(), buffer, sizeof(buffer), 0);
		if (bytes_received == -1) {
			std::cerr << "Error in receiving data ######" << std::endl;
		} else {
			std::cout << "############ " << bytes_received << " ############" << std::endl;

			std::string http_request(buffer);

			size_t header_end = http_request.find("\r\n\r\n");
			if (header_end == std::string::npos) {
				std::cout << "header end not found" << std::endl;
			}
			std::string header = http_request.substr(0, header_end);
			std::string body = http_request.substr(header_end + 4);

			std::istringstream	iss(header);
			std::string line;
			bool isFirstLine = true;

			while (getline(iss, line)) {
				if (isFirstLine) {
					isFirstLine = false;
					std::cout << RED << "First Line : " << line << RESET << std::endl;
					request->addMethod(line);
				} else {
					request->addHeader(line);
				}
			}
			if (body.length())
				request->addContent(body);
			
			std::cout << YELLOW << request->getContent() << RESET << std::endl;

			std::cout << "############" << std::endl;
		}
	}
	connection.setRequest(request);
}

void	Server::initiateResponse(Connection &connection, Location &loc)
{
	std::string file_path = createFilePath(loc.getRootPath(), connection.getRequest()->getRelativPath());
	std::cout << GREEN << file_path << RESET << std::endl;
	std::string header = "HTTP/1.1 200 OK\r\n";
	std::string body = generateResponse(file_path, connection.getRequest()->getRelativPath());
	std::ostringstream oss;
	oss << header << body;
	std::string response = oss.str();
	//std::cout << RED <<oss.str() << RESET << std::endl;
	if (send(connection.getFd(), response.c_str(), response.length(), MSG_NOSIGNAL) == -1)
		std::cerr << "Send failed: " << strerror(errno) << std::endl;
}

void	Server::executeGet(Connection &connection)
{
	std::cout << "executeGet" << std::endl;
	std::vector<Location *>::iterator	it = this->_location.begin();

	while (it != this->_location.end())
	{
		if (connection.getRequest()->getRelativPath() == (*it)->getLocMatcUhri())
		{
			if ((*it)->getRootPath() != "Nothing")
			{
				initiateResponse(connection, *(*it));
				return;
			}
		}
		it++;
	}
	std::cout << "pas de match parfait" << std::endl;

	closestMatch(connection);
}

void	Server::closestMatch(Connection &connection)
{
	int	index = findClosestStringIndex(connection.getRequest()->getRelativPath(), this->_location);
	std::cout << RED << "index vaut = " << RESET << index << std::endl;

	if (index != -1)
		initiateResponse(connection, *this->_location[index]);
	else
	{
		std::cout << GREEN <<"####### solveRequest ######" << RESET << std::endl;
		std::string header = "HTTP/1.1 200 OK\r\n";
		std::string body = "Hello from server!!! Here Adrien\n";
		std::ostringstream oss;
		oss << header << "Content-Length: " << body.length() << "\r\n\r\n" << body;
		std::string response = oss.str();
		if (send(connection.getFd(), response.c_str(), response.length(), 0) == -1)
			std::cerr << "Send failed: " << strerror(errno) << std::endl;
	}
}


void	Server::solveRequest(Connection &connection) {
	ft::display_map(connection.getRequest()->getHeader());
	std::cout << connection.getRequest()->getContent() << std::endl;

	if (connection.getRequest()->getMethod() == GET)
	{
		std::cout << "==GET==" << std::endl;
		executeGet(connection);	
	}
	if (connection.getRequest()->getMethod() == POST)
	{
		std::cout << "==POST==" << std::endl;
		std::cout << GREEN <<"####### solveRequest ######" << RESET << std::endl;
		std::string header = "HTTP/1.1 200 OK\r\n";
		std::string body = "Hello from server!!! Here Adrien\n";
		std::ostringstream oss;
		oss << header << "Content-Length: " << body.length() << "\r\n\r\n" << body;
		std::string response = oss.str();
		if (send(connection.getFd(), response.c_str(), response.length(), 0) == -1)
		std::cerr << "Send failed: " << strerror(errno) << std::endl;

	}
}


void	Server::runRecvAndSolve(Connection &connection) {
	try {
		recvRequest(connection);
	} catch (std::exception &e) {
		std::cerr << "recvRequest error!!!" << std::endl;
	}

	solveRequest(connection);
}

void	Server::addConnection(int client_fd, std::string client_ip, int client_port) {
    Connection *client = new Connection(client_fd, client_ip, client_port);

	std::cout << "IP : " << client_ip <<  std::endl << "PORT : " << client_port << std::endl;
    _connections.insert(std::make_pair(client_fd, client));
}

void	Server::acceptNewConnection() {
	int	client_fd;
	struct sockaddr_in client_addr;

	socklen_t addrlen = sizeof(_server_addr);

	if ((client_fd = accept(_fd, (struct sockaddr *)&client_addr, (socklen_t*)&addrlen)) == -1) {
		std::cerr << "Accept failed: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
	struct timeval timeout;
	timeout.tv_sec = TIMEOUT_SEC;
	timeout.tv_usec = 0;
	if (setsockopt(client_fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) == -1) {
		std::cerr << "Failed to set receive timeout" << std::endl;
		exit(EXIT_FAILURE);
	}

	char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
    int client_port = ntohs(client_addr.sin_port);

	addConnection(client_fd, client_ip, client_port);
}

void	Server::run() {
	if (this->_connections.size() >= (1024 / this->_manager->getServer().size()))
	{
		std::cout << "too many connection, old connection must be closed" << std::endl;
		return ;
	}

	acceptNewConnection();

	std::map<int, Connection *>::iterator it = _connections.begin();
	while (it != _connections.end())
	{
		std::map<int, Connection *>::iterator it2 = it;
		int fd = it2->first;

		runRecvAndSolve(*(it2->second));
		close(fd);
		it++;
	}
}

std::string	Server::createFilePath(std::string root_path, std::string relativ_path)
{
	//std::cout << "root path vaut : " << root_path << " et relative path vaut : " << relativ_path << std::endl;
	trim(root_path);
	trim(relativ_path);
	removeLastSemicolon(root_path);
	adjust(relativ_path);
	//std::cout << "APres root path vaut : " << root_path << " et relative path vaut : " << relativ_path << std::endl;
	std::string file_path = root_path + relativ_path;
	//std::cout << "file path vaut : " << file_path << std::endl;
	return (file_path);
}

int	Server::getPort()
{
	return (this->_port);
}

int	Server::getFd()
{
	return (this->_fd);
}
