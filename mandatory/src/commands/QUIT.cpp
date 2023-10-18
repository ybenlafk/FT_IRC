/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QUIT.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 12:52:25 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/10/18 14:49:40 by ybenlafk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Cmds.hpp"

void   Cmds::cmdQuit(vec_client &clients, int fd, std::string msg, map_channel &channels)
{
    for (size_t i = 0; i < clients.size(); i++)
    {
        if (clients[i]->getFd() == fd)
        {
            std::cout << "\033[1;31m● Client " << clients[i]->getNickName() << " disconnected.\033[0m" << std::endl;
            utils::ft_send(fd, "QUIT : " + msg + "\r\n");
            for (map_channel::iterator it = channels.begin(); it != channels.end(); it++)
            {
                for (size_t j = 0; j < it->second->get_clients().size(); j++)
                {
                    if (it->second->get_clients()[j].getFd() == fd)
                    {
                        it->second->get_clients().erase(it->second->get_clients().begin() + j);
                        break ;
                    }
                }
                if (it->second->get_clients().size() == 0)
                {
                    delete it->second;
                    channels.erase(it);
                    if (channels.size() == 0)
                        break;
                }
            }
            close(fd);
            clients.erase(clients.begin() + i);
            return ;
        }
    }
    std::cout << "\033[1;31m● Client disconnected.\033[0m" << std::endl;
}