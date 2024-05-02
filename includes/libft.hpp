/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motoko <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 14:03:34 by motoko            #+#    #+#             */
/*   Updated: 2024/04/30 16:57:33 by motoko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <deque>
#include <vector>
#include <stack>

#include "webserv_macro.hpp"

namespace ft {
	void		display_vector(const std::vector<std::string> &ctn);

	std::string getStringFromFile(std::string file_path);
}
