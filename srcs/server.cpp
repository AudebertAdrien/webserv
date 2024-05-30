#include "server.hpp"
#include "server_manager.hpp"
#include "request.hpp"
#include "connection.hpp"

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

	set(server_block);

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
 
    std::vector<std::string>::iterator it;
	for (it = location_block.begin(); it != location_block.end(); it++) {
        this->_location.push_back(new Location(*it));
    }
}

Server::~Server() {
}

void	Server::set(std::string server_block) {
	std::istringstream stream(server_block);
	std::string line;

	while (std::getline(stream, line)) {
		std::istringstream lineStream(line);
		std::string directive;

		lineStream >> directive;

		if (directive == "listen") {
			std::string portStr;
			lineStream >> portStr;
			portStr.resize(portStr.size() - 1);
			this->_port = atoi(portStr.c_str());
		} 
		if (directive == "return") {
			lineStream >> _redirect_status_code;
			lineStream >> _redirect_link;

		}
	}
}

void	Server::addConnection(int client_fd, std::string client_ip, int client_port) {
    Connection *client = new Connection(client_fd, client_ip, client_port, *this);

    _connections.insert(std::make_pair(client_fd, client));
	this->_manager->setFd(client_fd, "_read_set");
}

void	Server::acceptNewConnection() {
	int	client_fd;
	struct sockaddr_in client_addr;

	socklen_t addrlen = sizeof(_server_addr);

	if ((client_fd = accept(_fd, (struct sockaddr *)&client_addr, (socklen_t*)&addrlen)) == -1) {
		std::cerr << "Accept failed: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	int flags = fcntl(client_fd, F_GETFL, 0);
	if (flags == -1) {
		std::cerr << "Failed to get socket flags\n";
		close(client_fd);
		exit(EXIT_FAILURE);
	}

	if (fcntl(client_fd, F_SETFL, flags | O_NONBLOCK) == -1) {
		std::cerr << "Listen failed: " << strerror(errno) << std::endl;
		close(client_fd);
		exit(EXIT_FAILURE);
	}

	char client_ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
	int client_port = ntohs(client_addr.sin_port);

	addConnection(client_fd, client_ip, client_port);
}

void	Server::runRecvAndSolve(Connection &connection) {
	try {
		connection.recvRequest();
		connection.solveRequest();
	} catch (std::exception &e) {
		std::cerr << "error : Recv or Solve" << std::endl;
	}
}

void	Server::run() {
	if (FD_ISSET(this->_fd, &this->_manager->getFdReadSetCopy())) {
		acceptNewConnection();
	}

	std::map<int, Connection *>::iterator it = _connections.begin();
	while (it != _connections.end())
	{
		std::map<int, Connection *>::iterator it2 = it++;
		int fd = it2->first;

		if (FD_ISSET(fd, &this->_manager->getFdReadSetCopy())) {
			runRecvAndSolve(*(it2->second));
			removeFromSet(fd);
			close(fd);
        	this->_connections.erase(it2);
		} 	
	}
}

void	Server::removeFromSet(int fd)
{
	if (FD_ISSET(fd, &this->_manager->getFdReadSet()))
		FD_CLR(fd, &this->_manager->getFdReadSet());
	if (FD_ISSET(fd, &this->_manager->getFdWriteSet()))
		FD_CLR(fd, &this->_manager->getFdWriteSet());
}

int	Server::getPort()
{
	return (this->_port);
}

int	Server::getFd()
{
	return (this->_fd);
}

std::vector<Location *>&	Server::getLocation()
{
	return (this->_location);
}

std::string	Server::getRedirectLink() {
	return (this->_redirect_link);
}

int	Server::getRedirectStatusCode() {
	return (this->_redirect_status_code);
}
