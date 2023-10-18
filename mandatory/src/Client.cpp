/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 09:29:47 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/10/18 18:13:49 by ybenlafk         ###   ########.fr       */
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
    }
}

Client::~Client() {}

Client::Client(int fd, std::string nick_name, std::string user_name, std::string real_name, bool auth) : fd(fd), nick_name(nick_name), user_name(user_name), real_name(real_name), auth(auth) {}

void           Client::add_channel(std::string &name, bool admin) {this->channels[name] = admin;}

m_channel      &Client::getChannels() {return (this->channels);}

void           Client::setAuth(bool auth) {this->auth = auth;}

void           Client::setFd(int fd) {this->fd = fd;}

void           Client::setPw(bool pw) {this->pw = pw;}

void           Client::setNickName(std::string nick_name) {this->nick_name = nick_name;}

void            Client::setIpAddr(std::string ip_addr) {this->ip_addr = ip_addr;}

void           Client::setUserName(std::string user_name)
{
    std::string tmp = "";
    size_t i = 0;
    while (user_name[i] && user_name[i] != ' ') tmp += user_name[i++];
    this->user_name = utils::strTrim(tmp, "\r\n\t ");
    if (user_name[i] && user_name[i] == ' ') i++;
    if (user_name[i] && user_name[i] == '0') i++;
    if (user_name[i] && user_name[i] == ' ') i++;
    if (user_name[i] && user_name[i] == '*') i++;
    if (user_name[i] && user_name[i] == ' ') i++;
    tmp = "";
    while (user_name[i]) tmp += user_name[i++];
    this->real_name = utils::strTrim(tmp, "\r\n\t ");
}

void           Client::setRealName(std::string real_name) {this->real_name = real_name;}

bool           Client::getAuth() const {return (this->auth);}

bool           Client::getPw() const {return (this->pw);}

int            Client::getFd() const {return (this->fd);}

std::string    Client::getNickName() const {return (this->nick_name);}

std::string    Client::getUserName() const {return (this->user_name);}

std::string    Client::getRealName() const {return (this->real_name);}

std::string    Client::getIpAddr() const {return (this->ip_addr);}

std::string   Client::getPrifex()
{
    return this->nick_name + (user_name.empty() ? "" : "!" + utils::getCmd(user_name, ' ')) + (this->getIpAddr().empty() ? "" : "@" + this->getIpAddr());
    // return this->nick_name + (user_name.empty() ? "" : "!" + utils::getCmd(user_name, ' ')) + (utils::getHostName().empty() ? "" : "@" + utils::getHostName());
}