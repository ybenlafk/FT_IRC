/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 09:47:05 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/10/14 20:18:36 by ybenlafk         ###   ########.fr       */
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
typedef std::map<int, std::string> map_int_str;
typedef std::map<int, sockaddr_in> map_int_addr;
// typedef std::vector<Channel*> vec_channel;
typedef std::vector<Client*> vec_client;


bool    isExist(vec_client clients, int fd);


#endif