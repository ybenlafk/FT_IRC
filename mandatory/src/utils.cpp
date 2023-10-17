/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbadr <sbadr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 13:47:58 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/10/17 13:15:54 by sbadr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"


Client *utils::getClientByFd(int fd, vec_client clients)
{
    for (size_t i = 0; i < clients.size(); i++)
    {
        if (clients[i]->getFd() == fd)
            return (clients[i]);
    }
    return (NULL);
}


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
    std::memset(&serverAddress, 0, sizeof(serverAddress));
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

void    utils::split(std::string str, char c, vec_str *names, vec_str *keys)
{
    size_t i = 0;
    std::string word;

    while (str[i] && str[i] != ' ' && str[i] != '\t')
    {
        if (str[i] == c)
        {
            names->push_back(word);
            word = "";
        }
        else
            word += str[i];
        i++;
    }
    if (!word.empty())
        names->push_back(word);
    word = "";
    while (str[i] && (str[i] == ' ' || str[i] == '\t')) i++;
    
    while (str[i] && str[i] != '\r' && str[i] != '\n')
    {
        if (str[i] == c)
        {
            keys->push_back(word);
            word = "";
        }
        else
            word += str[i];
        i++;
    }
    if (!word.empty())
        keys->push_back(word);
}

void        utils::ft_send(int fd, std::string msg) {send(fd, msg.c_str(), msg.length(), 0);}

bool         utils::isValidName(std::string name)
{
    if (name[0] != '#')
        return (false);
    if (name.length() < 2)
        return (false);
    for (size_t i = 1; i < name.length(); i++)
        if (!isalnum(name[i]) && name[i] != '_')
            return (false);
    return (true);
}

std::string utils::getHostName()
{
    if (std::system("uname -n > /tmp/hostname") == -1)
        throw std::runtime_error("system() failed");
    std::ifstream file("/tmp/hostname");
    if (!file.is_open())
        throw std::runtime_error("open() failed");
    std::string hostname;
    std::getline(file, hostname);
    file.close();
    if (std::system("rm -rf /tmp/hostname") == -1)
        throw std::runtime_error("system() failed");
    return (hostname);
}

int    utils::split(std::string str, char c, vec_str *names, std::string *reason)
{
    size_t i = 0;
    std::string word;

    while (str[i] && str[i] != ' ' && str[i] != '\t' && str[i] != ':')
    {
        if (str[i] == c)
        {
            names->push_back(word);
            word = "";
        }
        else
            word += str[i];
        i++;
    }
    if (!word.empty())
        names->push_back(word);
    else
        return (0);
    word = "";
    while (str[i] && (str[i] == ' ' || str[i] == '\t')) i++;
    if (str[i]  != ':')
        return (0);
    i++;
    while (str[i] && str[i] != '\r' && str[i] != '\n')
    {
        word += str[i];
        i++;
    }
    *reason = word;
    return (1);
}

void    utils::reply(int fd, std::string msg, std::string prefix)
{
    msg = ":" + prefix + " " + msg;
    utils::ft_send(fd, msg);
}

std::string     utils::get_ip(sockaddr_in user_addr)
{
    return (inet_ntoa(user_addr.sin_addr));
}