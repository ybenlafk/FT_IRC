/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cmds.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 16:42:17 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/10/03 21:24:05 by ybenlafk         ###   ########.fr       */
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

int          parseJoin(std::string value, map_channel &channels, Client *client)
{
    vec_str  names;
    vec_str  keys;

    utils::split(value, ',', &names, &keys);
    for (size_t i = 0; i < names.size(); i++)
    {
        if (channels.find(names[i]) == channels.end())
        {
            if (keys.size() > i)
            {
                channels[names[i]] = new Channel(names[i], keys[i]);
                client->add_channel(channels[names[i]]);
                client->setAdmin(true);
                channels[names[i]]->add_client(client);
                channels[names[i]]->set_pw(true);
            }
            else
            {
                channels[names[i]] = new Channel(names[i], "");
                client->add_channel(channels[names[i]]);
                client->setAdmin(true);
                channels[names[i]]->add_client(client);
                channels[names[i]]->set_pw(false);
            }
        }
        else
        {
            if (channels[names[i]]->get_pw())
            {
                if (keys.size() > i)
                {
                    if (channels[names[i]]->get_key() == keys[i])
                    {
                        client->add_channel(channels[names[i]]);
                        if (isExist(channels[names[i]]->get_clients(), client->getFd()))
                            channels[names[i]]->add_client(client);
                    }
                    else
                    {
                        // wrong key
                        std::string ms = "475 * " + names[i] + " :Cannot join channel (+k)\r\n";
                        send(client->getFd(), ms.c_str(), ms.length(), 0);
                    }
                }
                else
                {
                    // key needed
                    std::string ms = "475 * " + names[i] + " :Cannot join channel (+k)\r\n";
                    send(client->getFd(), ms.c_str(), ms.length(), 0);
                }
            }
            else
            {
                client->add_channel(channels[names[i]]);
                if (isExist(channels[names[i]]->get_clients(), client->getFd()))
                    channels[names[i]]->add_client(client);
            }
        }
    }
    return (0);
}

void    Cmds::cmdJoin(map_channel &channels, vec_client clients, int fd, std::string value)
{
    for (size_t i = 0; i < clients.size(); i++)
    {
        if (clients[i]->getFd() == fd)
        {
            if (clients[i]->getAuth())
            {
                parseJoin(value, channels, clients[i]);
                for (map_channel::iterator it = channels.begin(); it != channels.end(); it++)
                {
                    std::cout << "name : " << it->first << std::endl;
                    std::cout << "key : " << it->second->get_key() << std::endl;
                    std::cout << "pw : " << (it->second->get_pw() ? "(true)" : "(false)") << std::endl;
                    std::cout << "[" ;
                    for (size_t j = 0; j < it->second->get_clients().size(); j++)
                    {
                        std::cout << it->second->get_clients()[j]->getNickName() << " ";
                        if (it->second->get_clients()[j]->getAdmin())
                            std::cout << "(admin)";
                        std::cout << "| ";
                    }
                    std::cout << "]" << std::endl;
                    std::cout << "---------------------------------------------" << std::endl;
                }
            }
        }
    }
}