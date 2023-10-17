/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODE.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 12:51:45 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/10/17 10:17:34 by ybenlafk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Cmds.hpp"
#include <ostream>
#include <sstream>

std::vector<std::string> split_it(std::string str)
{
    int i = 0;
    std::vector<std::string> res;
    std::string word;
    while (str[i])
    {
        word = "";
        while (str[i] && str[i] != ' ' && str[i] != '\t') word += str[i++];
        while (str[i] && (str[i] == ' ' || str[i] == '\t')) i++;
        if (!word.empty())
            res.push_back(word);
    }
    return(res);
}

void    Cmds::cmdMode(map_channel &channels, vec_client &clients, int fd, std::string value)
{
    int t = 0;
    long long limit;
    // int j = i + 1;
    std::string target;
    Client *sender;
    Channel *target_channel;
    vec_str tab;

    for(size_t i = 0; i < clients.size(); i++)
    {
        if (clients[i]->getFd() == fd)
            sender = clients[i];
    }
    tab = split_it(value);
    for (size_t j = 0; j < tab.size(); j++)
    {
        if (tab[j][0] == '#')
        {
            target = tab[j];
            if (channels.find(target) != channels.end())
            {
                j++;
                target_channel = channels[target];
                if (sender->getChannels().find(target) != sender->getChannels().end())
                {
                    if (sender->getChannels()[target] == false)
                        return utils::reply(fd, "482 MODE :You're not channel operator\r\n", sender->getPrifex());
                }
                else
                    return utils::reply(fd, "482 MODE :You're not channel operator\r\n", sender->getPrifex());
            }
            else
                return utils::reply(fd, "403 MODE " + target + " :No such channel\r\n", sender->getPrifex());
        }
        else
            return utils::reply(fd, "403 MODE " + target + " :No such channel\r\n", sender->getPrifex());
        
        if (tab[j][0] == '+')
        {
            if (( tab[j].length() == 2) && (tab[j][1] == 'i' || tab[j][1] == 'o' || tab[j][1] == 'k' || tab[j][1] == 't' || tab[j][1] == 'l'))
            {
                switch(tab[j][1])
                {
                    case 'i':
                        target_channel->set_invite_only(true);
                        break;
                    case 'o':
                        for (size_t i = 0; i < clients.size(); i++ )
                        {
                            if (clients[i]->getFd() == fd)
                                sender = clients[i];
                        }
                        break;
                    case 't':
                        target_channel->set_topic_changeable(true);
                        break;
                    case 'k':
                        target_channel->set_pw(true);
                        break;
                    case 'l':
                        t = 0;
                        while(tab[j+1][t]){
                            if (isdigit(tab[j+1][t]))
                                t++;
                            else
                                return utils::reply(fd, "472 MODE :is unknown mode char to me\r\n", sender->getPrifex());
                        }
                        std::istringstream(tab[j + 1]) >>limit;
                        target_channel->set_limit( limit);
                        break;
                }
            }
            else
                return utils::reply(fd, "472 MODE :is unknown mode char to me\r\n", sender->getPrifex());
        }
         
        else if (tab[j][0] == '-')
        {
            t = 0;
            while (tab[j][t])
            {
                if (tab[j][t] == 'i' || tab[j][t] == 'o' || tab[j][t] == 'k' || tab[j][t] == 't' || tab[j][t] == 'l')
                {
                    switch(tab[j][t])
                    {
                        case 'i':
                            target_channel->set_invite_only(true);
                            break;
                        case 'o':
                            target_channel->set_invite_only(true);
                            break;
                        case 't':
                            target_channel->set_topic_changeable(true);
                            break;
                        case 'k':
                            target_channel->set_pw(true);
                            break;
                        case 'l':
                            std::istringstream(tab[j + 1]) >>limit;
                            target_channel->set_limit( limit);
                            break;
                    }
                }
                else
                    return utils::reply(fd, "472 MODE :is unknown mode char to me\r\n", sender->getPrifex());
                    t++;
            }
        }
    }
}



