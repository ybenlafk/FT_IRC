/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 21:07:17 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/10/24 12:04:08 by ybenlafk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void    fillBuffers(vec_str &buffers, std::string str)
{
    int i = 0;
    std::string word = "";
    if (str.empty()) return ;
    while (str[i])
    {
        if (str[i] == '\r' && str[i + 1] == '\n')
        {
            i += 2;
            buffers.push_back(word + "\r\n");
            word = "";
        }
        else if (str[i] == '\n')
        {
            i++;
            buffers.push_back(word + "\r\n");
            word = "";
        }
        else
            word += str[i++];
    }
    if (word != "")
        buffers.push_back(word);
}

int    Server::AddClient(std::string cmd, Client *client, int fd)
{
    std::string pw, nick, user, tmp;
    pw = nick = user = "";
    cmd = utils::strTrim(cmd, "\r\n\t ");
    tmp = utils::getCmd(cmd, ' ');
    
    pw = utils::getValue(cmd, ' ');

    if (tmp == "PASS")
    {
        for (vec_client::iterator tt = clients.begin(); tt != clients.end(); tt++)
        {
            if (tt->getFd() == fd)
            {
                if (tt->getPw() == false)
                {
                    if (pw != password) return (1);
                    else tt->setPw(true);
                }
            }
        }
    }
    else if (tmp == "NICK")
    {
        for (vec_client::iterator it = clients.begin(); it != clients.end(); it++)
            if (pw == it->getNickName() && it->getFd() != fd)
                return (2);
        client->setNickName(pw);
    }
    else if (tmp == "USER")
        client->setUserName(pw);
    if (client->getNickName() != "" && client->getUserName() != "" && client->getPw() == true)
        client->setAuth(true);
    return (0);
}

bool    isExist(vec_client clients, int fd)
{
    for (size_t i = 0; i < clients.size(); i++)
        if (clients[i].getFd() == fd)
            return (false);
    return (true);
}

int    bufferChecker(std::string buffer, std::string &poper)
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

void Server::handleClients(int ServerSocket)
{
    for (size_t i = 0; i < this->pollfds.size(); i++)
    {
        if (this->pollfds[i].revents & POLLIN)
        {
            if (this->pollfds[i].fd == ServerSocket)
            {
                // Accept a client connection
                utils::AcceptConnection(ServerSocket, *this);
            }
            else
            {
                char buffer[FD_SETSIZE];
                std::memset(buffer, 0, FD_SETSIZE);
                ssize_t bytesRead = recv(this->pollfds[i].fd, buffer, FD_SETSIZE - 1, 0);
                if (bytesRead < 0)
                {
                    close(this->pollfds[i].fd);
                    this->pollfds.erase(this->pollfds.begin() + i);
                    i--;
                }
                else if (bytesRead == 0)
                {
                    Cmds::cmdQuit(this->clients, this->pollfds[i].fd, "client disconnected", this->channels, hostname);
                    close(this->pollfds[i].fd);
                    this->pollfds.erase(this->pollfds.begin() + i);
                    break;
                }
                fillBuffers(this->buffers, buffer);
                if (bytesRead > 0)
                {
                    utils::HandleReq(*this, i, hostname);
                    this->buffers.clear();
                }
            }
        }
    }
}

void    Server::run()
{
    hostname = utils::getHostName();
    int    ServerSocket = utils::setUpServer(&this->clients, this->port);
    std::cout << "\033[1;35m───────▷ The hostname : " << hostname << "\033[0m" << std::endl;
    if (fcntl(ServerSocket, F_SETFL, O_NONBLOCK) < 0)
        throw std::runtime_error("fcntl() failed");
    struct pollfd pollfdServer;
    pollfdServer.fd = ServerSocket;
    pollfdServer.events = POLLIN;
    pollfdServer.revents = 0;
    this->pollfds.push_back(pollfdServer);
    while (true)
    {
        int pl = poll(&this->pollfds[0], this->pollfds.size(), 0);
        if (pl < 0)
            throw std::runtime_error("poll() failed");
        else if (pl == 0)
            continue;
        this->handleClients(ServerSocket);
    }
}