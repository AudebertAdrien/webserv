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

	//std::cout << RED << first_line << RESET << std::endl;
	this->_request->addMethod(header);
	this->_request->setPhase(Request::ON_HEADER);
	return true;
}

bool	Connection::parseHeader() {
	std::cout << "parseHeader" << std::endl;
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
	std::cout << "parseBody" << std::endl;
	std::string rest(_buffer);

	_request->addContent(rest);
	return true;
}

void	Connection::recvRequest() {
	this->_request = new Request(*this, *this->_server);			

	int bytes_received = 1;
	while (bytes_received > 0) {

		bytes_received = recv(this->_fd, this->_buffer, sizeof(this->_buffer), 0);
		std::cout << RED << _buffer << RESET << std::endl;
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

			if (this->_request->getPhase() == Request::ON_BODY) {
				parseBody();
			}
		}
	}
} 

void	Connection::handleQuery(std::string &fp) {
    std::string script_path = fp;
    std::string param_string = "";

    if (fp.find("?") != std::string::npos) {
    	std::cout << GREEN << "fp.find" << RESET << std::endl;

        param_string = fp.substr(fp.find("?") + 1);
        script_path = fp.substr(0, script_path.find("?"));
    }

	std::string filename;
    size_t pos = fp.find("filename=");
    if (pos != std::string::npos) {
		filename = fp.substr(pos + 9);
	}

    std::cout << RED << "filename: " << filename <<  RESET << std::endl;
}

void	Connection::solveRequest() {
	ft::displayMap(this->_request->getHeader());
	std::cout << this->_server->getPort() << std::endl;
	std::cout << this->_request->getContent() << std::endl;

	this->_response = new Response(*this, *this->_server);

	int	index = findClosestStringIndex(this->_request->getRelativPath(), this->_server->getLocation());

	std::cout << "getRelativePath: " << this->_request->getRelativPath() << std::endl;
	std::cout << "this->_server->getLocation()[index]: " << this->_server->getLocation()[index]->getRootPath() << std::endl;

	std::string file_path = createFilePath(this->_server->getLocation()[index]->getRootPath(), this->_request->getRelativPath());
	std::cout << GREEN << file_path << RESET << std::endl;

	if (this->_request->getMethod() == GET)	{
		std::cout << "==GET==" << std::endl;

		this->_response->createResponse(file_path);
		this->_response->sendResponse(this->_fd);
	}

	/* == temporary method POST == */
	if (this->_request->getMethod() == POST) {
		std::cout << "==POST==" << std::endl;

		handleQuery(file_path);
		
		std::cout << GREEN <<"####### solveRequest ######" << RESET << std::endl;
		std::string header = "HTTP/1.1 200 OK\r\n";
		std::string body = "Hello from server!!! Method POST processed !\n";
		std::ostringstream oss;
		oss << header << "Content-Length: " << body.length() << "\r\n\r\n" << body;
		std::string response = oss.str();
		if (send(this->_fd, response.c_str(), response.length(), 0) == -1)
			std::cerr << "Send failed: " << strerror(errno) << std::endl;
	}
}
