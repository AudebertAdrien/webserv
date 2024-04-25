/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motoko <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 14:04:02 by motoko            #+#    #+#             */
/*   Updated: 2024/04/25 15:22:22 by motoko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.hpp"

namespace ft {
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
}
