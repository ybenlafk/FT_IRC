/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 13:46:24 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/10/03 12:11:29 by ybenlafk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LO
# define LO

# include "headers.hpp"
# include "defines.hpp"
# include "Channel.hpp"

class utils
{
    public:
        static std::string  getCmd(std::string str, char c);
        static std::string  getValue(std::string str, char c);
        static std::string  strTrim(std::string str, std::string chars);
        static void         spaceSkipe(std::string &str);
        static int          setUpServer(vec_client *clients, int port);
        static void         split(std::string str, char c, vec_str *names, vec_str *keys);
};
#endif