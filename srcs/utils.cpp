/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 19:10:06 by tlorne            #+#    #+#             */
/*   Updated: 2024/05/22 18:56:33 by motoko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

////////// FONCTION TO CLEAN STRG /////////////

void split(const std::string& str, char delimiter, std::vector<std::string>& tokens)
{
    size_t start = 0;
    size_t end = str.find(delimiter);

    while (end != std::string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find(delimiter, start);
    }
    tokens.push_back(str.substr(start));
}

void trim(std::string& str) 
{
    std::string::size_type start = str.find_first_not_of(" \t");
    if (start != std::string::npos) {
        str = str.substr(start);
    } else {
        str.clear();
        return;
    }

    std::string::size_type end = str.find_last_not_of(" \t");
    if (end != std::string::npos) {
        str = str.substr(0, end + 1);
    } else {
        str.clear();
    }
    return ;
}

void removeLastSemicolon(std::string& str)
{
    if (!str.empty() && str[str.size() - 1] == ';') {
        str.erase(str.size() - 1);
    }
}

bool containsDot(const std::string& str) 
{
	if (str.find('.') != std::string::npos)
    	return (1);
	else
		return (0);
}

int	lastWordaFile(std::string str)
{
	std::vector<std::string> tokens;

	split(str, '/', tokens);
	if (!tokens.empty() && containsDot(tokens.back()))
		return (1);
	else
		return (0);
}

int	lastNotaBS(std::string str)
{
	if (!str.empty() && str[str.length() - 1] != '/')
    	return (1);
    return (0);
}

void	adjust(std::string& str)
{
	if (str == "/")
		return;
	else if (lastWordaFile(str) == 1)
		return ;
	else if (lastNotaBS(str) == 1 )
		str += '/';
	return ;
}

/////////// FONCTION TO CHOOSE THE BEST LOCATION ////////////////


int findClosestStringIndex(const std::string &target, const std::vector<Location *> &vecteur)
{
    if (vecteur.empty())
		return -1;

    int closestIndex = 0;
	long unsigned   maxCommonCharacters = 0;

	std::cout << RED << "target: " << target << RESET << std::endl;
    for (size_t i = 0; i < vecteur.size(); ++i) 
	{
		std::cout << YELLOW << "vecteur[i]->getRootPath(): " << vecteur[i]->getRootPath() << RESET << std::endl;
		std::cout << YELLOW << "vecteur[i]->getLocMatchUri(): " << vecteur[i]->getLocMatchUri() << RESET << std::endl;
		if (target == vecteur[i]->getLocMatchUri())
			return (i);

		if((target.find(vecteur[i]->getLocMatchUri()) == 0) && (vecteur[i]->getLocMatchUri().size() > maxCommonCharacters)) {
			closestIndex = i;
			maxCommonCharacters = vecteur[i]->getLocMatchUri().size();
		}
	}
	std::cout << BLUE << "closesIndex: " << closestIndex << RESET << std::endl;
    return (closestIndex);
}


/////////// FONCTION TO GENERATE RESPONSE /////////////


std::string loadImageFileContent(const std::string& filePath) 
{
	std::cout << GREEN << "loadImageFileContent" << RESET << std::endl;
	std::ifstream fileStream(filePath.c_str(), std::ios::binary);
	std::string	content;

	content.assign((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
	fileStream.close();

	return (content);
}

std::string loadFileContent(const std::string& filePath) 
{
	std::cout << GREEN << "loadFileContent" << RESET << std::endl;
    std::ifstream file(filePath.c_str(), std::ios::binary);
    if (!file) 
	{
		std::cerr << RED <<"doc not find " << strerror(errno) << RESET << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return (buffer.str());
}

std::string generateHTMLPart(std::string filePath) 
{
    std::string htmlContent = loadFileContent(filePath + HTML_FILE_PATH);
    if (htmlContent.empty()) 
	{
		std::cerr << RED <<"HTML not find " << strerror(errno) << RESET << std::endl;
        return "";
    }

    std::stringstream part;
    part << "Content-Type: text/html" << "\r\n"
		 << "Content-Length: " << htmlContent.length()
         << "\r\n\r\n"
         << htmlContent;
    return (part.str());
}

std::string generateHTMLPartWithExt(std::string filePath) 
{
    std::string htmlContent = loadFileContent(filePath);
    if (htmlContent.empty()) 
	{
		std::cerr << RED <<"HTML not find " << strerror(errno) << RESET << std::endl;
        return "";
    }

    std::stringstream part;
    part << "Content-Type: text/html" << "\r\n"
		 << "Content-Length: " << htmlContent.length()
         << "\r\n\r\n"
         << htmlContent;
    return (part.str());
}

std::string generateCSSPart(std::string filePath) 
{
    std::string cssContent = loadFileContent(filePath);
    if (cssContent.empty()) 
	{
		std::cerr << RED <<"CSS not find " << strerror(errno) << RESET << std::endl;
        return "";
    }

    std::stringstream part;
    part << "Content-Type: text/css" << "\r\n"
		 << "Content-Length: " << cssContent.length()
         << "\r\n\r\n"
         << cssContent;
    return (part.str());
}

std::string generateImagePart(std::string filePath) 
{
    std::string imageData = loadImageFileContent(filePath);
    if (imageData.empty()) {
        return "";
    }

    std::stringstream part;
    part << "Content-Type: image/jpeg" << "\r\n"
		 << "Content-Length: " << imageData.length()
         << "\r\n\r\n"
         << imageData;
    return (part.str());
}

std::string generateFaviconPart(std::string filePath) 
{
    std::string imageData = loadImageFileContent(filePath);
    if (imageData.empty()) {
        return "";
    }

    std::stringstream part;
    part << "Content-Type: image/x-icon" << "\r\n"
		 << "Content-Length: " << imageData.length()
         << "\r\n\r\n"
         << imageData;
    return (part.str());
}


int	lastElem(std::string str)
{
	if (str[str.length() - 1] == '/')
    	return (1);
	return (0);
}

std::string lastExt(const std::string &str)
{
	std::vector<std::string> tokens;

	split(str, '/', tokens);
	size_t pos = tokens.back().find('.');
    if (pos != std::string::npos)
        return (tokens.back().substr(pos + 1));
    return "";
}

std::string generateResponse(std::string filePath) 
{
    std::string response;
	std::cout << "###### " << filePath << "######"<< std::endl;
	if (lastElem(filePath) == 1)
	{
		std::cout << "html part done : " << std::endl;
    	return generateHTMLPart(filePath);
	}
    else if (lastExt(filePath) == "html")
    {
        return generateHTMLPartWithExt(filePath);
    }
	else if (lastExt(filePath) == "css")
    {
		std::cout << "CSS part done : " << std::endl;
		return generateCSSPart(filePath);
	} 
	else if (lastExt(filePath) == "jpg")
	{
		std::cout << "jpg part done : " << std::endl;
		return generateImagePart(filePath);
	}
	else if (lastExt(filePath) == "ico")
	{
		std::cout << "ico part done : " << std::endl;
		return generateFaviconPart(filePath);
	}
    return (NULL);
}

std::string	createFilePath(std::string root_path, std::string relativ_path)
{
	trim(root_path);
	trim(relativ_path);
	removeLastSemicolon(root_path);
	adjust(relativ_path);
	std::string file_path = root_path + relativ_path;
	return (file_path);
}

std::string toString(Method method) {
    switch (method) {
        case DEFAULT:
			return "DEFAULT";
        case GET:
            return "GET";
        case POST:
            return "POST";
        case DELETE:
            return "DELETE";
        case PUT:
            return "PUT";
        default:
            return "UNKNOWN";
    }
}
