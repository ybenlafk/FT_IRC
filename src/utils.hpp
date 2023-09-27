/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 13:46:24 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/09/27 16:08:48 by ybenlafk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LO
# define LO

# include "headers.hpp"
# include "defines.hpp"

class utils
{
    public:
        static std::string getCmd(std::string str, char c);
        static std::string   strTrim(std::string str, std::string chars);
        static void         spaceSkipe(std::string &str);
};
#endif