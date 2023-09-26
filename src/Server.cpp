/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 21:07:17 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/09/26 10:40:03 by ybenlafk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool    Server::AddClient(std::string pw, int i)
{
    std::string word = "";
    // get first line
    std::string::size_type p = pw.find("\r\n");
    std::string line = pw.substr(0, p);
    // get second line
    pw = pw.substr(p + 2);
    p = pw.find("\r\n");
    std::string line2 = pw.substr(0, p);
    // get third line
    pw = pw.substr(p + 2);
    p = pw.find("\r\n");
    std::string line3 = pw.substr(0, p);
    // split line into two strings
    std::string::size_type pos = line.find(" ");
    std::string pass = line.substr(0, pos);
    std::string value = line.substr(pos + 1);

    if (pass.compare("PASS"))
        return (true);
    if (value.compare(this->password))
    {
        std::cout << "Wrong password" << std::endl;
        close(this->pollfds[i].fd);
        this->pollfds.erase(this->pollfds.begin() + i);
        return (false);
    }
    else
    {
        std::string::size_type pos = line2.find(" ");
        std::string nick = line2.substr(0, pos);
        std::string nick_value = line2.substr(pos + 1);
        
        std::string::size_type pos2 = line3.find(" 0 * ");
        std::string val = line3.substr(0, pos2);
        std::string real_name = line3.substr(pos2 + 5);
        
        std::string::size_type pos3 = val.find(" ");
        std::string user = val.substr(0, pos3);
        std::string user_value = val.substr(pos3 + 1);
    
        if (nick.compare("NICK") || user.compare("USER"))
            return (true);
        // check if the nick is already taken
        for (size_t i = 0; i < this->clients.size(); i++)
        {
            if (!this->clients[i]->getNickName().compare(nick_value))
            {
                std::cout << "Nick already taken" << std::endl;
                close(this->pollfds[i].fd);
                this->pollfds.erase(this->pollfds.begin() + i);
                return (false);
            }
        }
        std::cout << "Client " << nick_value << " connected" << std::endl;
        this->clients.push_back(new Client(this->pollfds[i].fd, nick_value, user_value, real_name));
    }
    return (true);
}

std::string info = "";
void Server::handleClients(int ServerSocket)
{
    static int l;
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
                    info += buffer;
                    if (l++ == 2)
                    {
                        if (!AddClient(info, i))
                            break;
                        l = 0; info = "";
                    }
                }
            }
        }
    }
}

void    Server::run()
{
    int    ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    
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
        else
            this->handleClients(ServerSocket);
    }
}