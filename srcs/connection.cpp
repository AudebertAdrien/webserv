#include "connection.hpp"
#include "request.hpp"
#include "server.hpp"
#include "response.hpp"

Connection::Connection(int client_fd, std::string client_ip, int client_port, Server &server)
{
    this->_fd = client_fd;
    this->_client_ip = client_ip;
    this->_client_port = client_port;
    this->_server = &server;
}

Connection::~Connection() {
	std::cout << "destructeur CONNECTION called" << std::endl;
}

/* == getter == */
Request*	Connection::getRequest() const {
	return (this->_request);
}

Response*	Connection::getRespons() const{
	return (this->_response);
}

int Connection::getFd() const
{
    return (this->_fd);
}

std::string	Connection::getClientIp() const
{
	return (this->_client_ip);
}

int	Connection::getClientPort() const
{
	return (this->_client_port);
}

bool	Connection::parseStartLine() {
	//std::cout << "parseStartLine" << std::endl;
	std::string http_request = _buffer;

	size_t header_end = http_request.find("\r\n\r\n");

	if (header_end == std::string::npos) {
		std::cout << "parseStartLine: Header end not found" << std::endl;
	}
	std::string header = http_request.substr(0, header_end);

	std::istringstream	iss(header);
	std::string first_line;
	std::getline(iss, first_line);

	std::cout << RED << first_line << RESET << std::endl;
	this->_request->addMethod(first_line);
	this->_request->setPhase(Request::ON_HEADER);
	return true;
}

bool	Connection::parseHeader() {
	//std::cout << "parseHeader" << std::endl;
	std::string http_request = _buffer;

	size_t header_end = http_request.find("\r\n\r\n");
	if (header_end == std::string::npos) {
		std::cout << "parseHeader: Header end not found" << std::endl;
	}

	std::string header = http_request.substr(0, header_end);

	std::istringstream	iss(header);
	std::string line;

	std::getline(iss, line);
	while (std::getline(iss, line)) {
		this->_request->addHeader(line);
	}

	if (ft::findKeyInMap(this->_request->getHeader(), "Content-Length")) {
		std::string body = http_request.substr(header_end + 4);
		this->_request->addContent(body);
		_buffer[0] = 0;
		this->_request->setPhase(Request::ON_BODY);
	}
	return true;
}

bool	Connection::parseBody() {
	//std::cout << "parseBody" << std::endl;
	std::string rest(_buffer);

	_request->addContent(rest);
	return true;
}


void	Connection::recvRequest() {
	this->_request = new Request(*this, *this->_server);			

	int bytes_received = 1;
	while (bytes_received > 0) {

		bytes_received = recv(this->_fd, this->_buffer, sizeof(this->_buffer), 0);
		if (bytes_received == -1) {
			break;
		} else if (bytes_received == 0) {
			std::cerr << "Connection close" << std::endl;
			break;
		} else {
			this->_buffer[bytes_received] = '\0';

			if (this->_request->getPhase() == Request::READY) {
				parseStartLine();
			}

			if (this->_request->getPhase() == Request::ON_HEADER) {
				parseHeader();
			}
			//ft::displayMap(this->_request->getHeader());
			if (this->_request->getPhase() == Request::ON_BODY) {
				parseBody();
			}
		}
	}
} 

