/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 20:41:48 by ybenlafk          #+#    #+#             */
/*   Updated: 2023/09/29 16:29:13 by ybenlafk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "src/Server.hpp"

bool ParseInput(std::string port, std::string pw)
{
    if (port.empty() || pw.empty())
        return (false);
    if (port.size() > 5 || pw.size() > 20)
        return (false);
    for (size_t i = 0; i < port.size(); i++)
        if (!std::isdigit(port[i]))
            return (false);
    return (true);
}

int main(int ac, char **av)
{
    if (ac != 3)
    {
        std::cout << "Usage: ./server <port> <password>" << std::endl;
        return 1;
    }
    try
    {
        if (!ParseInput(av[1], av[2]))
            throw std::runtime_error("Invalid input");
        Server server(std::atoi(av[1]), av[2]);
        server.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return (0);
}