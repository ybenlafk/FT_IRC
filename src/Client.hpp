/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbadr <sbadr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 09:26:59 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/10/15 11:46:57 by sbadr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OO
# define OO

#include "headers.hpp"

typedef std::map<std::string, bool> m_channel;

class Client
{
    private:
        int             fd;
        std::string     nick_name;
        std::string     user_name;
        std::string     real_name;
        m_channel       channels;
        bool            pw;
        bool            auth;
        bool            admin;
    public:
        Client();
        Client(Client const &src);
        ~Client();
        Client &operator=(const Client &s);
        Client(int fd, std::string nick_name, std::string user_name, std::string real_name, bool auth, bool admin);

        void            add_channel(std::string &name, bool admin);
        m_channel       &getChannels();

        void            setFd(int fd);
        void            setPw(bool pw);
        void            setAuth(bool auth);
        void            setNickName(std::string nick_name);
        void            setUserName(std::string user_name);
        void            setRealName(std::string real_name);
        void            setAdmin(bool admin);
        bool            getAuth() const;
        int             getFd() const;
        bool            getPw() const;
        std::string     getNickName() const;
        std::string     getUserName() const;
        std::string     getRealName() const;
        bool            getAdmin() const;

        std::string      getPrifex();
};

#endif