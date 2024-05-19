/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 19:10:06 by tlorne            #+#    #+#             */
/*   Updated: 2024/05/17 19:10:08 by tlorne           ###   ########.fr       */
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
    if (str.empty()) {
        return (0);
    }
	if (str[str.length() - 1] != '/')
    	return (1);
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

int countCommonCharacters(const std::string &s1, const std::string &s2)
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


int findClosestStringIndex(const std::string &target, const std::vector<Location *> &vecteur)
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



/////////// FONCTION TO GENERATE RESPONSE /////////////


std::string loadFileContent3(const std::string& filePath) 
{
	std::ifstream fileStream(filePath.c_str());
	std::string	content;

	content.assign((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
	fileStream.close();

	return (content);
}

std::string loadFileContent(const std::string& filePath) 
{
	std::cout << GREEN << filePath << RESET << std::endl;
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

std::string generateCSSPart(std::string filePath) 
{
    std::string cssContent = loadFileContent(filePath);
    if (cssContent.empty()) 
	{
		std::cerr << RED <<"CSS not find " << strerror(errno) << RESET << std::endl;
        return "";
    }

    std::stringstream part;
    part << "Content-Type: text/css" << "\n"
		 << "Content-Length: " << cssContent.length()
         << "\r\n\r\n"
         << cssContent;
    return (part.str());
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
    part << "Content-Type: text/html" << "\n"
		 << "Content-Length: " << htmlContent.length()
         << "\r\n\r\n"
         << htmlContent;
    return (part.str());
}

std::string generateImagePart(std::string filePath) 
{
    std::string imageData = loadFileContent3(filePath);
    if (imageData.empty()) {
        return "";
    }

    std::stringstream part;
    part << "Content-Type: image/jpeg" << "\n"
		 << "Content-Length: " << imageData.length()
         << "\r\n\r\n"
         << imageData;
    return (part.str());
}

int	lastElem(std::string str)
{
	//std::cout << RED << "WTFFFFFFFF LAST ELEM VAUT " << str[str.length() - 1] << RESET << std::endl;
	if (str[str.length() - 1] == '/')
    	return (1);
	else
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

std::string generateResponse(std::string filePath, std::string relativ_path) 
{
    std::string response;
	//std::cout << YELLOW;
	/* std::cout << "Last ext vaut = " << lastExt(filePath) << std::endl;
	std::cout << "Last elem vaut = " << lastElem(filePath) << RESET << std::endl; */
	if (lastElem(filePath) == 1)
	{
		std::cout << "html part done : " << RESET << std::endl;
    	response += generateHTMLPart(filePath);
	}
	else if (lastExt(filePath) == "css")
    {
		std::cout << "CSS part done : " << RESET << std::endl;
		response += generateCSSPart(filePath);
	} 
	else if (lastExt(filePath) == "jpg")
	{
		std::cout << "jpg part done : " << RESET << std::endl;
		response += generateImagePart(filePath);
	}
	std::cout << RESET << std::endl;
    return (response);
}