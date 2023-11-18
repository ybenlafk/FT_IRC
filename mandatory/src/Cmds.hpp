/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cmds.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 16:40:24 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/10/23 23:39:12 by ybenlafk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "headers.hpp"
# include "defines.hpp"
# include "utils.hpp"

class Cmds
{
    public:
        static  void    cmdMode(map_channel &channels, vec_client &clients, int fd, std::string value, std::string hostname, Client *sender);
        static  void    cmdNick(vec_client &clients, std::string nick, std::string hostname, map_channel &channels, Client *sender);
        static  void    cmdQuit(vec_client &clients, int fd, std::string msg, map_channel &channels, std::string hostname);
        static  void    cmdPrivmsg(vec_client &clients, int fd, std::string value, map_channel &channels, std::string hostname, Client *sender);
        static  void    cmdJoin(map_channel &channels, std::string value, std::string hostname, Client *sender);
        static  void    cmdPart(map_channel &channels, vec_client &clients, std::string value, std::string hostname, Client *sender);
        static  void    cmdKick(map_channel &channels, vec_client &clients, std::string value, std::string hostname, Client *sender);
        static  void    cmdTopic(map_channel &channels, std::string value, std::string hostname, Client *sender);
        static  void    cmdInvite(map_channel &channels, vec_client &clients, int fd, std::string value, std::string hostname, Client *sender);
        
};

void    removeFromChannel(map_channel &channels, int fd, std::string name, vec_client &clients);
void    printChannels(map_channel &channels);
bool    isClientExist(vec_member clients, int fd);
bool    isJoined(Client &client, std::string &name);
Channel* check_channel(map_channel &channels, std::string channel, int fd, Client *sender, std::string cmd, std::string hostname);
Client* check_client_s(vec_client &clients, std::string user, int fd, Client *sender, std::string cmd, std::string hostname);
int check_opratotPrivilege(Client *sender, Channel *channel);
bool    isJoined(Client &client, std::string &name);