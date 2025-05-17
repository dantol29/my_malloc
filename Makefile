ifeq ($(HOSTTYPE),)
    HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

CC = gcc
CFLAGS = -O2 -Wall -Wextra -Werror -fPIC
LIBPRINTF = libprintf
NAME = libft_malloc_$(HOSTTYPE).so
LINK_NAME = libft_malloc.so
SRCS = src/utils.c src/free.c src/malloc.c src/realloc.c src/print.c

OBJS = $(SRCS:.c=.o)

all: $(NAME) $(LINK_NAME)

$(NAME): $(OBJS)
	@make -C $(LIBPRINTF)
	$(CC) $(CFLAGS) -shared -o $@ $^ -L./$(LIBPRINTF) -lftprintf -Wl,-rpath,'$$ORIGIN/$(LIBPRINTF)'

$(LINK_NAME): $(NAME)
	ln -sf $(NAME) $(LINK_NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME) $(LINK_NAME)
	rm -rf $(LIBPRINTF)/*.o
	rm -rf $(LIBPRINTF)/*.a

re: fclean all

.PHONY: all clean fclean re
