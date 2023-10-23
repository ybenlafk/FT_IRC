/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PRIVMSG.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 12:52:21 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/10/23 23:26:59 by ybenlafk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Cmds.hpp"

bool    isJoined(Client &client, std::string &name)
{
    if (client.getChannels().find(name) != client.getChannels().end())
        return (true);
    return (false);
}

void    parsPrvMsg(std::string &value, std::string &target, std::string &msg)
{
    int i = 0;
    while (value[i] && value[i] != ':' && value[i] != ' ' && value[i] != '\t' && value[i] != '\r' && value[i] != '\n')
        target += value[i++];
    while (value[i] == ':') i++;
    while (value[i])
        msg += value[i++];
    msg = utils::strTrim(msg, "\r\n\t ");
    target = utils::strTrim(target, "\r\n\t ");
}

void    Cmds::cmdPrivmsg(vec_client &clients, int fd, std::string value, map_channel &channels, std::string hostname, Client *sender)
{
    std::string target = "", msg = "";
    if (!sender) return ;
    std::string prefix = sender->getPrifex(hostname);
    parsPrvMsg(value, target, msg);
    if (target.empty() || msg.empty())
    {
        utils::reply(fd, "412 * :No message to send\r\n", prefix);
        return ;
    }
    if (target[0] == '#')
    {
        if (channels.find(target) != channels.end())
        {
            Channel *chan = &channels[target];

            int l = 1;
            for (size_t i = 0 ; i < chan->get_clients().size();i++)
            {
                if (chan->get_clients()[i].getFd() == fd)
                    l = 0;
            }
            if (l)
            {
                utils::reply(fd, "404 * " + target + " :Cannot send to channel\r\n", prefix);
                return ;
            }
            for (size_t i = 0; i < chan->get_clients().size(); i++)
            {
                if (chan->get_clients()[i].getFd() != fd && isJoined(chan->get_clients()[i], target))
                {
                    msg = utils::strTrim(msg, "\r\n\t: ");
                    utils::reply(chan->get_clients()[i].getFd(), "PRIVMSG " + target + " :" + msg + "\r\n", prefix);
                }
            }
        }
        else
            utils::reply(fd, "401 * " + target + " :No such nick/channel\r\n", prefix);
    }
    else
    {
        for (size_t i = 0; i < clients.size(); i++)
        {
            if (clients[i].getNickName() == target)
            {
                if (clients[i].getAuth())
                {
                    msg = utils::strTrim(msg, "\r\n\t: ");
                    utils::reply(clients[i].getFd(), "PRIVMSG " + target + " :" + msg + "\r\n", prefix);
                    return ;
                }
                else
                {
                    utils::reply(fd, "401 * " + target + " :No such nick/channel\r\n", prefix);
                    return ;   
                }
                return ;
            }
        }
        utils::reply(fd, "401 * " + target + " :No such nick/channel\r\n", prefix);
    }
}