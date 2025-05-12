NAME	=	philo

CC	=	cc

CFLAGS	=	-Werror -Wall -Wextra -g

PTHREADS_FLAGS	=	-pthread

SRCS	=	helpers/helpers.c			\
			helpers/helpers_2.c			\
			helpers/routine_helpers_2.c	\
			helpers/routine_helpers.c	\
			src/routine.c				\
			src/initialisation.c		\
			src/main.c

OBJS	=	$(SRCS:.c=.o)

all	:	$(NAME)

$(NAME)	:	$(OBJS)	$(HEADER)
			$(CC)	$(CFLAGS)	$(OBJS)	-o	$(NAME)	$(PTHREADS_FLAGS)


clean	:
			rm	-rf	$(OBJS)

fclean	:	clean
			rm	$(NAME)

re	:	fclean all

.PHONY:	all clean fclean re