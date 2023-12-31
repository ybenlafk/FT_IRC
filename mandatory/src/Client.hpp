/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 09:26:59 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/10/23 23:46:08 by ybenlafk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "headers.hpp"

typedef std::map<std::string, bool> m_channel;
typedef std::map<std::string, bool> map_invited;

class Client
{
    private:
        int             fd;
        std::string     nick_name;
        std::string     user_name;
        std::string     real_name;
        std::string     ip_addr;
        m_channel     channels;
        bool            pw;
        bool            auth;
        map_invited     invited;
    public:
        Client();
        Client(Client const &src);
        ~Client();
        Client(int fd, std::string nick_name, std::string user_name, std::string real_name, bool auth);

        void            add_channel(std::string &name, bool admin);
        m_channel       &getChannels();

        void            setFd(int fd);
        void            setPw(bool pw);
        void            setAuth(bool auth);
        void            setNickName(std::string nick_name);
        void            setUserName(std::string user_name);
        void            setRealName(std::string real_name);
        void            setIpAddr(std::string ip_addr);
        void            setInvited(std::string name, bool invited);
        
        bool            getAuth() const;
        int             getFd() const;
        bool            getPw() const;
        std::string     getNickName() const;
        std::string     getUserName() const;
        std::string     getRealName() const;
        std::string     getIpAddr() const;
        bool            getInvited(std::string name) const;

        std::string      getPrifex(std::string hostname);
        std::string      getIp();
};