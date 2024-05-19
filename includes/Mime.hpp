/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mime.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motoko <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 17:06:01 by motoko            #+#    #+#             */
/*   Updated: 2024/05/19 17:06:39 by motoko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MIME_HPP
#define MIME_HPP

#include "Webserv.hpp"

class Mime
{
	public:
		Mime();
		std::string getMimeType(std::string extension);

	private:
		std::map<std::string, std::string> _mime_types;

};

#endif
