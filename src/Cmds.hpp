/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cmds.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 16:40:24 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/10/06 13:02:29 by ybenlafk         ###   ########.fr       */
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
        static  void    cmdNick(vec_client clients, int fd, std::string nick);
        static  void    cmdQuit(vec_client &clients, int fd, std::string msg, map_channel &channels);
        static  void    cmdPrivmsg(vec_client clients, int fd, std::string value, map_channel &channels);
        static  void    cmdJoin(map_channel &channels, vec_client clients, int fd, std::string value);
        static  void    cmdPart(map_channel &channels, vec_client clients, int fd, std::string value);
};
void    printChannels(map_channel &channels);
bool    isClientExist(vec_member clients, int fd);
bool    isJoined(Client &client, std::string &name);
#endif