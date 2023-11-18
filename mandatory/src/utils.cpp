/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 13:47:58 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/10/23 23:27:13 by ybenlafk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

std::string utils::getCmd(std::string str, char c)
{
    std::string cmd = "";
    int i = 0;
    while (str[i] && str[i] != c) cmd += str[i++];
    return (cmd);
}

void utils::AcceptConnection(int ServerSocket, Server &server)
{
    std::cout << "\033[1;32m● Connecting...\033[0m" << std::endl;
    sockaddr_in user_addr;
    socklen_t user_len = sizeof(user_addr);
    int clientSocket = accept(ServerSocket, (sockaddr *)&user_addr, &user_len);
    if (clientSocket < 0)
        throw std::runtime_error("accept() failed");
    if (fcntl(clientSocket, F_SETFL, O_NONBLOCK) < 0)
        throw std::runtime_error("fcntl() failed");
    struct pollfd pollfdClient;
    pollfdClient.fd = clientSocket;
    pollfdClient.events = POLLIN;
    pollfdClient.revents = 0;
    server.pollfds.push_back(pollfdClient);
    server.popers[clientSocket] = "";
    server.addrs[clientSocket] = user_addr;
}


void    utils::HandleReq(Server &server, int i, std::string hostname)
{
    for (size_t v = 0; v < server.buffers.size(); v++)
    {
        int res = utils::bufferChecker(server.buffers[v], server.popers[server.pollfds[i].fd]);
        if (res == 0)
            continue;
        else if (res == 2)
        {
            utils::ft_send(server.pollfds[i].fd, "417 * :Too long command\r\n");
            continue;
        }
        if (isExist(server.clients, server.pollfds[i].fd))
            server.clients.push_back(Client(server.pollfds[i].fd, "", "", "", false));
        vec_client::iterator it = server.clients.begin();
        for (; it != server.clients.end(); it++)
        {
            if (it->getFd() == server.pollfds[i].fd)
            {
                if (it->getAuth() == false)
                {
                    int res = server.AddClient(server.popers[server.pollfds[i].fd], &(*it), server.pollfds[i].fd);
                    if (res == 1)
                        utils::ft_send(server.pollfds[i].fd, "464 * :Password incorrect\r\n");
                    else if (res == 2)
                        utils::ft_send(server.pollfds[i].fd, "433 * :Nickname is already in use\r\n");
                    else if (res == 0 && it->getAuth() == true)
                    {
                        std::cout << "\033[1;32m● Client " << it->getNickName() <<" connected\033[0m" << std::endl;
                        it->setIpAddr(utils::get_ip(server.addrs[server.pollfds[i].fd]));
                        utils::reply(server.pollfds[i].fd, "001 " + it->getNickName()+ " :Welcome to Tchipa's IRC server 🤪\r\n", it->getPrifex(hostname));
                    }
                }
                else if (it->getAuth() == true)
                    utils::ExecCmds(server, i, it, hostname);
                server.popers[server.pollfds[i].fd] = "";
                break;
            }
        }
    }
}


void    utils::ExecCmds(Server &server, int i, vec_client::iterator &it, std::string hostname)
{
    std::string cmds[12] = {"NICK" , "JOIN", "MODE" ,"QUIT" ,"KICK" , "INVITE", "TOPIC", "PRIVMSG", "PART", "PASS", "USER", "PONG"};
    std::string cmd = utils::strTrim(server.popers[server.pollfds[i].fd], "\r\n\t ");
    cmd = utils::getCmd(server.popers[server.pollfds[i].fd], ' ');
    cmd = utils::strTrim(cmd, "\r\n\t ");
    std::string value = utils::getValue(server.popers[server.pollfds[i].fd], ' ');
    value = utils::strTrim(value, "\r\n\t ");
    size_t l = 0;
    for (; l < 12; l++)
        if (cmd == cmds[l]) break;
    switch (l)
    {
        case 0:
            Cmds::cmdNick(server.clients, value, hostname, server.channels, &(*it));
            break;
        case 1:
            Cmds::cmdJoin(server.channels, value, hostname, &(*it));
            break;
        case 2:
            Cmds::cmdMode(server.channels, server.clients, server.pollfds[i].fd, value, hostname, &(*it));
            break;
        case 3:
            Cmds::cmdQuit(server.clients, server.pollfds[i].fd, value, server.channels, hostname);
            break;
        case 4:
            Cmds::cmdKick(server.channels, server.clients,  value, hostname, &(*it));
            break;
        case 5:
            Cmds::cmdInvite(server.channels, server.clients, server.pollfds[i].fd, value, hostname, &(*it));
            break;
        case 6:
            Cmds::cmdTopic(server.channels, value, hostname, &(*it));
            break;
        case 7:
            Cmds::cmdPrivmsg(server.clients, server.pollfds[i].fd, value, server.channels, hostname, &(*it));
            break;
        case 8:
            Cmds::cmdPart(server.channels, server.clients, value, hostname, &(*it));
            break;
        case 9:
            utils::reply(server.pollfds[i].fd, "462 * :You may not reregister\r\n", it->getPrifex(hostname));
            break;
        case 10:
            utils::reply(server.pollfds[i].fd, "462 * :You may not reregister\r\n", it->getPrifex(hostname));
            break;
        case 11:
            break;
    default:
        utils::reply(server.pollfds[i].fd, "421 * :Unknown command\r\n", it->getPrifex(hostname));
        break;
    }
}


