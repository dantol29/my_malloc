ifeq ($(HOSTTYPE),)
    HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

CC = gcc
CFLAGS = -g -O2 -Wall -Wextra -Werror -fPIC
NAME = libft_malloc_$(HOSTTYPE).so
LINK_NAME = libft_malloc.so
SRCS = src/utils.c src/free.c src/malloc.c src/realloc.c src/print.c src/free_list.c src/zones.c

OBJS = $(SRCS:.c=.o)

all: $(NAME) $(LINK_NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -shared -o $@ $^ -Wl,-rpath

$(LINK_NAME): $(NAME)
	ln -sf $(NAME) $(LINK_NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME) $(LINK_NAME)

re: fclean all

.PHONY: all clean fclean re