void	Connection::solveRequest() {
	ft::displayMap(this->_request->getHeader());
	std::cout << this->_server->getPort() << std::endl;
	std::cout << this->_request->getContent() << std::endl;

	this->_response = new Response(*this, *this->_server);

	int	index = findClosestStringIndex(this->_request->getRelativPath(), this->_server->getLocation());

	std::cout << "getRelativePath: " << this->_request->getRelativPath() << std::endl;
	std::cout << "this->_server->getLocation()[index]: " << this->_server->getLocation()[index]->getRootPath() << std::endl;

	if (this->_request->getMethod() == GET)	{
		std::cout << "==GET==" << std::endl;
		std::cout << "check allow method" << std::endl;
		if (checkAllowMethod(this->_server->getLocation()[index]->getInfo("allow_methods:"), "GET") == 0)
		{
			this->_response->createResponse("/home/tlorne/Webserv/git_webserv/default_error_pages/405.html", "405 Method Not Allowed");
			this->_response->sendResponse(this->_fd);
			std::cout << RED <<"Wrong method" << RESET << std::endl;
			return ;
		}
		if (checkAllowMethod(this->_server->getLocation()[index]->getInfo("autoindex"), "on;") == 1)
		{
			std::string file_path = createFilePath(this->_server->getLocation()[index]->getRootPath(), this->_request->getRelativPath());
			std::cout << GREEN << file_path << RESET << std::endl;
			if (containsDot(this->_request->getRelativPath()) == 0)
			{
				std::cout << "###############################3est un repertoir" << std::endl;
				this->_response->listOfDirectory(file_path);
			}		
			else													// c'est un fichier
			{
				removeLastBS(file_path);
				if (fileExists(file_path.c_str()) == 0) // est ce que le fichier existe ?
					this->_response->createResponse("/home/tlorne/Webserv/git_webserv/default_error_pages/404.html", "404 Not Found");
				if (fileExists(file_path.c_str()) == 1 && hasAccess(file_path.c_str(), R_OK) == 0) // est ce que le client peut y acceder ? 
					this->_response->createResponse("/home/tlorne/Webserv/git_webserv/default_error_pages/403.html", "403 Forbidden");
				if (fileExists(file_path.c_str()) ==  1 && hasAccess(file_path.c_str(), R_OK) == 1)
					this->_response->createResponse(file_path, "200 OK");
			}
			this->_response->sendResponse(this->_fd);
			return ;
		}
		std::string file_path = createFilePath(this->_server->getLocation()[index]->getRootPath(), this->_request->getRelativPath());
		std::cout << GREEN << file_path << RESET << std::endl;
		this->_response->createResponse(file_path, "200 OK");
		this->_response->sendResponse(this->_fd);
		return ;
	}

	/* == temporary method POST == */
	else if (this->_request->getMethod() == POST) 
	{
		std::cout << "==POST==" << std::endl;
		std::cout << "check allow method" << std::endl;
		/* if (checkAllowMethod(this->_server->getLocation()[index]->getInfo("allow_methods:"), "POST") == 0)
		{
			this->_response->createResponse("/home/tlorne/Webserv/git_webserv/default_error_pages/405.html", "405 Method Not Allowed");
			this->_response->sendResponse(this->_fd);
			std::cout << RED <<"Wrong method" << RESET << std::endl;
		} */
		if (this->_request->getUpload() == 1)
		{
			this->_request->writeFiles();
			std::string response = 
        		"HTTP/1.1 201 Created\r\n"
        		"Content-Type: text/plain\r\n"
        		"Content-Length: 23\r\n"
        		"\r\n"
        		"File uploaded successfully";
			if (send(this->_fd, response.c_str(), response.length(), 0) == -1)
				std::cerr << "Send failed: " << strerror(errno) << std::endl;
		}
		else
		{
			std::cout << GREEN <<"####### solveRequest ######" << RESET << std::endl;
			std::string header = "HTTP/1.1 200 OK\r\n";
			std::string body = "Hello from server!!! Method POST processed !\n";
			std::ostringstream oss;
			oss << header << "Content-Length: " << body.length() << "\r\n\r\n" << body;
			std::string response = oss.str();
			if (send(this->_fd, response.c_str(), response.length(), 0) == -1)
				std::cerr << "Send failed: " << strerror(errno) << std::endl;
		}
		return ;
	}

	else if (this->_request->getMethod() == DELETE)
	{
		std::cout << GREEN <<"do something" << RESET << std::endl;
		return ;
	}
	else
	{
		int met = this->_request->getMethod();
		std::cout << RED << "!!!!!!!!! Error, met vaut = " << met << "   rappel 1=GET, 2=HEAD, 3=POST, 4=PUT, 5=DELETE, 6=OPTION 7=TRACE" << RESET <<std::endl;
		if (met == DEFAULT || met == HEAD || met == PUT || met == OPTIONS || met == TRACE)
		{
			this->_response->createResponse("/home/tlorne/Webserv/git_webserv/default_error_pages/501.html", "501 Not Implemented");
			this->_response->sendResponse(this->_fd);
			return ;
		}
		else
		{
			this->_response->createResponse("/home/tlorne/Webserv/git_webserv/default_error_pages/400.html", "400 Bad Request");
			this->_response->sendResponse(this->_fd);
			return ;
		}
	}
}
