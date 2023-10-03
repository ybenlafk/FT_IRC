/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 10:48:09 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/10/03 21:53:13 by ybenlafk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPS
# define OPS

#include "headers.hpp"
#include "Client.hpp"

class Client;

typedef std::vector<Client*> vec_client;

class Channel
{
    private:
        std::string name;
        vec_client clients;
        std::string topic;
        std::string mode;
        std::string key;
        bool        pw;
        bool        invite_only;
        bool        topic_changeable;
        int         limit;
    public:
        Channel(){}
        Channel(std::string name, std::string key) : name(name), key(key) {}
        ~Channel(){}
        
        void        add_client(Client *client);
        
        void        set_name(std::string name);
        void        set_topic(std::string topic);
        void        set_mode(std::string mode);
        void        set_key(std::string key);
        void        set_pw(bool pw);
        void        set_invite_only(bool invite_only);
        void        set_topic_changeable(bool topic_changeable);
        void        set_limit(int limit);
        
        vec_client  get_clients() const;
        std::string get_name() const;
        std::string get_topic() const;
        std::string get_mode() const;
        std::string get_key() const;
        bool        get_pw() const;
        bool        get_invite_only() const;
        bool        get_topic_changeable() const;
        int         get_limit() const;
};

#endif