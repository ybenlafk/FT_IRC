/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cmds.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 16:40:24 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/10/19 10:41:01 by ybenlafk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMDS
# define CMDS
# include "headers.hpp"
# include "defines.hpp"
# include "utils.hpp"

class Cmds
{
    public:
        static  void    cmdMode(map_channel &channels, vec_client &clients, int fd, std::string value, std::string hostname);
        static  void    cmdNick(vec_client &clients, int fd, std::string nick, std::string hostname);
        static  void    cmdQuit(vec_client &clients, int fd, std::string msg, map_channel &channels, std::string hostname);
        static  void    cmdPrivmsg(vec_client clients, int fd, std::string value, map_channel &channels, std::string hostname);
        static  void    cmdJoin(map_channel &channels, vec_client &clients, int fd, std::string value, std::string hostname);
        static  void    cmdPart(map_channel &channels, vec_client &clients, int fd, std::string value, std::string hostname);
        static  void    cmdKick(map_channel &channels, vec_client &clients, int fd, std::string value, std::string hostname);
        static  void    cmdTopic(map_channel &channels, vec_client &clients, int fd, std::string value, std::string hostname);
        static  void    cmdInvite(map_channel &channels, vec_client &clients, int fd, std::string value, std::string hostname);
        
};

void    removeFromChannel(map_channel &channels, int fd, std::string name, vec_client &clients);
void    printChannels(map_channel &channels);
bool    isClientExist(vec_member clients, int fd);
bool    isJoined(Client &client, std::string &name);
Channel* check_channel(map_channel &channels, std::string channel, int fd, Client *sender, std::string cmd, std::string hostname);
Client* check_client_fd(vec_client &clients, int fd);
Client* check_client_s(vec_client &clients, std::string user, int fd, Client *sender, std::string cmd, std::string hostname);
int check_opratotPrivilege(Client *sender, Channel *channel);

#endif