/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 12:51:12 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/10/24 12:07:21 by ybenlafk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Cmds.hpp"

void    Cmds::cmdNick(vec_client &clients, std::string nick, std::string hostname, map_channel &channels, Client *sender)
{
    if (nick[0] == ':')
        nick.erase(0, 1);
    for (size_t i = 0; i < clients.size(); i++)
    {
        if (clients[i].getFd() != sender->getFd())
        {
            if (clients[i].getNickName() == nick)
            {
                utils::reply(sender->getFd(), "433 * " + nick + " :Nickname is already in use\r\n", clients[i].getPrifex(hostname));
                return ;
            }
        }
    }
    if (nick.empty())
    {
        utils::reply(sender->getFd(), "431 * :No nickname given\r\n", sender->getPrifex(hostname));
        return ;
    }
    // inform all the client that the nickname has changed
    for (size_t j = 0; j < clients.size(); j++)
    {
        if (clients[j].getFd() != sender->getFd() && clients[j].getAuth() == true)
            utils::reply(clients[j].getFd()," NICK " + nick + "\r\n", sender->getPrifex(hostname));
    }
    sender->setNickName(nick);
    for (map_channel::iterator it = channels.begin(); it != channels.end(); it++)
    {
        vec_member &members = it->second.get_clients();
        for (size_t j = 0; j < members.size(); j++)
        {
            if (members[j].getFd() == sender->getFd())
            {
                members[j].setNickName(nick);
                break ;
            }
        }
    }
    return ;
}