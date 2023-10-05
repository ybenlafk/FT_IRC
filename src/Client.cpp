/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 09:29:47 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/10/05 16:30:30 by ybenlafk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "utils.hpp"

Client::Client() {}

Client::Client(Client const &src)
{
    if (this != &src)
    {
        this->fd = src.fd;
        this->nick_name = src.nick_name;
        this->user_name = src.user_name;
        this->real_name = src.real_name;
        this->channels = src.channels;
        this->pw = src.pw;
        this->auth = src.auth;
        this->admin = src.admin;
    }
}

Client::~Client() {}

Client::Client(int fd, std::string nick_name, std::string user_name, std::string real_name, bool auth, bool admin) : fd(fd), nick_name(nick_name), user_name(user_name), real_name(real_name), auth(auth), admin(admin) {}

void           Client::add_channel(std::string &name, bool admin) {this->channels[name] = admin;}

m_channel      &Client::getChannels() {return (this->channels);}

void           Client::setAuth(bool auth) {this->auth = auth;}

void           Client::setFd(int fd) {this->fd = fd;}

void           Client::setPw(bool pw) {this->pw = pw;}

void           Client::setNickName(std::string nick_name) {this->nick_name = nick_name;}

void           Client::setUserName(std::string user_name) {this->user_name = user_name;}

void           Client::setRealName(std::string real_name) {this->real_name = real_name;}

void           Client::setAdmin(bool admin) {this->admin = admin;}

bool           Client::getAuth() const {return (this->auth);}

bool           Client::getPw() const {return (this->pw);}

int            Client::getFd() const {return (this->fd);}

std::string    Client::getNickName() const {return (this->nick_name);}

std::string    Client::getUserName() const {return (this->user_name);}

std::string    Client::getRealName() const {return (this->real_name);}

bool           Client::getAdmin() const {return (this->admin);}

std::string   Client::getPrifex()
{
    return this->nick_name + (user_name.empty() ? "" : "!" + utils::getCmd(user_name, ' ')) + (utils::getHostName().empty() ? "" : "@localhost");
}