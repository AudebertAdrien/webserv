/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motoko <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 14:04:02 by motoko            #+#    #+#             */
/*   Updated: 2024/05/17 18:39:54 by motoko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.hpp"

namespace ft {
	void	displayMap(const std::map<std::string, std::string> &map) {
		std::map<std::string, std::string>::const_iterator it;

		for (it = map.begin(); it != map.end(); it++) {
			std::cout << BLUE << it->first << " : " << it->second << RESET << std::endl;
		}
	}

	void	displayVector(const std::vector<std::string> &vector) {
		std::vector<std::string>::const_iterator it;

		for (it = vector.begin(); it != vector.end(); it++) {
			std::cout << BLUE << *it << RESET << std::endl;
		}
	}

	void	displayFdSet(const fd_set& set) {
		std::cout << RED << "File descriptors set:" << RESET << std::endl;
		for (int fd = 0; fd < FD_SETSIZE; ++fd) {
			if (FD_ISSET(fd, &set)) {
				std::cout << fd << std::endl;
			}
		}
	}

	std::string getStringFromFile(std::string file_path) {
		std::ifstream file(file_path.c_str());

		if (!file.is_open()) {
			std::cerr << "Error: Unable to open file." << std::endl;
			exit(EXIT_FAILURE);
		}

		std::string line;
		std::getline(file, line, '\0');

		file.close();
		return (line);
	}

	void displayMultiMap(std::multimap<std::string , std::string> map)
	{
    std::multimap<std::string , std::string>::iterator   it = map.begin();
    
    while(it != map.end())
    {
       std::cout << BLUE << it->first << " : " << it->second << RESET << std::endl;
        it++;
    }
	}
}

