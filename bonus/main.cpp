/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 10:25:38 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/10/17 21:39:33 by ybenlafk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>
#include <map>
#include <cstring>
#include <cstdlib>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <fcntl.h>
#include <vector>
#include <string>

std::vector<std::string> getFileNames(std::string file)
{
    std::vector<std::string> res;
    std::ifstream ifs(file);
    std::string line;
    while (std::getline(ifs, line))
        res.push_back(line);
    ifs.close();
    return (res);
}

std::string getRandomSong(std::vector<std::string> files)
{
    std::srand(std::time(NULL));
    return (files[std::rand() % files.size()]);
}

void ft_send(int fd, std::string msg)
{
    send(fd, msg.c_str(), msg.length(), 0);
}

std::string   strTrim(std::string str, std::string chars)
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

std::string getValue(std::string str, char c)
{
    std::string value = "";
    int i = 0;
    if (str[i] == ':') i++;
    while (str[i] && str[i] != c) i++;
    i++;
    while (str[i]) value += str[i++];
    return (strTrim(value, " \t\r\n"));
}


void playMP3(const std::string& filePath)
{
    std::string command = "open " + filePath;
    system(command.c_str());
}

int main(int ac, char **av)
{
    try
    {
        if (ac == 4)
        {
            std::string av1 = av[1];
            std::string host = av[2];
            std::string pw = av[3];
            std::vector<std::string> files = getFileNames("./songs.config");
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
            std::string nick = "tchipa";
            ft_send(sock, "PASS " + pw + "\r\n");
            ft_send(sock, "NICK " + nick + "\r\n");
            ft_send(sock, "USER " + nick + " 0 * :" + nick + "\r\n");
            while(true)
            {
                char buffer[1024];
                int fd = recv(sock, buffer, 1023, 0);
                if (fd < 0) continue;
                buffer[fd] = '\0';
                std::string msg = strTrim(buffer, " \t\r\n");
                msg = getValue(msg, ':');
                std::string song = getRandomSong(files);
                if (msg == "play")
                {
                    playMP3(song);
                    std::system("clear");
                    std::cout << "\033[1;32m ♬ the song is playing... \033[0m" << std::endl;
                }
                else
                    ft_send(sock, "421 tchipa :Unknown command\r\n");
            }
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

// ─●─────────────── ⇆
// 　◁　　❚❚　　▷　　↻