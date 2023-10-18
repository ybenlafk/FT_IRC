/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODE.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbadr <sbadr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 12:51:45 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/10/18 14:39:25 by sbadr            ###   ########.fr       */
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

int check_int(std::string tab, int fd, Client *sender)
{
    int t;
    
    t = 0;
    while(tab[t]){
        if (isdigit(tab[t]))
            t++;
        else
        {
            utils::reply(fd, "472 MODE :is unknown mode char to me\r\n", sender->getPrifex());   
            return 0;
        }
    }
    return 1;
}

void    Cmds::cmdMode(map_channel &channels, vec_client &clients, int fd, std::string value)
{
    int t = 0;
    long long limit;
    Client *sender;
    Channel *target_channel;
    vec_str tab;

    for(size_t i = 0; i < clients.size(); i++)
    {
        if (clients[i]->getFd() == fd)
            sender = clients[i];
    }
    tab = split_it(value);
    if (tab.size() < 2)
        return utils::reply(fd, "461 MODE :Not enough parameters\r\n", sender->getPrifex());
    else if (tab.size() > 3)
        return utils::reply(fd, "461 MODE :Too many parameters\r\n", sender->getPrifex());
    std::string channel_name = tab[0];
    std::string target_mode = tab[1];
    std::string target_nick = "";
    if (tab.size() == 3)
        target_nick = tab[2];
    if (channel_name[0] == '#')
    {
        if (channels.find(channel_name) != channels.end())
        {
            target_channel = channels[channel_name];
            if (sender->getChannels().find(channel_name) != sender->getChannels().end())
            {
                if (sender->getChannels()[channel_name] == false)
                    return utils::reply(fd, "482 MODE :You're not channel operator\r\n", sender->getPrifex());
            }
            else
                return utils::reply(fd, "482 MODE :You're not channel operator\r\n", sender->getPrifex());
        }
        else
            return utils::reply(fd, "407 MODE " + channel_name + " :No such channel\r\n", sender->getPrifex());
    }
    else
        return utils::reply(fd, "407 MODE " + channel_name + " :No such channel\r\n", sender->getPrifex());
    
    if (target_mode[0] == '+')
        {
            if (( target_mode.length() == 2) && (target_mode[1] == 'i' || target_mode[1] == 'o' || target_mode[1] == 'k' || target_mode[1] == 't' || target_mode[1] == 'l'))
            {
                switch(target_mode[1])
                {
                    case 'i':
                        target_channel->set_invite_only(true);
                        break;
                    case 'o':
                        if (tab.size() != 3)
                            return utils::reply(fd, "461 MODE :Not enough parameters\r\n", sender->getPrifex());
                        for (size_t i = 0; i < clients.size(); i++ )
                        {
                            std::cout << " nick "<< target_nick <<"."<< std::endl;
                            if (clients[i]->getNickName() == target_nick)
                            {
                                if (clients[i]->getChannels().find(channel_name) != clients[i]->getChannels().end())
                                {
                                    if (clients[i]->getChannels()[channel_name] == false)
                                    {
                                        clients[i]->getChannels()[channel_name] = true;
                                        for (size_t j = 0; j < target_channel->get_clients().size(); j++)
                                        {
                                            if (target_channel->get_clients()[j].getNickName() == target_nick)
                                            {
                                                target_channel->get_clients()[j].getChannels()[channel_name] = true;
                                                break;
                                            }
                                        }
                                    }
                                }
                                else
                                    return utils::reply(fd, "482 MODE :the user isn't present in the channel\r\n", sender->getPrifex());
                            }
                        }
                        break;
                    case 't':
                        target_channel->set_topic_changeable(true);
                        break;
                    case 'k':
                        if (tab.size() == 4)
                        {
                            target_channel->set_pw(true);
                            target_channel->set_key(target_nick);
                        }
                        target_channel->set_pw(true);
                        break;
                    case 'l':
                        if( check_int(target_nick, fd, sender) == 0)
                            return ;
                        std::istringstream(target_nick) >>limit;
                        target_channel->set_limit( limit);
                        break;
                }
            }
            else
                return utils::reply(fd, "472 MODE :is unknown mode char to me\r\n", sender->getPrifex());
        }
    else if (target_mode[0] == '-')
    {
        t = 0;
        while (target_mode[t])
        {
            if (target_mode[t] == 'i' || target_mode[t] == 'o' || target_mode[t] == 'k' || target_mode[t] == 't' || target_mode[t] == 'l')
            {
                switch(target_mode[t])
                {
                    case 'i':
                        target_channel->set_invite_only(false);
                        break;
                    case 'o':
                        if (tab.size() != 3)
                            return utils::reply(fd, "461 MODE :Not enough parameters\r\n", sender->getPrifex());
                        for (size_t i = 0; i < clients.size(); i++ )
                        {
                            if (clients[i]->getNickName() == target_nick)
                            {
                                if (clients[i]->getChannels().find(channel_name) != clients[i]->getChannels().end())
                                {
                                    if (clients[i]->getChannels()[channel_name] == true)
                                    {
                                        clients[i]->getChannels()[channel_name] = false;
                                        for (size_t j = 0; j < target_channel->get_clients().size(); j++)
                                    {
                                        if (target_channel->get_clients()[j].getNickName() == target_nick)
                                        {
                                            target_channel->get_clients()[j].getChannels()[channel_name] = true;
                                            break;
                                        }
                                    }
                                    }
                                }
                                else
                                    return utils::reply(fd, "482 MODE :the user isn't present in the channel\r\n", sender->getPrifex());
                            }
                            else
                                return utils::reply(fd, "401 MODE :No such nick\r\n", sender->getPrifex());
                        }
                        break;
                    case 't':
                        target_channel->set_topic_changeable(false);
                        break;
                    case 'k':
                        target_channel->set_pw(false);
                        break;
                    case 'l':
                        target_channel->set_limit(INT_MAX);
                        break;
                }
            }
            else
                return utils::reply(fd, "472 MODE :is unknown mode char to me\r\n", sender->getPrifex());
            t++;
        }
    }
}
