/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:51:12 by tlorne            #+#    #+#             */
/*   Updated: 2024/05/28 14:28:22 by motoko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"
#include "server.hpp"
#include "connection.hpp"

Request::Request(Connection &connection, Server &server) {
	this->_connection = &connection;
	this->_server = &server;
	this->_phase = Request::READY;
}

Request::~Request() {
}

void	Request::writeFiles()
{
	for (std::map<std::string, FileData>::const_iterator it = _parsed_data.begin(); it != _parsed_data.end(); ++it) 
	{
        const FileData &file_data = it->second;
        
        std::ios_base::openmode mode = std::ios::out;												
        if (file_data.content_type.find("text/") == std::string::npos) 							
		{																						
            mode |= std::ios::binary;															
        }																						
		
		
		std::string file_path = "/home/tlorne/Webserv/git_webserv/upload/" + file_data.filename;
																								
        std::ofstream outfile(file_path.c_str(), mode);											
        if (outfile.is_open()) {																
            outfile.write(file_data.content.c_str(), file_data.content.size());					
            outfile.close();																	
            std::cout << "Successfully wrote " << file_data.filename << std::endl;				
        } 																						
		else 																					
		{																						
            std::cerr << "Failed to write " << file_data.filename << std::endl;					
        }																						
	}
}

/* == setter == */
void	Request::addHeader(std::string &line) {
	std::string key, value;

	size_t pos = line.find(":");
	if (pos != std::string::npos) {
		std::string key = line.substr(0, pos);
		std::string value = line.substr(pos + 1);

		this->_headers.insert(std::make_pair(key, value));
		if (value.find(";") != std::string::npos)
		{
			std::string key2, value2;
			key2 = line.substr(value.find(";"), value.find("="));
			value2 = line.substr(value.find("=") + 1);
			this->_headers.insert(std::make_pair(key2, value2));
			this->_upload = 1;
		}
	}
}

void	Request::addMethod(std::string &line) {
	std::istringstream iss(line);
	std::string	method;

	iss >> method;
	if (method == "GET")
		this->_method = GET;
	if (method == "POST")
		this->_method = POST;
	if (method == "DELETE")
		this->_method = DELETE;
	if (method == "HEAD")
		this->_method = HEAD;
	if (method == "PUT")
		this->_method = PUT;
	if (method == "OPTIONS")
		this->_method = OPTIONS;
	if (method == "TRACE")
		this->_method = TRACE;
	if (method == "DEFAULT")
		this->_method = DEFAULT;
	iss >> this->_relativ_path;
}

void	Request::addContent(std::string &content) {
	this->_content += content;	
}

void	Request::setPhase(Phase new_phase) {
	this->_phase = new_phase;
}

/* == getter == */
std::map<std::string, std::string>	Request::getHeader() const
{
	return (_headers);
}

Connection	Request::getConnection() const
{
	return (*_connection);
}

Server	Request::getServer() const
{
	return (*_server);
}

Method	Request::getMethod() const
{
	return (_method);
}

std::string	Request::getContent() const
{
	return (_content);
}

Location	Request::getLocation() const
{
	return (*_location);
}

std::string	Request::getUri() const
{
	return (_uri);
}

URIType	Request::getUriType() const
{
	return (_uri_type);
}

Request::Phase	Request::getPhase() const
{
	return (_phase);
}

std::string		Request::getRelativPath() const
{
	return (this->_relativ_path);
}

int	Request::getUpload() const
{
	return (this->_upload);
}

std::map<std::string, FileData>		Request::getParsedData()
{
	return (this->_parsed_data);
}
