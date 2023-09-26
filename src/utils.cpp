/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 13:47:58 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/09/26 13:48:26 by ybenlafk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

std::string utils::getCmd(std::string str, char c)
{
    std::string cmd = "";
    size_t pos = str.find(c);
    if (pos != std::string::npos)
        cmd = str.substr(0, pos);
    return (cmd);
}

std::string   utils::strTrim(std::string str)
{
    std::string cmd = "";
    size_t pos = str.find(' ');
    if (pos != std::string::npos)
        cmd = str.substr(pos + 1, str.size() - pos);
    return (cmd);
}