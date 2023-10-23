/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODE.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 12:51:45 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/10/23 22:57:14 by ybenlafk         ###   ########.fr       */
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

int check_int(std::string tab, int fd, Client *sender, std::string hostname)
{
    int t;
    
    t = 0;
    while(tab[t])
    {
        if (isdigit(tab[t]))
            t++;
        else 
            return (utils::reply(fd, "461 MODE :Invalid parameter\r\n", sender->getPrifex(hostname)), 0);
    }
    return 1;
}

void    inform(std::string channel_name, Channel *target_channel, Client *sender, std::string hostname)
{
    for (size_t i = 0; i < target_channel->get_clients().size(); i++)
    {
        utils::reply(target_channel->get_clients()[i].getFd(), "JOIN :" + channel_name + "\r\n", target_channel->get_clients()[i].getIp());
        utils::reply(target_channel->get_clients()[i].getFd(), "353 " + sender->getNickName() + " = " + channel_name + " :" + target_channel->get_members() + "\r\n", hostname);
        utils::reply(target_channel->get_clients()[i].getFd(), "366 " + sender->getNickName() + " " + channel_name + " :End of /NAMES list\r\n", hostname);
    }
}

int    setMode(std::string target_mode, std::string target_nick, Channel *target_channel,\
        int fd, Client *sender, std::string hostname, vec_client &clients, \
        std::string channel_name, std::vector<std::string> tab)
{
    long long limit;
    int t = 1;
    std::string modes = "";
    if ((target_mode.length() == 2) && (target_mode[t] == 'i' || target_mode[t] == 'o' || target_mode[t] == 'k' || target_mode[t] == 't' || target_mode[t] == 'l'))
    {
        modes += target_mode[t];
        switch(target_mode[t])
        {
            case 'i':
                target_channel->set_invite_only(true);
                utils::reply(fd, "MODE " + channel_name + ":+i\r\n", sender->getPrifex(hostname));
                break;
            case 'o':
                if (tab.size() != 3)
                {
                    utils::reply(fd, "461 MODE :Not enough parameters\r\n", sender->getPrifex(hostname));
                    break;
                }
                for (size_t i = 0; i < clients.size(); i++ )
                {
                    if (clients[i].getNickName() == target_nick)
                    {
                        if (clients[i].getChannels().find(channel_name) != clients[i].getChannels().end())
                        {
                            if (clients[i].getChannels()[channel_name] == false)
                            {
                                clients[i].getChannels()[channel_name] = true;
                                for (size_t j = 0; j < target_channel->get_clients().size(); j++)
                                {
                                    if (target_channel->get_clients()[j].getNickName() == target_nick)
                                    {
                                        target_channel->get_clients()[j].getChannels()[channel_name] = true;
                                        utils::reply(fd, "MODE " + channel_name + ": +o\r\n", sender->getPrifex(hostname));
                                        inform(channel_name, target_channel, sender, hostname);
                                        break;
                                    }
                                }
                            }
                        }
                        else
                            utils::reply(fd, "482 MODE :the user isn't present in the channel\r\n", sender->getPrifex(hostname));
                    }
                }
                break;
            case 't':
                target_channel->set_topic_changeable(false);
                utils::reply(fd, "MODE " + channel_name + ": +t\r\n", sender->getPrifex(hostname));
                break;
            case 'k':
                if (tab.size() == 3)
                {
                    target_channel->set_pw(true);
                    target_channel->set_key(target_nick);
                    utils::reply(fd, "MODE " + channel_name + ": +k\r\n", sender->getPrifex(hostname));
                }
                else
                    utils::reply(fd, "461 MODE :Not enough parameters\r\n", sender->getPrifex(hostname));
                break;
            case 'l':
                if (tab.size() == 3)
                {
                    if(check_int(target_nick, fd, sender, hostname) == 0)
                        return (0);
                    std::istringstream(target_nick) >> limit;
                    if (limit <= 0)
                        return (utils::reply(fd, "461 MODE :Invalid parameter\r\n", sender->getPrifex(hostname)), 0);
                    target_channel->set_limit(limit);
                    utils::reply(fd, "MODE " + channel_name + ": +l\r\n", sender->getPrifex(hostname));
                }
                else
                    utils::reply(fd, "461 MODE :Not enough parameters\r\n", sender->getPrifex(hostname));
                break;
        }
    }
    else
        utils::reply(fd, "472 i :is unknown mode char to me\r\n", sender->getPrifex(hostname));
    if (modes.length() != 1)
        target_channel->set_mode(target_channel->get_mode() + modes);
    return (1);
}

