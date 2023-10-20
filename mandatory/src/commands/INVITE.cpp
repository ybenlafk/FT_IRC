/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   INVITE.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 12:52:10 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/10/20 14:43:30 by ybenlafk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Cmds.hpp"

Channel* check_channel(map_channel &channels, std::string channel, int fd, Client *sender, std::string cmd, std::string hostname)
{
    if (channels.find(channel) != channels.end())
    {
        Channel *res = &channels[channel];
        return (res);
    }
    utils::reply(fd, "403 "+cmd+ " :No such channel\r\n", sender->getPrifex(hostname));
    return (NULL);
}

Client* check_client_fd(vec_client &clients, int fd)
{
    for (size_t i = 0; i < clients.size(); i++)
    {
        if (clients[i].getFd() == fd)
            return &clients[i];
    }
    return (NULL);
}

int check_opratotPrivilege(Client *sender, Channel *channel)
{
    if (sender->getChannels().find(channel->get_name()) != sender->getChannels().end())
    {
        if (sender->getChannels()[channel->get_name()] == true)
            return 1;
    }
    return 0;
}

Client* check_client_s(vec_client &clients, std::string user, int fd, Client *sender, std::string cmd, std::string hostname)
{
    for (size_t i = 0; i < clients.size(); i++)
    {
        if (clients[i].getNickName() == user)
            return &clients[i];
    }
    utils::reply(fd, "401 " + cmd + " :No such nick/channel\r\n", sender->getPrifex(hostname));
    return NULL;
}

void    sendToClients(std::string msg, vec_client clients, int fd, std::string hostname)
{
    for (size_t i = 0; i < clients.size(); i++)
        if (clients[i].getFd() != fd)
            utils::reply(clients[i].getFd(), msg, clients[i].getPrifex(hostname));
}

void    Cmds::cmdInvite(map_channel &channels, vec_client &clients, int fd, std::string value, std::string hostname)
{
    std::vector<std::string> tab = split_it(value);
    Client* sender = check_client_fd(clients, fd);
    if (tab.size() < 2)
        return utils::reply (fd, "461 INVITE :Not enough parameters\r\n",sender->getPrifex(hostname));
    Channel *channel = check_channel(channels, tab[1], fd, sender, "INVITE", hostname);
    Client* user = check_client_s(clients, tab[0], fd, sender, "INVITE", hostname);
    // check if the channel exist ,
    if (!channel || !user)
        return ;
    if (check_opratotPrivilege(sender, channel) == 0)
        return utils::reply(fd, "482 INVITE :You're not channel operator\r\n", sender->getPrifex(hostname));
    //  check if the user is in the channel and the invited client isn't
    if (channel->is_member(sender))
    {
        if (channel->is_member(user))
            return utils::reply(fd, "443 INVITE :is already on channel\r\n", sender->getPrifex(hostname));
        else
        {
            utils::reply(user->getFd(), "INVITE "+user->getNickName()+" "+channel->get_name()+"\r\n", sender->getPrifex(hostname));
            user->setInvited(true);
        }
    }
}