/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 10:48:22 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/10/06 14:40:59 by ybenlafk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

void        Channel::add_client(Client client) {this->clients.push_back(Client(client));}

void        Channel::set_name(std::string name) {this->name = name;}

void        Channel::set_topic(std::string topic) {this->topic = topic;}

void        Channel::set_mode(std::string mode) {this->mode = mode;}

void        Channel::set_key(std::string key) {this->key = key;}

void        Channel::set_pw(bool pw) {this->pw = pw;}

void        Channel::set_invite_only(bool invite_only) {this->invite_only = invite_only;}

void        Channel::set_topic_changeable(bool topic_changeable) {this->topic_changeable = topic_changeable;}

void        Channel::set_limit(int limit) {this->limit = limit;}

vec_member  &Channel::get_clients() {return this->clients;}

std::string Channel::get_name() const {return this->name;}

std::string Channel::get_topic() const {return this->topic;}

std::string Channel::get_mode() const {return this->mode;}

std::string Channel::get_key() const {return this->key;}

bool        Channel::get_pw() const {return this->pw;}

bool        Channel::get_invite_only() const {return this->invite_only;}

bool        Channel::get_topic_changeable() const {return this->topic_changeable;}

int         Channel::get_limit() const {return this->limit;}

std::string  Channel::get_members()
{
    std::string res;
    for (size_t i = 0; i < this->clients.size(); i++)
    {
        if (this->clients[i].getChannels()[this->name])
            res += "@";
        res += this->clients[i].getNickName();
        if (i + 1 < this->clients.size())
            res += " ";
    }
    return (res);
}