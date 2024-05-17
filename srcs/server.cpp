/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:50:12 by tlorne            #+#    #+#             */
/*   Updated: 2024/05/17 19:43:28 by motoko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "server_manager.hpp"
#include "request.hpp"

#define PORT 8080
#define BUFFER_SIZE 1024
#define TIMEOUT_SEC 5
const std::string HTML_FILE_PATH = "index.html";

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
 
	/*
	int flags = fcntl(_fd, F_GETFL, 0);
    if (flags == -1) {
        std::cerr << "Failed to get socket flags\n";
        close(_fd);
		exit(EXIT_FAILURE);
    }

	if (fcntl(_fd, F_SETFL, flags | O_NONBLOCK) == -1) {
		std::cerr << "Listen failed: " << strerror(errno) << std::endl;
        close(_fd);
		exit(EXIT_FAILURE);
	}
	*/

	/* std::cout << RED << " !!!!!!!!!! Pour le serveur port num " << this->_port << std::endl << "Location block vaut :" << std::endl;
	for(std::vector<std::string>::iterator	it=location_block.begin(); it != location_block.end(); it++)
	{
		std::cout << *it << std::endl;
	}
	std::cout<< RESET; */
	completeVectorLocation(location_block);
}

Server::~Server() {
}

/*
void	Server::recvRequest(Connection &connection) {
	Request	*request = new Request(connection, *this);			

	int bytes_received = 1;
	while (bytes_received > 0) {
		char	buffer[500];	

		//bytes_received = recv(connection.getFd(), buffer, sizeof(buffer), MSG_DONTWAIT);
		bytes_received = recv(connection.getFd(), buffer, sizeof(buffer), 0);
		if (bytes_received == -1) {
			std::cerr << "Error in receiving data ######" << std::endl;
			break;
		} else if (bytes_received == 0) {
			std::cerr << "Connection close" << std::endl;
			break;
		} else {
			std::string header;
			std::string body;

			buffer[bytes_received] = '\0';
			if (request->getPhase() == Request::READY) {
				std::string http_request(buffer);

				size_t header_end = http_request.find("\r\n\r\n");
				if (header_end == std::string::npos) {
					std::cout << "Header end not found1" << std::endl;
				}
				else {
					header = http_request.substr(0, header_end);

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
} 
*/


