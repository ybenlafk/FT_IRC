/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 12:51:48 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/10/19 16:38:31 by ybenlafk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Cmds.hpp"

void    removeFromChannel(map_channel &channels, std::string nick, std::string name, vec_client &clients)
{
    for (map_channel::iterator it = channels.begin(); it != channels.end(); it++)
        if (it->first == name)
            for (size_t j = 0; j < it->second.get_clients().size(); j++)
                if (it->second.get_clients()[j].getNickName() == nick)
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
        if (clients[i].getNickName() == nick)
        {
            if (clients[i].getChannels().find(name) != clients[i].getChannels().end())
                clients[i].getChannels().erase(name);
        }
    }
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

void    Cmds::cmdKick(map_channel &channels, vec_client &clients, int fd, std::string value, std::string hostname)
{
    vec_str params = getParams(value);
    Client *client = utils::getClientByFd(fd, clients);
    if (client == NULL)
    {
        utils::ft_send(fd, "451 KICK :You have not registered\r\n");
        return ;
    }
    if (params.size() != 3)
    {
        utils::reply(fd, "461 KICK :Not enough parameters\r\n", client->getPrifex(hostname));
        return ;
    }
    if (params[0] == "" || params[1] == "")
    {
        utils::reply(fd, "461 KICK :Not enough parameters\r\n", client->getPrifex(hostname));
        return ;
    }
    if (channels.find(params[0]) == channels.end())
    {
        utils::reply(fd, "403 KICK :No such channel\r\n", client->getPrifex(hostname));
        return ;
    }
    else
    {
        if (isOperator(clients, fd, params[0]))
        {
            std::cout << "KICK" << std::endl;
            removeFromChannel(channels, params[1], params[0], clients);
            utils::reply(fd, "KICK " + params[0] + " " + params[1] + " :" + client->getNickName() + "\r\n", client->getPrifex(hostname));
        }
        else
            utils::reply(fd, "482 KICK :You're not a channel operator\r\n", client->getPrifex(hostname));
    }
}