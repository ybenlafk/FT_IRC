/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 09:26:59 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/09/27 16:43:28 by ybenlafk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OO
# define OO

#include "headers.hpp"

class Client
{
    private:
        int             fd;
        std::string     nick_name;
        std::string     user_name;
        std::string     real_name;
        bool            pw;
        bool            auth;
    public:
        Client();
        ~Client();
        Client(int fd, std::string nick_name, std::string user_name, std::string real_name, bool auth);
        void            setFd(int fd);
        void            setPw(bool pw);
        void            setAuth(bool auth);
        void            setNickName(std::string nick_name);
        void            setUserName(std::string user_name);
        void            setRealName(std::string real_name);
        bool            getAuth() const;
        int             getFd() const;
        bool            getPw() const;
        std::string     getNickName() const;
        std::string     getUserName() const;
        std::string     getRealName() const;
};

#endif