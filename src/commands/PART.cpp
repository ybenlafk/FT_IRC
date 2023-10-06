/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PART.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 12:52:15 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/10/06 12:55:28 by ybenlafk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Cmds.hpp"

void    removeFromChannel(map_channel &channels, int fd, std::string name)
{
    for (map_channel::iterator it = channels.begin(); it != channels.end(); it++)
        if (it->first == name)
            for (size_t j = 0; j < it->second->get_clients().size(); j++)
                if (it->second->get_clients()[j].getFd() == fd)
                    it->second->get_clients().erase(it->second->get_clients().begin() + j);
    for (map_channel::iterator it = channels.begin(); it != channels.end(); it++)
    {
       if (it->second->get_clients().size() == 0)
       {
           delete it->second;
           channels.erase(it);
           break;
       }
    }
    // if (channels.find(name) != channels.end())
    // {
        
    // }
}

void    Cmds::cmdPart(map_channel &channels, vec_client clients, int fd, std::string value)
{
    for (size_t i = 0; i < clients.size(); i++)
    {
        if (clients[i]->getFd() == fd)
        {
            if (clients[i]->getAuth())
            {
                std::string reason = "";
                vec_str names;
                if (utils::split(value, ',', &names, &reason) == 0)
                {
                    utils::ft_send(fd, "461 * PART :Not enough parameters\r\n");
                    return ;
                }
                for (size_t j = 0; j < names.size(); j++)
                {
                    if (channels.find(names[j]) != channels.end())
                    {
                        if (isJoined(*clients[i], names[j]))
                        {
                            if (reason.empty())
                                utils::reply(fd, "PART " + names[j] + "\r\n", clients[i]->getPrifex());
                            else
                                utils::reply(fd, "PART " + names[j] + " :" + reason + "\r\n", clients[i]->getPrifex());
                            clients[i]->getChannels().erase(names[j]);
                            removeFromChannel(channels, fd, names[j]);
                            printChannels(channels);
                        }
                        else
                            utils::ft_send(fd, "442 * " + names[j] + " :You're not on that channel\r\n");
                    }
                    else
                        utils::ft_send(fd, "403 * " + names[j] + " :No such channel\r\n");
                }
            }
        }
    }
}