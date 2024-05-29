/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motoko <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 14:03:34 by motoko            #+#    #+#             */
/*   Updated: 2024/05/20 18:19:50 by motoko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <fstream>
#include <deque>
#include <vector>
#include <stack>
#include <map>

#include "webserv_macro.hpp"

namespace ft {
	void 		clearStringStream(std::istringstream &ss);
	void		trimStr(std::string &str);	
	std::string getStringFromFile(std::string file_path);
	bool		findKeyInMap(const std::map<std::string, std::string>& myMap, const std::string& key);
    
	void	displayMap(const std::map<std::string, std::string> &map);
	void	displayMultiMap(std::multimap<std::string , std::string> map);
	void	displayVector(const std::vector<std::string> &ctn);
	void	displayFdSet(const fd_set& set);
}
