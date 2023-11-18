/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 13:46:24 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/10/23 23:27:17 by ybenlafk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "headers.hpp"
# include "defines.hpp"
# include "Channel.hpp"
# include "Cmds.hpp"
# include "Server.hpp"

typedef std::vector<Client> vec_member;
class Server;

class utils
{
    public:
        static std::string  getCmd(std::string str, char c);
        static std::string  getValue(std::string str, char c);
        static std::string  strTrim(std::string str, std::string chars);
        static std::string  getHostName();
        static std::string  get_ip(sockaddr_in user_addr);
        static int          split(std::string str, char c, vec_str *names, std::string *reason);
        static int          bufferChecker(std::string buffer, std::string &poper);
        static bool         isValidName(std::string name);
        static int          setUpServer(vec_client *clients, int port);
        static void         spaceSkipe(std::string &str);
        static void         split(std::string str, char c, vec_str *names, vec_str *keys);
        static void         ft_send(int fd, std::string msg);
        static void         reply(int fd, std::string msg, std::string prefix);
        static void         AcceptConnection(int ServerSocket, Server &server);
        static void         HandleReq(Server &server, int i, std::string hostname);
        static void         ExecCmds(Server &server, int i, vec_client::iterator &it, std::string hostname);
};

std::vector<std::string> split_it(std::string str);
int check_int(std::string tab, int fd, Client *sender);
