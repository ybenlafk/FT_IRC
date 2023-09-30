/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 13:47:58 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/09/29 16:39:15 by ybenlafk         ###   ########.fr       */
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

int     utils::setUpServer(vec_client *clients, int port)
{
    int    ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    clients->push_back(new Client(-1, "", "", "", false));
    if (ServerSocket < 0)
        throw std::runtime_error("socket() failed");

    int optval = 1;
    if (setsockopt(ServerSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
        throw   std::runtime_error("setsockopt() failed");
    if (port < 0 || port > 65535)
        throw   std::runtime_error("Invalid port");
    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    if (bind(ServerSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
        throw   std::runtime_error("bind() failed");

    if (listen(ServerSocket, FD_SETSIZE) < 0)
        throw std::runtime_error("listen() failed");
    std::cout << "Listening on port " << port << std::endl;
    return (ServerSocket);
}
