#include "response.hpp"
#include "server.hpp"
#include "connection.hpp"

Response::Response(Connection &connection, Server &server)
{
	this->_connection = &connection;
	this->_server = &server;
}

Response::~Response()
{
}

void   Response::execCGI(int client_fd, std::string method, std::string path, std::string param)
 {
    int cgi_output[2];
    int cgi_input[2];

    if (pipe(cgi_output) < 0 || pipe(cgi_input) < 0) {
        perror("pipe");
        return;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return;
    }

    if (pid == 0) { 
        char method_env[255];
        char param_env[255];

        std::string for_method_env = "REQUEST_METHOD=" + method;

        sprintf(method_env, "%s",for_method_env.c_str());
        sprintf(param_env, "QUERY_STRING=%s", param.c_str());

        putenv(method_env);
        putenv(param_env);

        dup2(cgi_output[1], STDOUT_FILENO);
        dup2(cgi_input[0], STDIN_FILENO);

        close(cgi_output[0]);
        close(cgi_input[1]);

        char *argv[] = {const_cast<char*>(path.c_str()), NULL};

        execv(path.c_str(), argv);
        perror("execl");
        exit(1);
    } else {
        close(cgi_output[1]);
        close(cgi_input[0]);

        const char *status_line = "HTTP/1.1 200 OK\r\n";
        send(client_fd, status_line, strlen(status_line), 0);

        char buffer[1024];
        int n;

        while ((n = read(cgi_output[0], buffer, sizeof(buffer) - 1)) > 0) {
            buffer[n] = '\0';
            send(client_fd, buffer, n, 0);
        }

        close(cgi_output[0]);
        close(cgi_input[1]);

        waitpid(pid, NULL, 0);
    }
 }

 void   Response::handleCGI(std::string fp, int client_fd)
 {
    std::string script_path = fp;
    std::string param_string = "";

    if (fp.find("?") != std::string::npos) {
        param_string = fp.substr(fp.find("?") + 1);
        script_path = fp.substr(0, script_path.find("?"));
    }
    execCGI(client_fd, toString(this->_connection->getRequest()->getMethod()), script_path, param_string);
 }

 void   Response::createResponse(std::string fp, std::string status)
 {
    this->_header = "HTTP/1.1 " + status +"\r\n";
    if (fp.find("cgi") != std::string::npos)
    {
        handleCGI(fp, this->_connection->getFd());
		return ;
    }

	this->_body = generateResponse(fp);
	std::ostringstream oss;
	oss << this->_header << this->_body;
	this->_response = oss.str();
}

 void   Response::sendResponse(int fd)
 {
    size_t totalBytesSent = 0;
	size_t dataLength = this->_response.length();
	const char* dataPtr = this->_response.c_str();

	while (totalBytesSent < dataLength) {
		ssize_t bytesSent = send(fd, dataPtr + totalBytesSent, dataLength - totalBytesSent, MSG_NOSIGNAL);
		totalBytesSent += bytesSent;
	}
}

void    Response::listOfDirectory(std::string fp)
{
    this->_header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    removeLastBS(fp);
    this->_body = generateDirectoryListing(fp);
	std::ostringstream oss;
	oss << this->_header << this->_body;
	this->_response = oss.str();
}


int    Response::redirectResponse()
{
    std::string link = this->_server->getRedirectLink();
	int status_code = this->_server->getRedirectStatusCode();
	removeLastSemicolon(link);

   	if (!link.empty() && status_code) {
		std::ostringstream header;
		header << "HTTP/1.1 ";
		header << status_code << " Moved Permanently\r\n";
		header << "Location: " << link << "\r\n";
		header << "Content-Length: 0\r\n";

		std::string response = header.str();

		if (send(this->_server->getFd(), response.c_str(), response.length(), 0) == -1)
			std::cerr << "Send failed: " << strerror(errno) << std::endl;
        return (1);
	}
    return (0);
}

/* == getter == */
Connection*		Response::getConnection() const
{
	return (this->_connection);
}
