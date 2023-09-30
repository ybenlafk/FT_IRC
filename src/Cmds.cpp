/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cmds.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 16:42:17 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/09/30 10:33:26 by ybenlafk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cmds.hpp"

void    Cmds::cmdNick(vec_client clients, int fd, std::string nick)
{
    for (size_t i = 0; i < clients.size(); i++)
    {
        if (clients[i]->getFd() != fd && clients[i]->getAuth() == true)
        {
            if (clients[i]->getNickName() == nick)
            {
                std::string msg = "433 * " + nick + " :Nickname is already in use.\r\n";
                send(fd, msg.c_str(), msg.length(), 0);
                return ;
            }
        }
    }
    for (size_t i = 0; i < clients.size(); i++)
    {
        if (clients[i]->getFd() == fd)
        {
            clients[i]->setNickName(nick);
            return ;
        }
    }
}

void   Cmds::cmdQuit(vec_client clients, int fd, std::string msg)
{
    for (size_t i = 0; i < clients.size(); i++)
    {
        if (clients[i]->getFd() == fd)
        {
            std::string ms = "QUIT : " + msg + "\r\n";
            send(fd, ms.c_str(), ms.length(), 0);
            close(fd);
            clients.erase(clients.begin() + i);
            return ;
        }
    }
}

void    Cmds::cmdPrivmsg(vec_client clients, int fd, std::string value)
{
    std::string target = "", msg = "";
    int i = 0;
    while (value[i] && value[i] != ':')
        target += value[i++];
    if (value[i] == ':')
        i++;
    else
    {
        std::string ms = "412 * :No message to send\r\n";
        send(fd, ms.c_str(), ms.length(), 0);
        return ;
    }
    target = utils::strTrim(target, "\r\n\t ");
    while (value[i])
        msg += value[i++];
    if (!target.empty() && target[i] == '#')
    {
        // send to channel
    }
    else
    {
        for (size_t i = 0; i < clients.size(); i++)
        {
            if (clients[i]->getNickName() == target)
            {
                if (clients[i]->getAuth())
                {
                    msg = utils::strTrim(msg, "\r\n\t ");
                    std::string ms = msg + "\r\n";
                    send(clients[i]->getFd(), ms.c_str(), ms.length(), 0);
                    return ;
                }
                else
                {
                    std::string ms = "401 * " + target + " :No such nick/channel\r\n";
                    send(fd, ms.c_str(), ms.length(), 0);
                    return ;   
                }
                return ;
            }
        }
        std::string ms = "401 * " + target + " :No such nick/channel\r\n";
        send(fd, ms.c_str(), ms.length(), 0);
    }
}