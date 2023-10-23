/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 12:51:48 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/10/23 23:23:08 by ybenlafk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Cmds.hpp"

int    removeFromChannel(map_channel &channels, std::string nick, std::string name, vec_client &clients, Client *sender)
{
    size_t s = channels[name].get_clients().size();
    for (size_t j = 0; j < channels[name].get_clients().size(); j++)
        if (channels[name].get_clients()[j].getNickName() == nick)
            channels[name].get_clients().erase(channels[name].get_clients().begin() + j);
    
    if (s == channels[name].get_clients().size())
        return (utils::reply(sender->getFd(), "441 KICK :He's not on that channel\r\n", sender->getPrifex(utils::getHostName())), 1);

    for (map_channel::iterator it = channels.begin(); it != channels.end();)
    {
       if (it->second.get_clients().size() == 0)
            it = channels.erase(it);
        else
            ++it;
    }
    for (size_t i = 0; i < clients.size(); i++)
    {
        if (clients[i].getNickName() == nick)
        {
            if (clients[i].getChannels().find(name) != clients[i].getChannels().end())
                clients[i].getChannels().erase(name);
        }
    }
    return (0);
}

vec_str     getParams(std::string value)
{
    vec_str params;
    std::string param = "";
    size_t i = 0;
    while (value[i] && value[i] != ' ' && value[i] != '\t') param += value[i++];
    if (param[0] != '#') params.push_back("");
    else params.push_back(param);
    param = "";
    while (value[i] && (value[i] == ' ' || value[i] == '\t')) i++;
    while (value[i] && value[i] != ' ' && value[i] != '\t' && value[i] != ':') param += value[i++];
    params.push_back(param);
    param = "";
    while (value[i] && (value[i] == ' ' || value[i] == '\t')) i++;
    if (value[i] == ':')
    {
        i++;
        while (value[i] && (value[i] == ' ' || value[i] == '\t')) i++;
        while (value[i]) param += value[i++];
        params.push_back(param);
    }else params.push_back("");
    return (params);
}

bool    isOperator(vec_client &clients, int fd, std::string name)
{
    for (size_t i = 0; i < clients.size(); i++)
    {
        if (clients[i].getFd() == fd)
        {
            if (clients[i].getChannels().find(name) != clients[i].getChannels().end())
            {
                if (clients[i].getChannels()[name])
                    return (true);
            }
        }
    }
    return (false);
}

void    Cmds::cmdKick(map_channel &channels, vec_client &clients, std::string value, std::string hostname, Client *sender)
{
    vec_str params = getParams(value);
    
    if (!sender) return;
    if (params.size() != 3)
    {
        utils::reply(sender->getFd(), "461 KICK :Not enough parameters\r\n", sender->getPrifex(hostname));
        return ;
    }
    if (params[0] == "" || params[1] == "")
    {
        utils::reply(sender->getFd(), "461 KICK :Not enough parameters\r\n", sender->getPrifex(hostname));
        return ;
    }
    if (channels.find(params[0]) == channels.end())
    {
        utils::reply(sender->getFd(), "403 KICK :No such channel\r\n", sender->getPrifex(hostname));
        return ;
    }
    else 
    {
        if (isOperator(clients, sender->getFd(), params[0]))
        {
            Client *target = NULL;
            for (size_t i = 0; i < clients.size(); i++)
            {
                if (clients[i].getNickName() == params[1])
                {
                    target = &clients[i];
                    break ;
                }
            }
            if (!target)
                return utils::reply(sender->getFd(), "401 KICK :No such nick/channel\r\n", sender->getPrifex(hostname));
            if (params[2] == "")
            {
                utils::reply(sender->getFd(), "KICK " + params[0] + " " + params[1] + " :" + sender->getNickName() + "\r\n", sender->getPrifex(hostname));
                utils::reply(target->getFd(), "KICK " + params[0] + " " + params[1] + " :" + sender->getNickName() + "\r\n", sender->getPrifex(hostname));
            }
            else
            {
                utils::reply(sender->getFd(), "KICK " + params[0] + " " + params[1] + " :" + params[2] + "\r\n", sender->getPrifex(hostname));
                utils::reply(target->getFd(), "KICK " + params[0] + " " + params[1] + " :" + params[2] + "\r\n", sender->getPrifex(hostname));
            }
            if (removeFromChannel(channels, params[1], params[0], clients, sender))
                return ;
        }
        else
            utils::reply(sender->getFd(), "482 KICK :You're not a channel operator\r\n", sender->getPrifex(hostname));
    }
}