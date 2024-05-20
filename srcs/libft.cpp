/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motoko <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 14:04:02 by motoko            #+#    #+#             */
/*   Updated: 2024/05/20 14:16:55 by motoko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.hpp"

namespace ft {
	void clearStringStream(std::istringstream &ss) {
		ss.clear();
		ss.str("");
	}

	void    trimStr(std::string &str)
	{
		static const char* spaces = " \t";
		str.erase(0, str.find_first_not_of(spaces));
		str.erase(str.find_last_not_of(spaces) + 1);
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

	bool	findKeyInMap(const std::map<std::string, std::string>& myMap, const std::string& key) {
		std::map<std::string, std::string>::const_iterator it = myMap.find(key);

		return it != myMap.end();
	}

	void	displayMap(const std::map<std::string, std::string> &map) {
		std::map<std::string, std::string>::const_iterator it;

		for (it = map.begin(); it != map.end(); it++) {
			std::cout << BLUE << it->first << " : " << it->second << RESET << std::endl;
		}
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
}

