/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 09:29:47 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/09/26 09:40:10 by ybenlafk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client() {}

Client::~Client() {}

Client::Client(int fd, std::string nick_name, std::string user_name, std::string real_name) : fd(fd), nick_name(nick_name), user_name(user_name), real_name(real_name) {}

int            Client::getFd() const {return (this->fd);}

std::string    Client::getNickName() const {return (this->nick_name);}

std::string    Client::getUserName() const {return (this->user_name);}

std::string    Client::getRealName() const {return (this->real_name);}