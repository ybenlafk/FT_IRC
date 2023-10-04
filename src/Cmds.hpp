/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cmds.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 16:40:24 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/10/04 16:50:09 by ybenlafk         ###   ########.fr       */
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
        static  void    cmdQuit(vec_client clients, int fd, std::string msg);
        static  void    cmdPrivmsg(vec_client clients, int fd, std::string value, map_channel &channels);
        static  void    cmdJoin(map_channel &channels, vec_client clients, int fd, std::string value);
        static  void    cmdPart(map_channel &channels, vec_client clients, int fd, std::string value);
};

#endif