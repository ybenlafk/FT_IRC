/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TOPIC.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbadr <sbadr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 12:52:30 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/10/18 19:25:56 by sbadr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Cmds.hpp"

vec_str split_it_again(std::string tab)
{
    int i = 0;
    std::vector<std::string> res;
    std::string word = "";
    while(tab[i] && !isspace(tab[i]))
        word += tab[i++];
    res.push_back(word);
    word.clear();
    if (std::strchr(tab.c_str(), ':'))
    {
        word = std::strchr(tab.c_str(), ':');
        res.push_back(word);
    }
    return(res);
}

void    Cmds::cmdTopic(map_channel &channels, vec_client &clients, int fd, std::string value)
{
    std::vector<std::string> tab = split_it_again(value);
    for (size_t i = 0; i < tab.size(); i++)
    {
        std::cout << tab[i] << "."<< std::endl;
    }
    Client* sender = check_client_fd(clients, fd, "TOPIC");
    if(!sender)
        return ;
    Channel *channel = check_channel(channels, tab[0], fd, sender, "TOPIC");
    // check if the channel exist ,
    if (!channel)
        return ;
    if (check_opratotPrivilege(sender, channel) == 0 && channel->get_topic_changeable() == false)
        return utils::reply(fd, "482 TOPIC :You're not channel operator\r\n", sender->getPrifex());
    if (tab.size() == 1)
    {
        if (channel->get_topic().empty())
            return utils::reply(fd, "331 TOPIC :No topic is set\r\n", sender->getPrifex());
        return utils::reply(fd, "332 "+channel->get_name()+" :"+channel->get_topic()+"\r\n", sender->getPrifex());
    }
    else
    {
        if(tab[1][0] == ':' && tab[1].length() > 1)
        {
            channel->set_topic(tab[1].substr(1));
            return utils::reply(fd, "332 "+channel->get_name()+" :"+channel->get_topic()+"\r\n", sender->getPrifex());
        }
        else if (tab[1][0] == ':')
        {
            channel->set_topic("");
            return utils::reply(fd, "332 "+channel->get_name()+" :"+channel->get_topic()+"\r\n", sender->getPrifex());
        }
    }
}
