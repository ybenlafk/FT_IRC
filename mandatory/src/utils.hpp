/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 13:46:24 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/10/20 16:00:10 by ybenlafk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LO
# define LO

# include "headers.hpp"
# include "defines.hpp"
# include "Channel.hpp"
typedef std::vector<Client> vec_member;

class utils
{
    public:
        static Client       *getClientByFd(int fd, vec_client clients);
        static Client       *getClientByNick(std::string nick, vec_client clients);
        static std::string  getCmd(std::string str, char c);
        static std::string  getValue(std::string str, char c);
        static std::string  strTrim(std::string str, std::string chars);
        static void         spaceSkipe(std::string &str);
        static int          setUpServer(vec_client *clients, int port);
        static void         split(std::string str, char c, vec_str *names, vec_str *keys);
        static int         split(std::string str, char c, vec_str *names, std::string *reason);
        static void         ft_send(int fd, std::string msg);
        static bool         isValidName(std::string name);
        static std::string  getHostName();
        static void         reply(int fd, std::string msg, std::string prefix);
        static std::string  get_ip(sockaddr_in user_addr);
};

std::vector<std::string> split_it(std::string str);
int check_int(std::string tab, int fd, Client *sender);

#endif