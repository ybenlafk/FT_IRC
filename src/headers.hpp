/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   headers.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 20:45:24 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/09/25 15:29:48 by ybenlafk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PP
# define PP
#include <iostream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <netdb.h>
#include <sys/types.h>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <algorithm>
#include <cstdio>
#include <ctime>
#include <arpa/inet.h>

typedef struct pollfd pollfd;
typedef std::vector<pollfd> vec_pollfd;
typedef std::vector<std::string> vec_str;
typedef std::vector<int> vec_int;

#endif