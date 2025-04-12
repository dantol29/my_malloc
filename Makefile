CC = gcc
CFLAGS = -O2 -Wall -Wextra -Werror -fPIC
NAME = libft_malloc.so
SRCS = src/utils.c src/free.c src/malloc.c src/realloc.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -shared -o $@ $^

clean:
	rm -rf $(OBJS) $(BONUS_OBJS)

fclean: clean
	rm -Rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
