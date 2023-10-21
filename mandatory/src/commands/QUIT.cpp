/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QUIT.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 12:52:25 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/10/21 18:49:15 by ybenlafk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Cmds.hpp"

void Cmds::cmdQuit(vec_client &clients, int fd, std::string msg, map_channel &channels, std::string hostname) {
    // Find and mark the disconnected client for removal
    vec_client::iterator it = clients.begin();
    while (it != clients.end())
    {
        if (it->getFd() == fd)
        {
            std::cout << "\033[1;31m● Client " << it->getNickName() << " disconnected.\033[0m" << std::endl;
            utils::reply(fd, "QUIT :" + msg + "\r\n", it->getPrifex(hostname));
            it->setAuth(false);
            break;
        }
        ++it;
    }
    
    // Remove the marked disconnected clients
    it = clients.begin();
    while (it != clients.end())
    {
        if (!it->getAuth())
            it = clients.erase(it);
        else 
            ++it;
    }

    // Remove disconnected clients from channels
    for (map_channel::iterator tt = channels.begin(); tt != channels.end(); ++tt)
    {
        for (size_t j = 0; j < tt->second.get_clients().size(); ++j)
        {
            if (tt->second.get_clients()[j].getFd() == fd)
            {
                tt->second.get_clients().erase(tt->second.get_clients().begin() + j);
                break;
            }
        }
        if (tt->second.get_clients().empty()) 
            tt->second.setEmpty(true);
    }

    // Remove empty channels from the channels map
    for (map_channel::iterator tt = channels.begin(); tt != channels.end();)
    {
        if (tt->second.isEmpty())
            tt = channels.erase(tt);
        else
            ++tt;
    }
    close(fd);
}
