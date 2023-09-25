/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 20:44:08 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/09/25 16:47:32 by ybenlafk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef YY
# define YY

# include "headers.hpp"

class Server
{
    public:
        int         port;
        std::string password;
        vec_int     clients;
        vec_pollfd  pollfds;
        Server(int port, std::string password) : port(port), password(password) {}
        ~Server() {}
        void        run();
        void        handleClients(int ServerSocket);
        bool        isValidPw(std::string pw, int i);
};

#endif