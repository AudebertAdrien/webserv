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
    std::cout << BLUE <<"Constructor Location Param called" << RESET << std::endl;
    /*std::vector<std::string>    lines;
    std::istringstream iss(location);
    std::string line;

    while (std::getline(iss, line))
    {
        lines.push_back(line);
        // Traitement de chaque ligne ici
        std::cout << "Ligne : " << line << std::endl;
    }

    std::vector<std::string>::iterator it;

    for(it = lines.begin(), it != lines.end(), it++)
    {
        std::istringstream iss(*it); // Crée un flux d'entrée à partir de la chaîne
        std::string firstWord;
        iss >> firstWord;
        if (firstWord == "location" || firstWord =="}")
        {
            //si fw == location, checker suite pour verif que tout est ok
            continue;
        }
        else
        {
            //comparer le fw avec ce qui est possible 
        }
        
    }*/



}

Location::~Location()
{
    std::cout << "Destructor Location called" << std::endl;
}

