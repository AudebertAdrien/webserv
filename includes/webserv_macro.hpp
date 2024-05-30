/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv_macro.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: motoko <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 16:31:28 by motoko            #+#    #+#             */
/*   Updated: 2024/05/22 14:17:23 by motoko           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define	RED		"\e[31m"
#define	GREEN	"\e[32m"
#define	YELLOW	"\e[93m"
#define	BLUE	"\e[34m"
#define RESET	"\e[0m"

enum Method { DEFAULT, GET, HEAD, POST, PUT, DELETE, OPTIONS, TRACE };
enum URIType { DIRECTORY, RESOURCE_FILE, FILE_TO_CREATE, CGI_PROGRAM };