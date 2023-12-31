/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TOPIC.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 12:52:30 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/10/23 23:08:16 by ybenlafk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Cmds.hpp"

vec_str split_it_again(std::string tab)
{
    int i = 0;
    std::vector<std::string> res;
    std::string word = "";
    while(tab[i] && !std::isspace(tab[i]))
        word += tab[i++];
    res.push_back(word);
    word.clear();
    if (tab[i] == ':') i++;
    while (tab[i]) word += tab[i++];
    if (!word.empty())
        res.push_back( ":" + word);
    return(res);
}

void    Cmds::cmdTopic(map_channel &channels, std::string value, std::string hostname, Client *sender)
{
    std::vector<std::string> tab = split_it_again(value);
    if(!sender)
        return ;
    // check if the channel exist
    Channel *channel = check_channel(channels, tab[0], sender->getFd(), sender, "TOPIC", hostname);
    if (!channel)
        return ;
    if (check_opratotPrivilege(sender, channel) == 0 && channel->get_topic_changeable() == true)
        return utils::reply(sender->getFd(), "482 MODE :You're not channel operator\r\n", hostname);
    else if (isJoined(*sender, tab[0]) == false)
        return utils::reply(sender->getFd(), "442 :You're not on that channel\r\n", hostname);
    if (tab.size() == 1)
    {
        if (channel->get_topic().empty())
            return utils::reply(sender->getFd(), "331 :No topic is set\r\n", hostname);
        return utils::reply(sender->getFd(), "332 " + sender->getNickName() + " " + channel->get_name() + " :"+channel->get_topic()+"\r\n", hostname);
    }
    else
    {
        if(tab[1][0] == ':' && tab[1].length() > 1)
        {
            channel->set_topic(utils::strTrim(tab[1], "\t\n\r :"));
            return utils::reply(sender->getFd(), "TOPIC "+channel->get_name()+" :"+channel->get_topic()+"\r\n", sender->getPrifex(hostname));
        }
        else if (tab[1][0] == ':')
        {
            channel->set_topic("");
            return utils::reply(sender->getFd(), "TOPIC "+channel->get_name()+" :"+channel->get_topic()+"\r\n", sender->getPrifex(hostname));
        }
    }
}
