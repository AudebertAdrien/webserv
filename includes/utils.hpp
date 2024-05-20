/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorne <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 19:09:33 by tlorne            #+#    #+#             */
/*   Updated: 2024/05/17 19:09:41 by tlorne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

#include <iostream>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstring>

#include "location.hpp"
#include "request.hpp"

const std::string HTML_FILE_PATH = "index.html";

// FONCTION TO CHANGE METHODE TO STRG
std::string toString(Method method);

// FONCTION TO CLEAN STRG
void        trim(std::string& str);
void        removeLastSemicolon(std::string& str);
bool        containsDot(const std::string& str);
int         lastWordaFile(std::string str);
int         lastNotaBS(std::string str);
void        adjust(std::string& str);
void        split(const std::string& str, char delimiter, std::vector<std::string>& tokens);


//FONCTION TO CHOOSE THE GOOD LOCATION
int         countCommonCharacters(const std::string &s1, const std::string &s2);
int         findClosestStringIndex(const std::string &target, const std::vector<Location *> &vecteur);


//FONCTION TO GENERATE RESPONSE
std::string loadFileContent3(const std::string& filePath);
std::string loadFileContent(const std::string& filePath);
std::string generateCSSPart(std::string filePath);
std::string generateHTMLPart(std::string filePath);
std::string generateImagePart(std::string filePath);
int	        lastElem(std::string str);
std::string lastExt(const std::string &str);
std::string generateResponse(std::string filePath, std::string relativ_path);
std::string	createFilePath(std::string root_path, std::string relativ_path);

#endif