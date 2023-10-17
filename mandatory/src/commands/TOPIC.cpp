/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TOPIC.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbadr <sbadr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 12:52:30 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/10/17 15:26:07 by sbadr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Cmds.hpp"

void    Cmds::cmdTopic(map_channel &channels, vec_client &clients, int fd, std::string value)
{
    vec_str tab;
    Channel *target_channel;
    Client *sender;
    std::string channel;
    std::string topic;
    
    tab = split_it(value);
    channel = tab[0];
    topic = tab[1];
    for(size_t i = 0; i < clients.size(); i++)
    {
        if (clients[i]->getFd() == fd)
            sender = clients[i];
    }
////////////////////////////// check size of tab//
    if (tab.size() == 1)
        return utils::reply(fd, "461 TOPIC :Not enough parameters\r\n", "");
    else if (tab.size() > 2)
        return utils::reply(fd, "461 TOPIC :Too many parameters\r\n", "");
////////////////////////////// check channel and user oprator privilige//
    if (channels.find(channel) != channels.end())
    {
        target_channel = channels[channel];
        if (sender->getChannels().find(channel) != sender->getChannels().end())
        {
            if (sender->getChannels()[channel] == false && target_channel->get_topic_changeable() == false)
                return utils::reply(fd, "482 MODE :You're not channel operator\r\n", sender->getPrifex());
        }
        else
            return utils::reply(fd, "482 MODE :You're not in the channel\r\n", sender->getPrifex());//check//
    }
    else
        return utils::reply(fd, "407 MODE " + channel + " :No such channel\r\n", sender->getPrifex());
//////////////////////////////   
        if (channels.find(tab[0]) == channels.end())
        {
            utils::reply(fd, "403 TOPIC :No such channel\r\n", "");
            return ;
        }
        else
        {
            Channel *chan = channels[tab[0]];
            if (chan->getTopic().empty())
                utils::reply(fd, "331 TOPIC :No topic is set\r\n", "");
            else
                utils::reply(fd, "332 TOPIC " + chan->getName() + " :" + chan->getTopic() + "\r\n", "");
        }
    }
    
}