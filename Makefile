ifeq ($(HOSTTYPE),)
    HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

CC = gcc
CFLAGS = -O2 -Wall -Wextra -Werror -fPIC
LIBFT = libft
NAME = libft_malloc_$(HOSTTYPE).so
LINK_NAME = libft_malloc.so
SRCS = src/utils.c src/free.c src/malloc.c src/realloc.c

OBJS = $(SRCS:.c=.o)

all: $(NAME) $(LINK_NAME)

$(NAME): $(OBJS)
	@make -C $(LIBFT)
	$(CC) $(CFLAGS) -L./$(LIBFT) -lft -shared -o $@ $^

$(LINK_NAME): $(NAME)
	ln -sf $(NAME) $(LINK_NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME) $(LINK_NAME)

re: fclean all

.PHONY: all clean fclean re
