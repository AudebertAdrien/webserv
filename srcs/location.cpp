/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:50:29 by tlorne            #+#    #+#             */
/*   Updated: 2024/05/03 16:58:01 by motoko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "location.hpp"
#include "webserv_macro.hpp"

Location::Location() {
    //std::cout << "Constructor Location default called" << std::endl;
}

Location::Location(std::string location)
{
    this->_is_multi = 0;
	/*
    std::cout << BLUE << "########## YOUR PART ###########" << RESET << std::endl;
    std::cout << BLUE <<"Constructor Location Param called" << RESET << std::endl;

    std::cout << YELLOW << "show string" << std::endl;
    std::cout << location << std::endl;
    std::cout << "end string" << RESET << std::endl <<std::endl;
	*/


    std::vector<std::string>    lines;
    std::istringstream iss(location);
    std::string line;

    while (std::getline(iss, line))
    {
        lines.push_back(line);
        // Traitement de chaque ligne ici
        std::cout << "Loca Ligne : " << line << std::endl;
    }
    fullFillLocation(lines);

    // Useless fonction, just to show location
    showLocation();
    // to erease

    //std::cout << YELLOW << "GET ROOT" << std::endl;
    std::vector<std::string>    root = getInfo("root");
    size_t i = 0;
    while (i < root.size())
    {
        std::cout << "root : " << root[i] << std::endl;
        i++;
    }
    //std::cout << "END TEST ROOT"<< YELLOW << std::endl;
    //std::cout << BLUE << "########## END ###########" << RESET << std::endl;
}

Location::~Location()
{
    //std::cout << "Destructor Location called" << std::endl;
    this->_body_file.clear();
}

static void showMap(std::multimap<std::string , std::string> map)
{
    std::multimap<std::string , std::string>::iterator   it = map.begin();
    
    while(it != map.end())
    {
        std::cout << "Clé : " << it->first << ", Valeur : " << it->second << std::endl;
        it++;
    }
}

void    Location::showLocation()
{
	/*
    std::cout << GREEN << std::endl <<"Show what we get" << std::endl;
    std::cout <<"opt : " <<this->_opt_modif << std::endl;
    std::cout <<"Uri match : " <<this->_loc_match_uri << std::endl;
    std::cout << "Map : " << std::endl;
	*/
    showMap(this->_body_file);
}

std::vector<std::string>    Location::getInfo(std::string key)
{
    std::vector<std::string>    info; // stocker les info a renvoyer

    std::pair<std::multimap<std::string, std::string>::iterator, std::multimap<std::string, std::string>::iterator> range;

    range = this->_body_file.equal_range(key); //range de la valeur key

    std::multimap<std::string, std::string>::iterator it = range.first;
    while (it != range.second)
    {
        info.push_back(it->second);
        it++;
    }
    return (info);
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
        std::cout << "iteration :" << i << " avec le mot :" << word << std::endl;
        if (word == "location" || word == "{")
        {
            i++;
            continue;
        }
        else if (isOpt(word))
        {
            this->_opt_modif = word;
            opt = 1;
        }
        else if ((i == 2 && opt == 1) || (i == 1 && opt == 0))
            this->_loc_match_uri = word;
        else
            std::cout << RED << "Config file not available, format of a location block not accepted"<< RESET <<std::endl;//trow error
        i++;
    }

}

void    Location::handleBody(std::string line)
{
    std::istringstream iss(line);
    std::string key;
    int i = 1;
    std::string value;
    std::string buff;

    iss >> key;
    if (this->_body_file.find(key) != this->_body_file.end())
        this->_is_multi = 1;
    while (iss >> buff)
    {
        if (i <= 2)
        {
            //this->_body_file.insert(std::make_pair(key, value));
            value += buff;
            value += " ";
        }
        else
            std::cout << "Config file not available, format of a location block not accepted"<< std::endl;//trow error
        i++;
    }
    this->_body_file.insert(std::make_pair(key, value));
    // si map
    //if (iss >> error)
    //     std::cout << "Config file not available, format of a location block not accepted"<< std::endl;//trow error
    //else
    //    this->_body_file[key] = value;

    //si Multimap
}

void    Location::fullFillLocation(std::vector<std::string> lines)
{
    size_t  i = 0;
    while (i != lines.size())
    {
        if (i == 0)
            handleFirstLine(lines[0]);
        else
            handleBody(lines[i]);
        i++;
    }
}

std::string Location::getOpt() const
{
    return (this->_opt_modif);
}

std::string Location::getLocMatcUhri() const
{
    return (this->_loc_match_uri);
}

int Location::getIsMulti() const
{
    return (this->_is_multi);
}
