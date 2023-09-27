/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 09:29:47 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/09/27 16:43:50 by ybenlafk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client() {}

Client::~Client() {}

Client::Client(int fd, std::string nick_name, std::string user_name, std::string real_name, bool auth) : fd(fd), nick_name(nick_name), user_name(user_name), real_name(real_name), auth(auth) {}

void           Client::setAuth(bool auth) {this->auth = auth;}

void           Client::setFd(int fd) {this->fd = fd;}

void           Client::setPw(bool pw) {this->pw = pw;}

void           Client::setNickName(std::string nick_name) {this->nick_name = nick_name;}

void           Client::setUserName(std::string user_name) {this->user_name = user_name;}

void           Client::setRealName(std::string real_name) {this->real_name = real_name;}

bool           Client::getAuth() const {return (this->auth);}

bool           Client::getPw() const {return (this->pw);}

int            Client::getFd() const {return (this->fd);}

std::string    Client::getNickName() const {return (this->nick_name);}

std::string    Client::getUserName() const {return (this->user_name);}

std::string    Client::getRealName() const {return (this->real_name);}