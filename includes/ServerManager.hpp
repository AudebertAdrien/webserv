/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motoko <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 17:30:46 by motoko            #+#    #+#             */
/*   Updated: 2024/04/26 13:56:59 by motoko           ###   ########.fr       */
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
		bool	splitConfigString(std::string &config_string, std::string &config_block, std::vector<std::string> &server_string);
		bool	splitServerString(std::string &server_string, std::string &server_block, std::vector<std::string> &location_string);

	private:
};
