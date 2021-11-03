# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lucaslefrancq <lucaslefrancq@student.42    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/04/27 14:27:30 by hherin            #+#    #+#              #
#    Updated: 2021/06/21 17:45:18 by lucaslefran      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	lutils.a

SRCS		=	utils.cpp autoindex.cpp

OBJS		=	${SRCS:.cpp=.o}

HDRS		=	utils.hpp

FLAGS		=	-Wall -Werror -Wextra

all			:	${NAME}

%.o			:	%.cpp
				@${CC} -DDEBUG=${debug} ${FLAGS} -o $@ -c $<

${NAME}		:	${OBJS} 
				@ar rcs ${NAME} ${OBJS}
				@ranlib ${NAME}

${OBJS}			:	${HDRS}

clean			:
					@rm -rf *.o ../*.o

fclean			:	clean
					@rm -rf ${NAME}

re				:	fclean all

.PHONY			:	all clean fclean re