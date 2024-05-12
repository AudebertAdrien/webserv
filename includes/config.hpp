/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:46:10 by tlorne            #+#    #+#             */
/*   Updated: 2024/05/12 17:03:15 by motoko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

#include <iostream>
#include <sstream>

#include "webserv_macro.hpp"

class Config {
	public:
		Config();
		Config(std::string config_block, char **env);
        ~Config();

		std::string	getSoftwareName() const;
		std::string	getSoftwareVers() const;
		std::string	getHttpVers() const;
		std::string	getCgiVers() const;
		
	private:
		std::string	_software_name;
        std::string	_software_version;
		std::string _http_version;
		std::string	_cgi_version;
		char		**_env;
};

#endif
