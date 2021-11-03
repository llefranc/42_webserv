# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lucaslefrancq <lucaslefrancq@student.42    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/04/27 14:27:30 by hherin            #+#    #+#              #
#    Updated: 2021/06/21 17:44:56 by lucaslefran      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	lparser.a

SRCS		=	ServerInfo.cpp \
				FileParser.cpp \
				LocationObj.cpp \
				parserUtils.cpp

OBJS		=	${SRCS:.cpp=.o}

HDRS		=	FileParser.hpp \
				ServerInfo.hpp \
				LocationObj.hpp \
				parser.hpp

FLAGS		=	-Wall -Werror -Wextra

all			:	${NAME}

%.o			:	%.cpp
				@${CC} ${FLAGS} -o $@ -c $<

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