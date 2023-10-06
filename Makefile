# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/22 20:41:53 by ybenlafk          #+#    #+#              #
#    Updated: 2023/10/06 13:01:43 by ybenlafk         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME= ircserv
SRC=src/Server.cpp \
	src/Client.cpp \
	src/utils.cpp \
	src/commands/PART.cpp \
	src/commands/INVITE.cpp \
	src/commands/JOIN.cpp \
	src/commands/KICK.cpp \
	src/commands/MODE.cpp \
	src/commands/NICK.cpp \
	src/commands/PRIVMSG.cpp \
	src/commands/QUIT.cpp \
	src/commands/TOPIC.cpp \
	src/Channel.cpp \
	main.cpp
OBJ= $(patsubst src/%.cpp, objs/%.o, $(SRC))
CFLAGS= -Wall -Wextra -Werror -std=c++98  #-fsanitize=address -g3

all: $(NAME)

$(NAME): objs $(OBJ)
	c++ $(CFLAGS) $(OBJ) -o $(NAME)

objs:
	mkdir -p objs/commands

objs/%.o: src/%.cpp src/*.hpp | objs
	c++ $(CFLAGS) -c $< -o $@

clean:
	rm -rf objs

fclean: clean
	rm -rf $(NAME)

re: fclean all