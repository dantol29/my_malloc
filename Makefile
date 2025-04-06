CC = gcc
CFLAGS = -Wall -Wextra -Werror -fPIC
NAME = libft_malloc.so
SRCS = utils.c main.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
        $(CC) -shared -o $@ $^

clean:
        rm -rf $(OBJS) $(BONUS_OBJS)

fclean: clean
        rm -Rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
