/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motoko <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 14:03:34 by motoko            #+#    #+#             */
/*   Updated: 2024/05/17 18:36:53 by motoko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <deque>
#include <vector>
#include <stack>
#include <map>

#include "webserv_macro.hpp"

namespace ft {
	void	displayVector(const std::vector<std::string> &ctn);
	void	displayMap(const std::map<std::string, std::string> &map);
	void	displayFdSet(const fd_set& set);

	std::string getStringFromFile(std::string file_path);
}
