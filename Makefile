# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/22 20:41:53 by ybenlafk          #+#    #+#              #
#    Updated: 2023/10/21 17:39:06 by ybenlafk         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME= ircserv
BNAME= ircbot
SRC=mandatory/src/Server.cpp \
	mandatory/src/Client.cpp \
	mandatory/src/utils.cpp \
	mandatory/src/commands/PART.cpp \
	mandatory/src/commands/INVITE.cpp \
	mandatory/src/commands/JOIN.cpp \
	mandatory/src/commands/KICK.cpp \
	mandatory/src/commands/MODE.cpp \
	mandatory/src/commands/NICK.cpp \
	mandatory/src/commands/PRIVMSG.cpp \
	mandatory/src/commands/QUIT.cpp \
	mandatory/src/commands/TOPIC.cpp \
	mandatory/src/Channel.cpp \
	mandatory/main.cpp
BSRC=bonus/main.cpp

Headers=mandatory/src/Channel.hpp\
		mandatory/src/Client.hpp\
		mandatory/src/Cmds.hpp\
		mandatory/src/Server.hpp\
		mandatory/src/defines.hpp\
		mandatory/src/headers.hpp\
		mandatory/src/utils.hpp

OBJ= $(SRC:.cpp=.o)

BOBJ= $(BSRC:.cpp=.o)

CFLAGS= -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g

all: $(NAME)

bonus: $(BNAME)

$(NAME): $(OBJ)
	c++ $(CFLAGS) $(OBJ) -o $(NAME)

$(BNAME): $(BOBJ)
	c++ $(CFLAGS) $(BOBJ) -o $(BNAME)

%.o: %.cpp $(Headers)
	c++ $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ) $(BOBJ)

fclean: clean
	rm -rf $(NAME) $(BNAME)

re: fclean all
