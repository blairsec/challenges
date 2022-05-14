CC=gcc
CFLAGS=-Wall -Wpedantic -Werror -g3 -Og -fsanitize=address

NAME=lvm

.PHONY: clean run build

$(NAME): $(NAME).c
	$(CC) $(NAME).c -I. $(CFLAGS) -o $(NAME)
	# strip $(NAME)

build: $(NAME)

clean:
	rm $(NAME)

run: build
	./$(NAME)
