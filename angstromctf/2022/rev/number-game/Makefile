CC=gcc
CFLAGS=-Wall -Wpedantic -Werror -O0

NAME=number-game

.PHONY: clean run build

$(NAME): $(NAME).c
	$(CC) $(NAME).c -I. $(CFLAGS) -o $(NAME)

build: $(NAME)

clean:
	rm $(NAME)

run: build
	./$(NAME)
