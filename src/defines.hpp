/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 09:47:05 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/10/03 21:54:42 by ybenlafk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LL
# define LL

#include "headers.hpp"
#include "Client.hpp"
#include "Channel.hpp"

typedef struct pollfd pollfd;
typedef std::vector<pollfd> vec_pollfd;
typedef std::vector<std::string> vec_str;
typedef std::map<std::string, Channel*> map_channel;
typedef std::vector<Channel*> vec_channel;
typedef std::vector<Client*> vec_client;


bool    isExist(vec_client clients, int fd);


#endif