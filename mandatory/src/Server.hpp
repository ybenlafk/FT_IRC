/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 20:44:08 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/10/19 16:35:17 by ybenlafk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#pragma once

# include "headers.hpp"
#include "defines.hpp"
# include "utils.hpp"
# include "Cmds.hpp"
# include "Channel.hpp"
class utils;
class Server
{
    public:
        std::string hostname;
        int         port;
        std::string password;
        vec_client  clients;
        map_channel channels;
        vec_pollfd  pollfds;
        map_int_str popers;
        map_int_addr addrs;
        vec_str     buffers;
        Server(int port, std::string password) : port(port), password(password){}
        ~Server() {} 
        void        run();
        void        handleClients(int ServerSocket);
        int         AddClient(std::string pw, Client *client, int fd);
};
