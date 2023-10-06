/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PRIVMSG.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 12:52:21 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/10/06 12:53:31 by ybenlafk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Cmds.hpp"

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
                if (chan->get_clients()[i].getFd() != fd && isJoined(chan->get_clients()[i], target))
                {
                    msg = utils::strTrim(msg, "\r\n\t ");
                    utils::ft_send(chan->get_clients()[i].getFd(), "PRIVMSG " + target + " :" + msg + "\r\n");
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
                    utils::ft_send(clients[i]->getFd(), "PRIVMSG " + target + " :" + msg + "\r\n");
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