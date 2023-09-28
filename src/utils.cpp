/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 13:47:58 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/09/28 21:37:40 by ybenlafk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

std::string utils::getCmd(std::string str, char c)
{
    std::string cmd = "";
    int i = 0;
    while (str[i] && str[i] != c) cmd += str[i++];
    return (cmd);
}

std::string utils::getValue(std::string str, char c)
{
    std::string value = "";
    int i = 0;
    while (str[i] && str[i] != c) i++;
    i++;
    while (str[i]) value += str[i++];
    return (utils::strTrim(value, " \t\r\n"));
}

std::string   utils::strTrim(std::string str, std::string chars)
{
    std::string::size_type pos = str.find_last_not_of(chars);
    if (pos != std::string::npos)
        str.erase(pos + 1);
    else
        str.clear();
    pos = str.find_first_not_of(chars);
    if (pos != std::string::npos)
        str.erase(0, pos);
    return (str);
}

void         utils::spaceSkipe(std::string &str)
{
    size_t i = 0;
    for (; i < str.length(); i++) if (str[i] != ' ')break;
    str.erase(0, i);
}