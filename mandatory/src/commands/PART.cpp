/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PART.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 12:52:15 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/10/23 20:51:29 by ybenlafk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Cmds.hpp"

void    removeFromChannel(map_channel &channels, int fd, std::string name, vec_client &clients)
{
    for (map_channel::iterator it = channels.begin(); it != channels.end(); it++)
        if (it->first == name)
            for (size_t j = 0; j < it->second.get_clients().size(); j++)
                if (it->second.get_clients()[j].getFd() == fd)
                    it->second.get_clients().erase(it->second.get_clients().begin() + j);
    for (map_channel::iterator it = channels.begin(); it != channels.end(); it++)
    {
       if (it->second.get_clients().size() == 0)
       {
           channels.erase(it);
           break;
       }
    }
    for (size_t i = 0; i < clients.size(); i++)
    {
        if (clients[i].getFd() == fd)
        {
            if (clients[i].getChannels().find(name) != clients[i].getChannels().end())
                clients[i].getChannels().erase(name);
        }
    }
}

void    sendToMembers(map_channel &channels, int fd, std::string name, vec_client &clients, std::string hostname)
{
    Client *client = NULL;
    
    if (channels.find(name) != channels.end())
        for (size_t i = 0; i < clients.size(); i++)
            if (clients[i].getFd() == fd)
                client = &clients[i];
    if (client == NULL)
        return ;
    if (channels.find(name) != channels.end())
    {
        for (size_t i = 0; i < channels[name].get_clients().size(); i++)
        {
            if (channels[name].get_clients()[i].getFd() != fd)
                utils::reply(channels[name].get_clients()[i].getFd(), "PART " + name + "\r\n", client->getPrifex(hostname));
        }
    }
}

void    Cmds::cmdPart(map_channel &channels, vec_client &clients, std::string value, std::string hostname, Client *sender)
{
    std::string reason = "";
    vec_str names;

    if (!sender || (sender && sender->getAuth() == false))
    {
        utils::reply(sender->getFd(), "451 * :You have not registered\r\n", hostname);
        return ;
    }

    if (utils::split(value, ',', &names, &reason) == 0)
    {
        utils::reply(sender->getFd(), "461 PART :Not enough parameters\r\n", sender->getPrifex(hostname));
        return ;
    }
    for (size_t j = 0; j < names.size(); j++)
    {
        if (channels.find(names[j]) != channels.end())
        {
            if (isJoined(*sender, names[j]))
            {
                if (reason.empty())
                    utils::reply(sender->getFd(), "PART " + names[j] + "\r\n", sender->getPrifex(hostname));
                else
                    utils::reply(sender->getFd(), "PART " + names[j] + " :" + reason + "\r\n", sender->getPrifex(hostname));
                sender->getChannels().erase(names[j]);
                removeFromChannel(channels, sender->getFd(), names[j], clients);
                sendToMembers(channels, sender->getFd(), names[j], clients, hostname);
            }
            else
                utils::reply(sender->getFd(), "442 * " + names[j] + " :You're not on that channel\r\n", sender->getPrifex(hostname));
        }
        else
            utils::reply(sender->getFd(), "403 * " + names[j] + " :No such channel\r\n", sender->getPrifex(hostname));
    }
}