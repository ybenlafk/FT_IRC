# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ybenlafk <ybenlafk@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/22 20:41:53 by ybenlafk          #+#    #+#              #
#    Updated: 2023/09/26 09:53:02 by ybenlafk         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME= ircserv
SRC=src/Server.cpp \
	src/Client.cpp \
	main.cpp
OBJ= $(patsubst src/%.cpp,objs/%.o,$(SRC))
CFLAGS= -Wall -Wextra -Werror -std=c++98 -stdlib=libc++

all: $(NAME)

$(NAME): objs $(OBJ)
	c++ $(CFLAGS) $(OBJ) -o $(NAME)

objs:
	mkdir -p objs

objs/%.o: src/%.cpp src/*.hpp | objs
	c++ $(CFLAGS) -c $< -o $@

clean:
	rm -rf objs

fclean: clean
	rm -rf $(NAME)

re: fclean all