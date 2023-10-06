/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JOIN.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 12:52:04 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/10/06 15:23:32 by ybenlafk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Cmds.hpp"

void    printChannels(map_channel &channels)
{
    for (map_channel::iterator it = channels.begin(); it != channels.end(); it++)
    {
        std::cout << "name : " << it->first << std::endl;
        std::cout << "key : " << it->second->get_key() << std::endl;
        std::cout << "pw : " << (it->second->get_pw() ? "(true)" : "(false)") << std::endl;
        std::cout << "[" ;
        for (size_t j = 0; j < it->second->get_clients().size(); j++)
        {
            std::cout << it->second->get_clients()[j].getNickName() << " ";
            if (it->second->get_clients()[j].getChannels().find(it->first) != it->second->get_clients()[j].getChannels().end())
                if (it->second->get_clients()[j].getChannels().find(it->first)->second)
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

void    serv_reply(vec_str names, map_channel &channels, Client *client, size_t i)
{
    utils::reply(client->getFd(), "JOIN :" + names[i] + "\r\n", client->getPrifex());
    utils::reply(client->getFd(), "332 " + names[i] + " : " + channels[names[i]]->get_topic() + "\r\n", client->getPrifex());
    utils::reply(client->getFd(), "353 " + names[i] + " = " + client->getNickName() + " :" + channels[names[i]]->get_members() + "\r\n", client->getPrifex());
    utils::reply(client->getFd(), "366 " + names[i] + " :End of /NAMES list\r\n", client->getPrifex());
    for (size_t j = 0; j < channels[names[i]]->get_clients().size(); j++)
        if (channels[names[i]]->get_clients()[j].getNickName() != client->getNickName())
            utils::reply(channels[names[i]]->get_clients()[j].getFd(), "JOIN :" + names[i] + "\r\n", client->getPrifex());
}

bool    isValidChannel(map_channel &channels, vec_str names, Client *client, size_t i)
{
    if (!channels[names[i]]->get_invite_only())
    {
        // std::cout << channels[names[i]]->get_clients().size() <<" "<< (size_t)channels[names[i]]->get_limit() << std::endl;
        if (channels[names[i]]->get_clients().size() < (size_t)channels[names[i]]->get_limit())
            return (true);
        else
            return (utils::reply(client->getFd(), "471 * " + names[i] + " :Cannot join channel (+l)\r\n", client->getPrifex()), false);
    }
    else
        return (utils::reply(client->getFd(), "473 * " + names[i] + " :Cannot join channel (+i)\r\n", client->getPrifex()), false);
    return (false);
}

void          parseJoin(std::string value, map_channel &channels, Client *client)
{
    vec_str  names;
    vec_str  keys;

    utils::split(value, ',', &names, &keys);
    for (size_t i = 0; i < names.size(); i++)
    {
        if (!utils::isValidName(names[i]))
        {
            utils::reply(client->getFd(), "403 * " + names[i] + " :No such channel\r\n", client->getPrifex());
            continue ;
        }
        if (channels.find(names[i]) == channels.end())
        {
            if (keys.size() > i && !keys[i].empty())
            {
                channels[names[i]] = new Channel(names[i], keys[i]);
                client->add_channel(names[i], true);
                channels[names[i]]->add_client(*client);
                channels[names[i]]->set_pw(true);
            }
            else
            {
                channels[names[i]] = new Channel(names[i], "");
                client->add_channel(names[i], true);
                channels[names[i]]->add_client(*client);
                channels[names[i]]->set_pw(false);
            }
            utils::reply(client->getFd(), "353 " + client->getNickName() + " = " + names[i] + " :" + channels[names[i]]->get_members() + "\r\n", client->getPrifex());
            utils::reply(client->getFd(), "366 " + client->getNickName() + " " + names[i] + " :End of /NAMES list.\r\n", client->getPrifex());
            utils::reply(client->getFd(), "JOIN :" + names[i] + "\r\n", client->getPrifex());
        }
        else
        {
            if (channels[names[i]]->get_pw())
            {
                if (keys.size() > i)
                {
                    if (channels[names[i]]->get_key() == keys[i])
                    {
                        if (isValidChannel(channels, names, client, i))
                        {
                            client->add_channel(names[i], false);
                            if (isClientExist(channels[names[i]]->get_clients(), client->getFd()))
                                channels[names[i]]->add_client(*client);
                            serv_reply(names, channels, client, i);
                        }
                    }
                    else
                        utils::reply(client->getFd(), "475 * " + names[i] + " :Cannot join channel (+k)\r\n", client->getPrifex());
                }
                else
                    utils::reply(client->getFd(), "475 * " + names[i] + " :Cannot join channel (+k)\r\n", client->getPrifex());
            }
            else
            {
                if (isValidChannel(channels, names, client, i))
                {
                    client->add_channel(names[i], false);
                    if (isClientExist(channels[names[i]]->get_clients(), client->getFd()))
                        channels[names[i]]->add_client(*client);
                    serv_reply(names, channels, client, i);
                }
            }
        }
    }
}

void    Cmds::cmdJoin(map_channel &channels, vec_client &clients, int fd, std::string value)
{
    for (size_t i = 0; i < clients.size(); i++)
    {
        if (clients[i]->getFd() == fd)
        {
            if (clients[i]->getAuth())
            {
                parseJoin(value, channels, clients[i]);
                printChannels(channels);
            }
        }
    }
}