void	Server::recvRequest(Connection &connection) {
	std::cout << "recvRequest" << std::endl;

	Request	*request = new Request(connection, *this);			

	int bytes_received = 0;
	while (!bytes_received) {
		char buffer[1200];

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

static std::string loadFileContent3(const std::string& filePath) 
{
	std::ifstream fileStream(filePath.c_str());
	std::string	content;

	content.assign((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
	fileStream.close();

	return (content);
}

static std::string loadFileContent(const std::string& filePath) 
{
	std::cout << GREEN << filePath << RESET << std::endl;
    std::ifstream file(filePath.c_str(), std::ios::binary);
    if (!file) 
	{
		std::cerr << RED <<"doc not find " << strerror(errno) << RESET << std::endl;
        return ""; // Gérer l'erreur de fichier non trouvé
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return (buffer.str());
}

static std::string generateCSSPart(std::string filePath) 
{
    std::string cssContent = loadFileContent(filePath);
    if (cssContent.empty()) 
	{
		std::cerr << RED <<"CSS not find " << strerror(errno) << RESET << std::endl;
        return ""; // Gérer l'erreur de fichier CSS non trouvé
    }

    std::stringstream part;
    part //<< "--boundary\r\n"
         << "Content-Type: text/css" << "\n"
		 << "Content-Length: " << cssContent.length()
         << "\r\n\r\n"
         << cssContent;
    return (part.str());
}

static std::string generateHTMLPart(std::string filePath) 
{
    std::string htmlContent = loadFileContent(filePath + HTML_FILE_PATH);
    if (htmlContent.empty()) 
	{
		std::cerr << RED <<"HTML not find " << strerror(errno) << RESET << std::endl;
        return ""; // Gérer l'erreur de fichier HTML non trouvé
    }

    std::stringstream part;
    part //<< "--boundary\r\n"
         << "Content-Type: text/html" << "\n"
		 << "Content-Length: " << htmlContent.length()
         << "\r\n\r\n"
         << htmlContent;
    return (part.str());
}

static std::string generateImagePart(std::string filePath) 
{
    std::string imageData = loadFileContent3(filePath);
    if (imageData.empty()) {
        return ""; // Gérer l'erreur de fichier image non trouvé
    }

    std::stringstream part;
    part //<< "--boundary\r\n"
         << "Content-Type: image/jpeg" << "\n"
		 << "Content-Length: " << imageData.length()
         << "\r\n\r\n"
         << imageData;
    return (part.str());
}

static std::string generateResponse(std::string filePath, std::string relativ_path) 
{
    std::string response;
	if (relativ_path == "/")
    	response += generateHTMLPart(filePath);
	//std::cout << RED << "HTML part done : " <<response << RESET << std::endl;
	else if (relativ_path == "/css/style.css")
    	response += generateCSSPart(filePath); // Ajouter la partie CSS
	//std::cout << YELLOW << "CSS part done : " <<response << RESET << std::endl;
    else if (relativ_path == "/images/photo.jpg")
		response += generateImagePart(filePath);
    //response += "--boundary--\r\n";
	//std::cout << GREEN << "all part done : " <<response << RESET << std::endl;
    return (response);
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
		//std::cout << YELLOW << "@@@@@@@@@@ root path : " << (*it)->getRootPath() << RESET << std::endl;
		if (connection.getRequest()->getRelativPath() == (*it)->getLocMatcUhri())
		{
			if ((*it)->getRootPath() != "Nothing")
			{
				initiateResponse(connection, *(*it));
				//std::cout << YELLOW << "@@@@@@@@ root path : @@@@@@@ " << (*it)->getRootPath() << std::endl;
				/* std::string file_path = createFilePath((*it)->getRootPath(), connection.getRequest()->getRelativPath());
				std::string header = "HTTP/1.1 200 OK\r\n";
				std::string body = generateResponse(file_path, connection.getRequest()->getRelativPath());
				std::ostringstream oss;
				oss << header << body;
				std::string response = oss.str();
				//std::cout << RED <<oss.str() << RESET << std::endl;
				if (send(connection.getFd(), response.c_str(), response.length(), MSG_NOSIGNAL) == -1)
					std::cerr << "Send failed: " << strerror(errno) << std::endl; */
				return;
			}
		}
		it++;
	}
	std::cout << "pas de match parfait" << std::endl;

	closestMatch(connection);
}

static int countCommonCharacters(const std::string &s1, const std::string &s2)
{
    int commonCount = 0;
    size_t len = std::min(s1.size(), s2.size());

    for (size_t i = 0; i < len; ++i) 
	{
        if (s1[i] == s2[i])
            commonCount++;
    }
    return (commonCount);
}


static int findClosestStringIndex(const std::string &target, const std::vector<Location *> &vecteur)
{
    if (vecteur.empty())
		return -1;

    int closestIndex = 0;
    int maxCommonCharacters = countCommonCharacters(target, vecteur[0]->getLocMatcUhri());

    for (size_t i = 1; i < vecteur.size(); ++i) 
	{
        int commonCharacters = countCommonCharacters(target, vecteur[i]->getLocMatcUhri());
        if (commonCharacters > maxCommonCharacters) {
            closestIndex = i;
            maxCommonCharacters = commonCharacters;
        }
    }

    return (closestIndex);
}

void	Server::closestMatch(Connection &connection)
{
	/* std::vector<Location *>::iterator	it = this->_location.begin();
	
	/* for (it = this->_location.begin(); it != this->_location.end(); it++)
	{
		if ((*it)->getLocMatcUhri() == "/")
		{
			initiateResponse(connection, *(*it));
			/* std::string file_path = createFilePath((*it)->getRootPath(), connection.getRequest()->getRelativPath());
			std::string header = "HTTP/1.1 200 OK\r\n";
			std::string body = generateResponse(file_path, connection.getRequest()->getRelativPath());
			std::ostringstream oss;
			oss << header << body;
			std::string response = oss.str();
			//std::cout << RED <<oss.str() << RESET << std::endl;
			if (send(connection.getFd(), response.c_str(), response.length(), MSG_NOSIGNAL) == -1)
				std::cerr << "Send failed: " << strerror(errno) << std::endl; 
			return;
		}
	}
	
	std::cout << GREEN <<"####### solveRequest ######" << RESET << std::endl;
	std::string header = "HTTP/1.1 200 OK\r\n";
	std::string body = "Hello from server!!! Here Adrien\n";
	std::ostringstream oss;
	oss << header << "Content-Length: " << body.length() << "\r\n\r\n" << body;
	std::string response = oss.str();
	if (send(connection.getFd(), response.c_str(), response.length(), 0) == -1)
		std::cerr << "Send failed: " << strerror(errno) << std::endl; */

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
	ft::displayMap(connection.getRequest()->getHeader());
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
		solveRequest(connection);
	} catch (std::exception &e) {
		std::cerr << "recvRequest error!!!" << std::endl;
	}
}

void	Server::addConnection(int client_fd, std::string client_ip, int client_port) {
    Connection *client = new Connection(client_fd, client_ip, client_port);

	std::cout << "IP : " << client_ip <<  std::endl << "PORT : " << client_port << std::endl;
    _connections.insert(std::make_pair(client_fd, client));
	this->_manager->setFd(client_fd, "_read_set");
	this->_manager->setFd(client_fd, "_write_set");

	std::cout << RED << "READ SET" << RESET << std::endl;
	ft::displayFdSet(this->_manager->getFdReadSet());
	std::cout << RED << "WRITE SET" << RESET << std::endl;
	ft::displayFdSet(this->_manager->getFdWriteSet());
}

void	Server::acceptNewConnection() {
	int	client_fd;
	struct sockaddr_in client_addr;

	socklen_t addrlen = sizeof(_server_addr);

	if ((client_fd = accept(_fd, (struct sockaddr *)&client_addr, (socklen_t*)&addrlen)) == -1) {
		std::cerr << "Accept failed: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	/*
	int flags = fcntl(client_fd, F_GETFL, 0);
    if (flags == -1) {
        std::cerr << "Failed to get socket flags\n";
        close(_fd);
		exit(EXIT_FAILURE);
    }

	if (fcntl(client_fd, F_SETFL, flags | O_NONBLOCK) == -1) {
		std::cerr << "Listen failed: " << strerror(errno) << std::endl;
        close(_fd);
		exit(EXIT_FAILURE);
	}
	*/

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

static void trim(std::string& str) {
    // Supprimer les espaces et les tabulations au début de la chaîne
    std::string::size_type start = str.find_first_not_of(" \t");
    if (start != std::string::npos) {
        str = str.substr(start);
    } else {
        str.clear(); // Si la chaîne est entièrement composée d'espaces ou de tabulations
        return;
    }

    // Supprimer les espaces et les tabulations à la fin de la chaîne
    std::string::size_type end = str.find_last_not_of(" \t");
    if (end != std::string::npos) {
        str = str.substr(0, end + 1);
    } else {
        str.clear(); // Si la chaîne est entièrement composée d'espaces ou de tabulations
    }
}

static void removeLastSemicolon(std::string& str) {
    if (!str.empty() && str[str.size() - 1] == ';') {
        str.erase(str.size() - 1);
    }
}

void split(const std::string& str, char delimiter, std::vector<std::string>& tokens) {
    size_t start = 0;
    size_t end = str.find(delimiter);

    while (end != std::string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find(delimiter, start);
    }
    tokens.push_back(str.substr(start));
}

bool containsDot(const std::string& str) 
{
	if (str.find('.') != std::string::npos)
    	return (1);
	else
		return (0);
}

static int	lastWordaFile(std::string str)
{
	std::vector<std::string> tokens;

	split(str, '/', tokens);
	if (!tokens.empty() && containsDot(tokens.back()))
		return (1);
	else
		return (0);
}

/* static int	lastNotaBS(std::string str)
{
	size_t len = std::strlen(str);
    if (len == 0) {
        return 0;
    }
    return str[len - 1] == '?';
}

static void	adjust(std::string& str)
{
	if (str == "/")
		return;
	else if (lastWordaFile(str) == 1)
		return ;
	else if (lastNotaBS(str) == 1 )
		addBS(str);
	return ;
} */

std::string	Server::createFilePath(std::string root_path, std::string relativ_path)
{
	//std::cout << "root path vaut : " << root_path << " et relative path vaut : " << relativ_path << std::endl;
	trim(root_path);
	trim(relativ_path);
	removeLastSemicolon(root_path);
	//adjust(relativ_path);
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