int unsetMode(std::string target_mode, std::string target_nick, Channel *target_channel,\
        int fd, Client *sender, std::string hostname, vec_client &clients, \
        std::string channel_name, std::vector<std::string> tab)
{
    std::string modes = "";
    int t = 1;
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
                    {
                        utils::reply(fd, "461 MODE :Not enough parameters\r\n", sender->getPrifex(hostname));
                        break;
                    }
                    for (size_t i = 0; i < clients.size(); i++ )
                    {
                        if (clients[i].getNickName() == target_nick)
                        {
                            if (clients[i].getChannels().find(channel_name) != clients[i].getChannels().end())
                            {
                                if (clients[i].getChannels()[channel_name] == true)
                                {
                                    clients[i].getChannels()[channel_name] = false;
                                    for (size_t j = 0; j < target_channel->get_clients().size(); j++)
                                    {
                                        if (target_channel->get_clients()[j].getNickName() == target_nick)
                                        {
                                            target_channel->get_clients()[j].getChannels()[channel_name] = false;
                                            inform(channel_name, target_channel, sender, hostname);
                                            break;
                                        }
                                    }
                                }
                            }
                            else
                                utils::reply(fd, "482 MODE :the user isn't present in the channel\r\n", sender->getPrifex(hostname));
                        }
                    }
                    break;
                case 't':
                    target_channel->set_topic_changeable(true);
                    break;
                case 'k':
                    target_channel->set_pw(false);
                    target_channel->set_key("");
                    break;
                case 'l':
                    target_channel->set_limit(INT_MAX);
                    break;
            }
        }
        else
            utils::reply(fd, "472 : is unknown mode char to me\r\n", sender->getPrifex(hostname));
        t++;
    }
    if (modes.length() != 1)
    {
        for (size_t i = 0; i < modes.length(); i++)
        {
            if (target_channel->get_mode().find(modes[i]) != std::string::npos)
                target_channel->set_mode(target_channel->get_mode().erase(target_channel->get_mode().find(modes[i]), 1));
        }
    }
    return (1);
}

void    Cmds::cmdMode(map_channel &channels, vec_client &clients, int fd, std::string value, std::string hostname, Client *sender)
{
    Channel *target_channel;
    vec_str tab;

    if (!sender) return ;
    tab = split_it(value);
    if (tab.size() < 1)
        return utils::reply(fd, "461 MODE :Not enough parameters\r\n", sender->getPrifex(hostname));
    else if (tab.size() > 3)
        return utils::reply(fd, "461 MODE :Too many parameters\r\n", sender->getPrifex(hostname));
    std::string channel_name = tab[0];
    std::string target_mode = "";
    std::string target_nick = "";
    if (tab.size() >= 2)
        target_mode = tab[1];
    if (tab.size() == 3)
        target_nick = tab[2];
    if (channel_name[0] == '#')
    {
        if (channels.find(channel_name) != channels.end())
        {
            target_channel = &channels[channel_name];
            if (sender->getChannels().find(channel_name) != sender->getChannels().end())
            {
                if (sender->getChannels()[channel_name] == false)
                    return utils::reply(fd, "482 MODE :You're not a channel operator\r\n", sender->getPrifex(hostname));
            }
        }
        else
            return utils::reply(fd, "407 " + channel_name + " :No such channel\r\n", sender->getPrifex(hostname));
    }
    else
        return utils::reply(fd, "407 " + channel_name + " :No such channel\r\n", sender->getPrifex(hostname));
    if (tab.size() == 1)
        return utils::reply(fd, "342 " + channel_name + " Current Modes: " + target_channel->get_mode() + "\r\n", hostname);
    if (target_mode[0] == '+')
    {
        if (setMode(target_mode, target_nick, target_channel, fd, sender, hostname, clients, channel_name, tab) == 0)
            return ;
    }
    else if (target_mode[0] == '-')
    {
        if (unsetMode(target_mode, target_nick, target_channel, fd, sender, hostname, clients, channel_name, tab) == 0)
            return ;
    }
}
