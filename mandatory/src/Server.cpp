/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 21:07:17 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/10/17 21:38:51 by ybenlafk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void    fillBuffers(vec_str &buffers, std::string str)
{
    int i = 0;
    std::string word = "";
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
    tmp = utils::getCmd(cmd, ' ');
    
    size_t j = 0;
    for (j = 0; j < cmd.length(); j++) if (cmd[j] == ' ') break;
    for (; j < cmd.length(); j++) pw += cmd[j];
    pw = utils::strTrim(pw, "\r\n\t ");

    if (tmp == "PASS")
    {
        for (vec_client::iterator tt = clients.begin(); tt != clients.end(); tt++)
        {
            if ((*tt)->getFd() == fd)
            {
                if ((*tt)->getPw() == false)
                {
                    if (pw != password) return (1);
                    else (*tt)->setPw(true);
                }
            }
        }
    }
    else if (tmp == "NICK")
    {
        for (vec_client::iterator it = clients.begin(); it != clients.end(); it++)
            if (pw == (*it)->getNickName() && (*it)->getFd() != fd)
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
        if (clients[i]->getFd() == fd)
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
                sockaddr_in user_addr;
                socklen_t user_len = sizeof(user_addr);
                int clientSocket = accept(ServerSocket, reinterpret_cast<sockaddr*>(&user_addr), &user_len);
                if (clientSocket < 0)
                    throw std::runtime_error("accept() failed");
                fcntl(ServerSocket, F_SETFL, O_NONBLOCK);
                struct pollfd pollfdClient;
                pollfdClient.fd = clientSocket;
                pollfdClient.events = POLLIN;
                pollfdClient.revents = 0;
                this->pollfds.push_back(pollfdClient);
                this->popers[clientSocket] = "";
                this->addrs[clientSocket] = user_addr;
                
            }
            else
            {
                char buffer[FD_SETSIZE];
                std::memset(buffer, 0, FD_SETSIZE);
                ssize_t bytesRead = recv(this->pollfds[i].fd, buffer, FD_SETSIZE - 1, 0);
                // for (int p = 0; p < (int )buffers.size(); p++)
                //     std::cout << buffers[p] << std::endl;
                if (bytesRead < 0)
                {
                    close(this->pollfds[i].fd);
                    this->pollfds.erase(this->pollfds.begin() + i);
                    i--;
                }
                else if (bytesRead == 0)
                {
                    Cmds::cmdQuit(this->clients, this->pollfds[i].fd, "client disconnected", this->channels);
                    break;
                }
                buffer[bytesRead] = '\0';
                fillBuffers(this->buffers, buffer);
                if (bytesRead > 0)
                {
                    for (size_t v = 0; v < this->buffers.size(); v++)
                    {
                        int res = bufferChecker(buffers[v], this->popers[this->pollfds[i].fd]);
                        // std::cout <<  this->popers[this->pollfds[i].fd] << std::endl;
                        if (res == 0)
                            continue;
                        else if (res == 2)
                        {
                            utils::ft_send(this->pollfds[i].fd, "417 * :Too long command\r\n");
                            continue;
                        }
                        if (isExist(this->clients, this->pollfds[i].fd))
                            clients.push_back(new Client(this->pollfds[i].fd, "", "", "", false));
                        vec_client::iterator it = clients.begin();
                        for (; it != clients.end(); it++)
                        {
                            if ((*it)->getFd() == this->pollfds[i].fd)
                            {
                                if ((*it)->getAuth() == false)
                                {
                                    int res = AddClient(this->popers[this->pollfds[i].fd], *it, this->pollfds[i].fd);
                                    if (res == 1)
                                        utils::ft_send(this->pollfds[i].fd, "464 * :Password incorrect\r\n");
                                    else if (res == 2)
                                        utils::ft_send(this->pollfds[i].fd, "433 * :Nickname is already in use\r\n");
                                    else if (res == 0 && (*it)->getAuth() == true)
                                    {
                                        std::cout << "Client " << (*it)->getNickName() <<" connected" << std::endl;
                                        (*it)->setIpAddr(utils::get_ip(this->addrs[this->pollfds[i].fd]));
                                        utils::reply(this->pollfds[i].fd, "001 " +(*it)->getNickName()+ " :Welcome to the Internet Relay Network\r\n", (*it)->getPrifex());
                                    }
                                }
                                else if ((*it)->getAuth() == true)
                                {
                                    std::string cmds[11] = {"NICK" , "JOIN", "MODE" ,"QUIT" ,"KICK" , "INVITE", "TOPIC", "PRIVMSG", "PART", "PASS", "USER"};
                                    std::string cmd = utils::strTrim(this->popers[this->pollfds[i].fd], "\r\n\t ");
                                    cmd = utils::getCmd(this->popers[this->pollfds[i].fd], ' ');
                                    cmd = utils::strTrim(cmd, "\r\n\t ");
                                    std::string value = utils::getValue(this->popers[this->pollfds[i].fd], ' ');
                                    value = utils::strTrim(value, "\r\n\t ");
                                    size_t l = 0;
                                    for (; l < 11; l++)
                                        if (cmd == cmds[l]) break;
                                    switch (l)
                                    {
                                        case 0:
                                            Cmds::cmdNick(clients, this->pollfds[i].fd, value);
                                            break;
                                        case 1:
                                            Cmds::cmdJoin(this->channels, clients, this->pollfds[i].fd, value);
                                            break;
                                        case 2:
                                            Cmds::cmdMode(this->channels, clients, this->pollfds[i].fd, value);
                                            break;
                                        case 3:
                                            Cmds::cmdQuit(clients, this->pollfds[i].fd, value, this->channels);
                                            break;
                                        case 4:
                                            Cmds::cmdKick(this->channels, clients, this->pollfds[i].fd, value);
                                            break;
                                        case 5:
                                            // INVITE handler
                                            break;
                                        case 6:
                                            // TOPIC handler
                                            break;
                                        case 7:
                                            Cmds::cmdPrivmsg(clients, this->pollfds[i].fd, value, this->channels);
                                            break;
                                        case 8:
                                            Cmds::cmdPart(this->channels, clients, this->pollfds[i].fd, value);
                                            break;
                                        case 9:
                                            utils::reply(this->pollfds[i].fd, "462 * :You may not reregister\r\n", (*it)->getPrifex());
                                            break;
                                        case 10:
                                            utils::reply(this->pollfds[i].fd, "462 * :You may not reregister\r\n", (*it)->getPrifex());
                                            break;
                                    default:
                                        utils::reply(this->pollfds[i].fd, "421 * :Unknown command\r\n", (*it)->getPrifex());
                                        break;
                                    }
                                }
                                this->popers[this->pollfds[i].fd] = "";
                                break;
                            }
                        }
                    }
                    this->buffers.clear();
                }
            }
        }
    }
}

void    Server::run()
{
    int    ServerSocket = utils::setUpServer(&this->clients, this->port);
    std::cout << "\033[1;32m" << utils::getHostName() << "\033[0m" << std::endl;
    fcntl(ServerSocket, F_SETFL, O_NONBLOCK);
    struct pollfd pollfdServer;
    pollfdServer.fd = ServerSocket;
    pollfdServer.events = POLLIN;
    pollfdServer.revents = 0;
    this->pollfds.push_back(pollfdServer);
    while (true)
    {
        int pl = poll(this->pollfds.data(), this->pollfds.size(), 0);
        if (pl < 0)
            throw std::runtime_error("poll() failed");
        else if (pl == 0)
            continue;
        this->handleClients(ServerSocket);
    }
}