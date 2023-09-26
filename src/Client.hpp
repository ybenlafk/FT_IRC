/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 09:26:59 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/09/26 09:40:02 by ybenlafk         ###   ########.fr       */
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
    public:
        Client();
        ~Client();
        Client(int fd, std::string nick_name, std::string user_name, std::string real_name);
        int             getFd() const;
        std::string     getNickName() const;
        std::string     getUserName() const;
        std::string     getRealName() const;
};

#endif