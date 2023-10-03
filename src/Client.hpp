/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 09:26:59 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/10/03 13:26:24 by ybenlafk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OO
# define OO

#include "headers.hpp"
#include "Channel.hpp"

class Channel;

typedef std::vector<Channel*> vec_channel;

class Client
{
    private:
        int             fd;
        std::string     nick_name;
        std::string     user_name;
        std::string     real_name;
        vec_channel     channels;
        bool            pw;
        bool            auth;
        bool            admin;
    public:
        Client();
        Client(Client const &src);
        ~Client();
        Client(int fd, std::string nick_name, std::string user_name, std::string real_name, bool auth, bool admin);

        void            add_channel(Channel *channel);

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
};

#endif