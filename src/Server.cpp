/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 21:07:17 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/09/25 16:52:42 by ybenlafk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool    Server::isValidPw(std::string pw, int i)
{
    // split pw into two strings
    std::string::size_type pos = pw.find(" ");
    std::string pw1 = pw.substr(0, pos);
    std::string pw2 = pw.substr(pos + 1);

    if (pw1.compare("PASS"))
        return true;
    if (pw2.compare(this->password + "\r\n"))
    {
        std::cout << "Wrong password" << std::endl;
        close(this->pollfds[i].fd);
        this->pollfds.erase(this->pollfds.begin() + i);
        return false;
    }
    else
    {
        std::cout << "Client connected" << std::endl;
        this->clients.push_back(this->pollfds[i].fd);
    }
    return true;
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
                        std::cout << "{" << buffer << "}"<< std::endl;
                        if (!isValidPw(buffer, i))
                            break;
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