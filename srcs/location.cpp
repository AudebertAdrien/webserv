/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:50:29 by tlorne            #+#    #+#             */
/*   Updated: 2024/05/22 18:49:27 by motoko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "location.hpp"

Location::Location(std::string location)
{
    this->_is_multi = 0;

    std::vector<std::string>    lines;
    std::istringstream iss(location);
    std::string line;

    while (std::getline(iss, line))
        lines.push_back(line);

    fullFillLocation(lines);
    std::vector<std::string>    root = getInfo("root");
    size_t i = 0;
    while (i < root.size())
    {
        this->_root_path = root[i];
        i++;
    }
}

Location::~Location()
{
    this->_body_file.clear();
}


std::vector<std::string>    Location::getInfo(std::string key)
{
    std::vector<std::string>    info;

    std::pair<std::multimap<std::string, std::string>::iterator, std::multimap<std::string, std::string>::iterator> range;

    range = this->_body_file.equal_range(key);

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
        i++;
    }

}

void    Location::handleBody(std::string line)
{
    std::istringstream iss(line);
    std::string key;
    std::string value;
    std::string buff;
    int i = 1;

    iss >> key;
    if (this->_body_file.find(key) != this->_body_file.end())
        this->_is_multi = 1;
    while (iss >> buff)
    {
        if (i <= 2)
        {
            value += buff;
            value += " ";
        }
        i++;
    }
    this->_body_file.insert(std::make_pair(key, value));
    std::multimap<std::string , std::string>::iterator  it = this->_body_file.begin();
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

std::string Location::getLocMatchUri() const
{
    return (this->_loc_match_uri);
}

int Location::getIsMulti() const
{
    return (this->_is_multi);
}

std::string Location::getRootPath() const
{
    return (this->_root_path);
}
