/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 09:47:05 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/09/26 09:48:07 by ybenlafk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LL
# define LL

#include "headers.hpp"
#include "Client.hpp"
typedef struct pollfd pollfd;
typedef std::vector<pollfd> vec_pollfd;
typedef std::vector<std::string> vec_str;
typedef std::vector<Client*> vec_client;

#endif