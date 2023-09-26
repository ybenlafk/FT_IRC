/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 20:44:08 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/09/26 10:14:36 by ybenlafk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef YY
# define YY

# include "headers.hpp"
#include "defines.hpp"

class Server
{
    public:
        int         port;
        std::string password;
        vec_client  clients;
        vec_pollfd  pollfds;
        Server(int port, std::string password) : port(port), password(password) {}
        ~Server() {}
        void        run();
        void        handleClients(int ServerSocket);
        bool        AddClient(std::string pw, int i);
};

#endif