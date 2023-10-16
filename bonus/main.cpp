/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 10:25:38 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/10/16 14:21:54 by ybenlafk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>
#include <map>
#include <cstring>
#include <cstdlib>
#include <netdb.h>
#include <sys/types.h>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <algorithm>
#include <cstdio>
#include <ctime>
#include <arpa/inet.h>
#include <fstream>
#include <fcntl.h>

void ft_send(int fd, std::string msg)
{
    send(fd, msg.c_str(), msg.length(), 0);
}

int main(int ac, char **av)
{
    if (ac == 4)
    {
        std::string av1 = av[1];
        std::string host = av[2];
        std::string pw = av[3];
        if (av1.empty() || host.empty() || pw.empty()) return (0);
        int port  = std::atoi(av1.c_str());

        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == -1) return (0);
        struct hostent *hostinfo = gethostbyname(host.c_str());
        if (hostinfo == NULL) return (0);
        
        struct sockaddr_in add;
        std::memset(&add, 0, sizeof(add));
        add.sin_addr = *(struct in_addr *) hostinfo->h_addr;
        add.sin_family = AF_INET;
        add.sin_port = htons(port);

        if (connect(sock, (struct sockaddr *)&add, sizeof(add)) == -1) return (0);
        fcntl(sock, F_SETFL, O_NONBLOCK);
        
        ft_send(sock, "PASS " + pw + "\r\n");
        ft_send(sock, "NICK TCHIPA\r\n");
        ft_send(sock, "USER TCHIPA 0 * TCHIPA\r\n");
        while(true)
        {
            
        }
    }
}