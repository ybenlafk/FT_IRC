/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cmds.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbadr <sbadr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 16:40:24 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/10/18 14:44:06 by sbadr            ###   ########.fr       */
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
        static  void    cmdMode(map_channel &channels, vec_client &clients, int fd, std::string value);
        static  void    cmdNick(vec_client &clients, int fd, std::string nick);
        static  void    cmdQuit(vec_client &clients, int fd, std::string msg, map_channel &channels);
        static  void    cmdPrivmsg(vec_client clients, int fd, std::string value, map_channel &channels);
        static  void    cmdJoin(map_channel &channels, vec_client &clients, int fd, std::string value);
        static  void    cmdPart(map_channel &channels, vec_client &clients, int fd, std::string value);
        static  void    cmdKick(map_channel &channels, vec_client &clients, int fd, std::string value);
        static  void    cmdTopic(map_channel &channels, vec_client &clients, int fd, std::string value);
        static  void    cmdInvite(map_channel &channels, vec_client &clients, int fd, std::string value);
        
};

void    removeFromChannel(map_channel &channels, int fd, std::string name, vec_client &clients);
void    printChannels(map_channel &channels);
bool    isClientExist(vec_member clients, int fd);
bool    isJoined(Client &client, std::string &name);
Channel* check_channel(map_channel &channels, std::string channel, int fd, Client *sender, std::string cmd);
Client* check_client_fd(vec_client &clients, int fd, std::string cmd);
Client* check_client_s(vec_client &clients, std::string user, int fd, Client *sender, std::string cmd);
int check_opratotPrivilege(Client *sender, Channel *channel);

#endif