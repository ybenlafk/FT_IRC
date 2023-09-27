/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 21:07:17 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/09/27 16:58:54 by ybenlafk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"


int    Server::AddClient(std::string cmd, int i)
{
    std::string pw, nick, user, tmp;
    pw = nick = user = "";
    tmp = utils::getCmd(cmd, ' ');
    
    int j = 0;
    for (j = 0; j < cmd.length(); j++) if (cmd[j] == ' ') break;
    for (; j < cmd.length(); j++) pw += cmd[j];
    pw = utils::strTrim(pw, "\r\n\t ");
    
    if (tmp == "PASS")
    {
        for (vec_client::iterator it = clients.begin(); it != clients.end(); it++)
        {
            if ((*it)->getFd() == this->pollfds[i].fd)
            {
                if ((*it)->getPw() == false)
                {
                    if (pw != password) return (1); 
                    else (*it)->setPw(true);
                }
            }
        }
    }
    else if (tmp == "NICK")
    {
        for (vec_client::iterator it = clients.begin(); it != clients.end(); it++)
            if (pw == (*it)->getNickName() && (*it)->getFd() != this->pollfds[i].fd)
                return (2);
        clients[i]->setNickName(pw);
    }
    else if (tmp == "USER")
        clients[i]->setUserName(pw);
    if (clients[i]->getNickName() != "" && clients[i]->getUserName() != "")
        clients[i]->setAuth(true);
    return (0);
}

bool    isExist(vec_client clients, int fd)
{
    for (size_t i = 0; i < clients.size(); i++)
        if (clients[i]->getFd() == fd)
            return (false);
    return (true);
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
                int clientSocket = accept(ServerSocket, NULL, NULL);
                if (clientSocket < 0)
                    throw std::runtime_error("accept() failed");
                struct pollfd pollfdClient;
                pollfdClient.fd = clientSocket;
                pollfdClient.events = POLLIN;
                pollfdClient.revents = 0;
                this->pollfds.push_back(pollfdClient);
            }
            else
            {
                char buffer[FD_SETSIZE];
                memset(buffer, 0, FD_SETSIZE);
                ssize_t bytesRead = recv(this->pollfds[i].fd, buffer, FD_SETSIZE - 1, 0);
                if (bytesRead < 0)
                {
                    close(this->pollfds[i].fd);
                    this->pollfds.erase(this->pollfds.begin() + i);
                    i--;
                }
                else if (bytesRead == 0)
                {
                    std::cout << "Client disconnected" << std::endl;
                    close(this->pollfds[i].fd);
                    this->pollfds.erase(this->pollfds.begin() + i);
                    break;
                }
                if (bytesRead > 0)
                {
                    if (isExist(this->clients, this->pollfds[i].fd))
                        clients.push_back(new Client(this->pollfds[i].fd, "", "", "", false));
                    for (int j = 0; j < clients.size(); j++)
                    {
                        if (clients[j]->getFd() == this->pollfds[i].fd)
                        {
                            if (clients[j]->getAuth() == false)
                            {
                                int res = AddClient(buffer, j);
                                if (res == 1)
                                {
                                    std::string msg = "NOTICE user :Wrong password.\r\n";
                                    send(this->pollfds[i].fd, msg.c_str(), msg.length(), 0);
                                }
                                else if (res == 2)
                                {
                                    std::string msg = "NOTICE user :Nickname is already in use.\r\n";
                                    send(this->pollfds[i].fd, msg.c_str(), msg.length(), 0);
                                }
                                else if (res == 0 && clients[j]->getAuth() == true)
                                {
                                    std::string msg = "NOTICE user :Welcome to the Internet Relay Network " + clients[j]->getNickName() + "\r\n";
                                    send(this->pollfds[i].fd, msg.c_str(), msg.length(), 0);
                                }
                            }
                            else if (clients[j]->getAuth() == true)
                            {
                                // dofus
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
}

void    Server::run()
{
    int    ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    this->clients.push_back(new Client(-1, "", "", "", false));
    if (ServerSocket < 0)
        throw std::runtime_error("socket() failed");

    int optval = 1;
    if (setsockopt(ServerSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
        throw   std::runtime_error("setsockopt() failed");

    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    if (bind(ServerSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
        throw   std::runtime_error("bind() failed");

    if (listen(ServerSocket, FD_SETSIZE) < 0)
        throw std::runtime_error("listen() failed");
    std::cout << "Listening on port " << port << std::endl;

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