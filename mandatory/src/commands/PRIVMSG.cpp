/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PRIVMSG.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 12:52:21 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/10/16 10:09:26 by ybenlafk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Cmds.hpp"

bool    isJoined(Client &client, std::string &name)
{
    if (client.getChannels().find(name) != client.getChannels().end())
        return (true);
    return (false);
}

Client *getClientByFd(int fd, vec_client clients)
{
    for (size_t i = 0; i < clients.size(); i++)
    {
        if (clients[i]->getFd() == fd)
            return (clients[i]);
    }
    return (NULL);
}

void    Cmds::cmdPrivmsg(vec_client clients, int fd, std::string value, map_channel &channels)
{
    std::string target = "", msg = "";
    Client *client = getClientByFd(fd, clients);
    if (!client)
        return ;
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
        utils::reply(fd, "412 * :No message to send\r\n", client->getPrifex());
        return ;
    }
    if (target[0] == '#')
    {
        if (channels.find(target) != channels.end())
        {
            Channel *chan = channels[target];
            for (size_t i = 0; i < chan->get_clients().size(); i++)
            {
                if (chan->get_clients()[i].getFd() != fd && isJoined(chan->get_clients()[i], target))
                {
                    msg = utils::strTrim(msg, "\r\n\t: ");
                    utils::reply(chan->get_clients()[i].getFd(), "PRIVMSG " + target + " :" + msg + "\r\n", client->getPrifex());
                }
            }
        }
        else
            utils::reply(fd, "401 * " + target + " :No such nick/channel\r\n", client->getPrifex());
    }
    else
    {
        for (size_t i = 0; i < clients.size(); i++)
        {
            if (clients[i]->getNickName() == target)
            {
                if (clients[i]->getAuth())
                {
                    msg = utils::strTrim(msg, "\r\n\t: ");
                    utils::reply(clients[i]->getFd(), "PRIVMSG " + target + " :" + msg + "\r\n", client->getPrifex());
                    return ;
                }
                else
                {
                    utils::reply(fd, "401 * " + target + " :No such nick/channel\r\n", client->getPrifex());
                    return ;   
                }
                return ;
            }
        }
        utils::reply(fd, "401 * " + target + " :No such nick/channel\r\n", client->getPrifex());
    }
}