/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motoko <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 17:30:46 by motoko            #+#    #+#             */
/*   Updated: 2024/04/25 16:59:07 by motoko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <vector>
#include <sstream>

class ServerManager {
	public:
		ServerManager();
		~ServerManager();

		void	createServer(const std::string &configuration_file_path, char **env);
		void	splitConfigString(std::string &config_string, std::string &config_block, std::vector<std::string> &server_string);
	private:
};
