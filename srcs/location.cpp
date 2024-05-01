/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:50:29 by tlorne            #+#    #+#             */
/*   Updated: 2024/04/22 12:50:31 by tlorne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "location.hpp"
#include "Webserv_macro.hpp"

Location::Location()
{
    std::cout << "Constructor Location default called" << std::endl;
}

Location::Location(std::string  location)
{
    std::cout << BLUE << "########## YOUR PART ###########" << RESET << std::endl;
    std::cout << BLUE <<"Constructor Location Param called" << RESET << std::endl;

    std::cout << "show string" << std::endl;
    std::cout << location << std::endl;
    std::cout << "end string" << std::endl <<std::endl;


    std::vector<std::string>    lines;
    std::istringstream iss(location);
    std::string line;

    while (std::getline(iss, line))
    {
        lines.push_back(line);
        // Traitement de chaque ligne ici
        std::cout << "Ligne : " << line << std::endl;
    }
    fullFillLocation(lines);
    std::cout << BLUE << "########## END ###########" << RESET << std::endl;
}

Location::~Location()
{
    std::cout << "Destructor Location called" << std::endl;
}

static int  isOpt(std::string word)
{
    std::string opt[] = {"=", "~", "~*", "^~"};

    for (int i = 0; i < 4; i++)
    {
        if (word == opt[i])
            return (1);
    }
    return (0);
}

void    Location::handleFirstLine(std::string line)
{
    std::istringstream iss(line);
    std::string word;

    int i = 0;
    int opt = 0;
    while (iss >> word)
    {
        if (word == "location")
            continue;
        else if (isOpt(word))
        {
            this->_opt_modif = word;
            opt = 1;
        }
        else if ((i == 2 && opt == 1) || (i == 1 && opt == 0))
            this->_loc_match_uri = word;
        else
            std::cout << "Config file not available, format of a location block not accepted"<< std::endl;//trow error
    }

}

void    Location::handleBody(std::string line)
{
    std::istringstream iss(line);
    std::string key;
    std::string value;
    std::string error;

    iss >> key;
    iss >> value;
    if (iss >> error)
         std::cout << "Config file not available, format of a location block not accepted"<< std::endl;//trow error
    else
        this->_body_file[key] = value;
}

void    Location::fullFillLocation(std::vector<std::string> lines)
{
    size_t  i = 0;
    while (i != lines.size())
    {
        if (i = 0)
            handleFirstLine(lines[0]);
        else
            handleBody(lines[i]);
        i++;
    }
}