std::string utils::getValue(std::string str, char c)
{
    std::string value = "";
    int i = 0;
    while (str[i] && str[i] != c) i++;
    if (str[i] == c) i++;
    else return ("");
    while (str[i]) value += str[i++];
    return (utils::strTrim(value, " \t\r\n"));
}

std::string   utils::strTrim(std::string str, std::string chars)
{
    std::string::size_type pos = str.find_last_not_of(chars);
    if (pos != std::string::npos)
        str.erase(pos + 1);
    else
        str.clear();
    pos = str.find_first_not_of(chars);
    if (pos != std::string::npos)
        str.erase(0, pos);
    return (str);
}

void         utils::spaceSkipe(std::string &str)
{
    size_t i = 0;
    for (; i < str.length(); i++) if (str[i] != ' ')break;
    str.erase(0, i);
}

int     utils::setUpServer(vec_client *clients, int port)
{
    int    ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    clients->push_back(Client(-1, "", "", "", false));
    if (ServerSocket < 0)
        throw std::runtime_error("socket() failed");

    int optval = 1;
    if (setsockopt(ServerSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
        throw   std::runtime_error("setsockopt() failed");
    if (port < 0 || port > 65535)
        throw   std::runtime_error("Invalid port");
    struct sockaddr_in serverAddress;
    std::memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    if (bind(ServerSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
        throw   std::runtime_error("bind() failed");

    if (listen(ServerSocket, FD_SETSIZE) < 0)
        throw std::runtime_error("listen() failed");
    std::cout << "\033[1;36m──────────────▷ Irc server listening on port " << port <<"... ◁──────────────\033[0m" << std::endl;
    return (ServerSocket);
}

void    utils::split(std::string str, char c, vec_str *names, vec_str *keys)
{
    size_t i = 0;
    std::string word;

    while (str[i] && str[i] != ' ' && str[i] != '\t')
    {
        if (str[i] == c)
        {
            names->push_back(word);
            word = "";
        }
        else
            word += str[i];
        i++;
    }
    if (!word.empty())
        names->push_back(word);
    word = "";
    while (str[i] && (str[i] == ' ' || str[i] == '\t')) i++;
    
    while (str[i] && str[i] != '\r' && str[i] != '\n')
    {
        if (str[i] == c)
        {
            keys->push_back(word);
            word = "";
        }
        else
            word += str[i];
        i++;
    }
    if (!word.empty())
        keys->push_back(word);
}

void        utils::ft_send(int fd, std::string msg) {send(fd, msg.c_str(), msg.length(), 0);}

bool         utils::isValidName(std::string name)
{
    if (name[0] != '#')
        return (false);
    if (name.length() < 2)
        return (false);
    for (size_t i = 1; i < name.length(); i++)
        if (!isalnum(name[i]) && name[i] != '_')
            return (false);
    return (true);
}

std::string utils::getHostName()
{
    if (std::system("uname -n > /tmp/host.txt") == -1)
        throw std::runtime_error("system() failed");
    std::ifstream file("/tmp/host.txt");
    if (!file.is_open())
        throw std::runtime_error("open() failed");
    std::string res;
    std::getline(file, res);
    file.close();
    if (std::system("rm -rf /tmp/host.txt") == -1)
        throw std::runtime_error("system() failed");
    return (res);
}

int    utils::split(std::string str, char c, vec_str *names, std::string *reason)
{
    size_t i = 0;
    std::string word = "";

    while (str[i] && str[i] != ' ' && str[i] != '\t' && str[i] != ':')
    {
        if (str[i] == c)
        {
            word = utils::strTrim(word, " \t\r\n");
            names->push_back(word);
            word = "";
        }
        else
            word += str[i];
        i++;
    }
    if (!word.empty())
        names->push_back(word);
    else
        return (0);
    word = "";
    while (str[i] && (str[i] == ' ' || str[i] == '\t')) i++;
    if (str[i] == ':') i++;
    while (str[i] && str[i] != '\r' && str[i] != '\n')
    {
        word += str[i];
        i++;
    }
    *reason = word;
    return (1);
}

void    utils::reply(int fd, std::string msg, std::string prefix)
{
    msg = ":" + prefix + " " + msg;
    utils::ft_send(fd, msg);
}

std::string     utils::get_ip(sockaddr_in user_addr)
{
    return (inet_ntoa(user_addr.sin_addr));
}

int    utils::bufferChecker(std::string buffer, std::string &poper)
{
    size_t i = buffer.size();
    if (i > 512) return (2);
    i--;
    poper.append(buffer);
    if (i >= 1 && buffer[i] == '\n' && buffer[i - 1] == '\r')
        return (1);
    if (buffer[i] == '\n')
        return (1);
    return (0);
}