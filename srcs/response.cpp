/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:51:25 by tlorne            #+#    #+#             */
/*   Updated: 2024/05/22 19:01:04 by motoko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

        sprintf(method_env, for_method_env.c_str());
        sprintf(param_env, "QUERY_STRING=%s", param.c_str());

        putenv(method_env);
        putenv(param_env);

        dup2(cgi_output[1], STDOUT_FILENO);
        dup2(cgi_input[0], STDIN_FILENO);

        close(cgi_output[0]);
        close(cgi_input[1]);

        char *argv[] = {const_cast<char*>(path.c_str()), NULL};

        execv(path.c_str(), argv);
        //execl(path.c_str(), path.c_str(), NULL);
        perror("execl");
        exit(1);
    } else { // Parent process
        close(cgi_output[1]);
        close(cgi_input[0]);

        // first, send status line
        const char *status_line = "HTTP/1.1 200 OK\r\n";
        send(client_fd, status_line, strlen(status_line), 0);


        // second, read the script and send it
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

    if (fp.find("?") != std::string::npos) 
    {

    	std::cout << GREEN << "fp.find" << RESET << std::endl;
        param_string = fp.substr(fp.find("?") + 1);
        script_path = fp.substr(0, script_path.find("?"));
    }

    std::cout << GREEN << "path : "<<script_path << RESET << std::endl;
    std::cout << YELLOW <<"param : " << param_string << RESET << std::endl;
    execCGI(client_fd, toString(this->_connection->getRequest()->getMethod()), script_path, param_string);
 }

 void   Response::createResponse(std::string fp)
 {
    this->_header = "HTTP/1.1 200 OK\r\n";
    if (fp.find("cgi") != std::string::npos)
    {
        std::cout << "cgi part has to be handle : " << RESET << std::endl;
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

 /* == getter == */
Connection*		Response::getConnection() const
{
    return (this->_connection);
}
