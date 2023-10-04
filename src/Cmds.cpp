/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cmds.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 16:42:17 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/10/04 14:10:01 by ybenlafk         ###   ########.fr       */
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
                utils::ft_send(fd, "433 * " + nick + " :Nickname is already in use\r\n");
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
            utils::ft_send(fd, "QUIT : " + msg + "\r\n");
            close(fd);
            clients.erase(clients.begin() + i);
            return ;
        }
    }
}

bool    isJoined(Client &client, std::string &name)
{
    if (client.getChannels().find(name) != client.getChannels().end())
        return (true);
    return (false);
}

void    Cmds::cmdPrivmsg(vec_client clients, int fd, std::string value, map_channel &channels)
{
    std::string target = "", msg = "";
    int i = 0;
    while (value[i] && value[i] != ':')
        target += value[i++];
    if (value[i] == ':')
        i++;
    else
    {
        utils::ft_send(fd, "412 * :No message to send\r\n");
        return ;
    }
    target = utils::strTrim(target, "\r\n\t ");
    while (value[i])
        msg += value[i++];
    if (target.empty() || msg.empty())
    {
        utils::ft_send(fd, "412 * :No message to send\r\n");
        return ;
    }
    if (target[0] == '#')
    {
        if (channels.find(target) != channels.end())
        {
            Channel *chan = channels[target];
            for (size_t i = 0; i < chan->get_clients().size(); i++)
            {
                if (chan->get_clients()[i]->getFd() != fd && isJoined(*chan->get_clients()[i], target))
                {
                    msg = utils::strTrim(msg, "\r\n\t ");
                    utils::ft_send(chan->get_clients()[i]->getFd(), "PRIVMSG " + target + " :" + msg + "\r\n");
                }
            }
        }
        else
            utils::ft_send(fd, "402 * " + target + " :No such nick/channel\r\n");
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
                    utils::ft_send(fd, "401 * " + target + " :No such nick/channel\r\n");
                    return ;   
                }
                return ;
            }
        }
        utils::ft_send(fd, "401 * " + target + " :No such nick/channel\r\n");
    }
}

int          parseJoin(std::string value, map_channel &channels, Client *client)
{
    vec_str  names;
    vec_str  keys;

    utils::split(value, ',', &names, &keys);
    for (size_t i = 0; i < names.size(); i++)
    {
        if (!utils::isValidName(names[i]))
        {
            utils::ft_send(client->getFd(), "403 * " + names[i] + " :No such channel\r\n");
            continue ;
        }
        if (channels.find(names[i]) == channels.end())
        {
            if (keys.size() > i)
            {
                channels[names[i]] = new Channel(names[i], keys[i]);
                client->add_channel(names[i], true);
                channels[names[i]]->add_client(client);
                channels[names[i]]->set_pw(true);
            }
            else
            {
                channels[names[i]] = new Channel(names[i], "");
                client->add_channel(names[i], true);
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
                        client->add_channel(names[i], false);
                        if (isExist(channels[names[i]]->get_clients(), client->getFd()))
                            channels[names[i]]->add_client(client);
                    }
                    else
                        utils::ft_send(client->getFd(), "475 * " + names[i] + " :Cannot join channel (+k)\r\n");
                }
                else
                    utils::ft_send(client->getFd(), "475 * " + names[i] + " :Cannot join channel (+k)\r\n");
            }
            else
            {
                client->add_channel(names[i], false);
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
                        if (it->second->get_clients()[j]->getChannels().find(it->first) != it->second->get_clients()[j]->getChannels().end())
                            if (it->second->get_clients()[j]->getChannels().find(it->first)->second)
                                std::cout << "(admin) ";
                        std::cout << "| ";
                    }
                    std::cout << "]" << std::endl;
                    std::cout << "---------------------------------------------" << std::endl;
                }
            }
        }
    }
}