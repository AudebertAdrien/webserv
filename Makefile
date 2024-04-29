# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: motoko <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/02 14:26:47 by motoko            #+#    #+#              #
#    Updated: 2024/04/25 14:08:30 by motoko           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			:= ./webserv
SRCS_DIR		:= srcs
OBJS_DIR		:= objs
INCLUDES_DIR	:= includes

VPATH			:= $(SRCS_DIR):

SRCS			:= main.cpp				\
				   libft.cpp			\
				   server_manager.cpp

CC				:= c++
CFLAGS			:= -Wall -Wextra

#Colors:
GREEN		=	\e[92m
YELLOW		=	\e[93m
RESET		=	\e[0m

#Debug 
ifeq ($(DEBUG), 1)
   CLFAGS += -ggdb3 
endif

#C standar version
ifdef STD
CFLAGS			+= -std=$(STD)
else
CFLAGS			+= -std=c++98
endif

RM				:= rm -rf

OBJS			:= $(addprefix $(OBJS_DIR)/, $(SRCS:.cpp=.o))

all:	$(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) -I$(INCLUDES_DIR) $(OBJS) -o $@	
	@printf "$(GREEN)- Executable ready.\n$(RESET)"

$(OBJS_DIR)/%.o: %.cpp | $(OBJS_DIR)
	@$(CC) $(CFLAGS) -I$(INCLUDES_DIR) -c $< -o $@

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

clean:
	@$(RM) $(OBJS)
	@$(RM) $(OBJS_DIR)
	@printf "$(YELLOW)- Object files removed.\n$(RESET)"

fclean: clean
	@$(RM) $(NAME)
	@printf "$(YELLOW)- Executable removed.\n$(RESET)"

re: fclean all

.PHONY:	all clean fclean re
