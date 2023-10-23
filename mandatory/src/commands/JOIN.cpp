/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JOIN.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 12:52:04 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/10/23 22:55:38 by ybenlafk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Cmds.hpp"

void    printChannels(map_channel &channels)
{
    for (map_channel::iterator it = channels.begin(); it != channels.end(); it++)
    {
        std::cout << "name : " << it->first << std::endl;
        std::cout << "key : " << it->second.get_key() << std::endl;
        std::cout << "pw : " << (it->second.get_pw() ? "(true)" : "(false)") << std::endl;
        std::cout << "[" ;
        for (size_t j = 0; j < it->second.get_clients().size(); j++)
        {
            std::cout << it->second.get_clients()[j].getNickName() << " ";
            if (it->second.get_clients()[j].getChannels().find(it->first) != it->second.get_clients()[j].getChannels().end())
                if (it->second.get_clients()[j].getChannels().find(it->first)->second)
                    std::cout << "(admin) ";
            std::cout << "| ";
        }
        std::cout << "]" << std::endl;
        std::cout << "---------------------------------------------" << std::endl;
    }
}

bool    isClientExist(vec_member clients, int fd)
{
    for (size_t i = 0; i < clients.size(); i++)
        if (clients[i].getFd() == fd)
            return (false);
    return (true);
}

void    serv_reply(vec_str names, map_channel &channels, Client *client, size_t i, std::string hostname)
{
    for (size_t j = 0; j < channels[names[i]].get_clients().size(); j++)
        if (channels[names[i]].get_clients()[j].getNickName() != client->getNickName())
            utils::reply(channels[names[i]].get_clients()[j].getFd(), "JOIN :" + names[i] + "\r\n", client->getIp());
    
    utils::reply(client->getFd(), "JOIN :" + names[i] + "\r\n", client->getIp());
    utils::reply(client->getFd(), "332 " + client->getNickName() + " " + names[i] + " : " + channels[names[i]].get_topic() + "\r\n", hostname);
    utils::reply(client->getFd(), "353 " + client->getNickName() + " @ " + names[i] + " :" + channels[names[i]].get_members() + "\r\n", hostname);
    utils::reply(client->getFd(), "366 " + client->getNickName() + " " + names[i] + " :End of /NAMES list\r\n", hostname);
}

bool    isValidChannel(map_channel &channels, vec_str names, Client *client, size_t i, std::string hostname)
{
    if (!channels[names[i]].get_invite_only())
    {
        if (channels[names[i]].get_clients().size() < (size_t)channels[names[i]].get_limit())
            return (true);
        else
            return (utils::reply(client->getFd(), "471 * " + names[i] + " :Cannot join channel (+l)\r\n", client->getPrifex(hostname)), false);
    }
    else if (channels[names[i]].get_invite_only() && client->getInvited())
    {
         if (channels[names[i]].get_clients().size() < (size_t)channels[names[i]].get_limit())
            return (true);
        else
            return (utils::reply(client->getFd(), "471 * " + names[i] + " :Cannot join channel (+l)\r\n", client->getPrifex(hostname)), false);
    }
    else
        return (utils::reply(client->getFd(), "473 * " + names[i] + " :Cannot join channel (+i)\r\n", client->getPrifex(hostname)), false);
    return (false);
}

void    Cmds::cmdJoin(map_channel &channels, std::string value, std::string hostname, Client *sender)
{
    vec_str  keys;
    vec_str  names;

    if (!sender) return;
    utils::split(value, ',', &names, &keys);
    for (size_t i = 0; i < names.size(); i++)
    {
        if (!utils::isValidName(names[i]))
        {
            utils::reply(sender->getFd(), "403 * " + names[i] + " :No such channel\r\n", sender->getPrifex(hostname));
            continue ;
        }
        if (channels.find(names[i]) == channels.end())
        {
            if (keys.size() > i && !keys[i].empty())
            {
                channels.insert(std::pair<std::string, Channel>(names[i], Channel(names[i], keys[i])));
                sender->add_channel(names[i], true);
                channels[names[i]].add_client(*sender);
                channels[names[i]].set_pw(true);
            }
            else
            {
                channels.insert(std::pair<std::string, Channel>(names[i], Channel(names[i], "")));
                sender->add_channel(names[i], true);
                channels[names[i]].add_client(*sender);
                channels[names[i]].set_pw(false);
            }
            utils::reply(sender->getFd(), "JOIN :" + names[i] + "\r\n", sender->getIp());
            utils::reply(sender->getFd(), "353 " + sender->getNickName() + " = " + names[i] + " :" + channels[names[i]].get_members() + "\r\n",  hostname);
            utils::reply(sender->getFd(), "366 " + sender->getNickName() + " " + names[i] + " :End of /NAMES list\r\n", hostname);
        }
        else
        {
            if (channels[names[i]].get_pw())
            {
                if (keys.size() > i)
                {
                    if (channels[names[i]].get_key() == keys[i])
                    {
                        if (isValidChannel(channels, names, sender, i, hostname))
                        {
                            if (isClientExist(channels[names[i]].get_clients(), sender->getFd()))
                            {
                                sender->add_channel(names[i], false);
                                channels[names[i]].add_client(*sender);
                                serv_reply(names, channels, sender, i, hostname);
                            }
                        }
                    }
                    else
                        utils::reply(sender->getFd(), "475 * " + names[i] + " :Cannot join channel (+k)\r\n", sender->getPrifex(hostname));
                }
                else
                    utils::reply(sender->getFd(), "475 * " + names[i] + " :Cannot join channel (+k)\r\n", sender->getPrifex(hostname));
            }
            else
            {
                if (isValidChannel(channels, names, sender, i, hostname))
                {
                    if (isClientExist(channels[names[i]].get_clients(), sender->getFd()))
                    {
                        sender->add_channel(names[i], false);
                        channels[names[i]].add_client(*sender);
                        serv_reply(names, channels, sender, i, hostname);
                    }
                }
            }
        }
    }
}
