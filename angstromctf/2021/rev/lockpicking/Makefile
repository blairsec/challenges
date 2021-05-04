CC=gcc
CFLAGS=-Wall -Wpedantic -Werror -O2

NAME=lockpicking

.PHONY: clean run build

$(NAME): $(NAME).c constants.h
	$(CC) $(NAME).c -I. $(CFLAGS) -o $(NAME)
	strip $(NAME)

build: $(NAME)

clean:
	rm $(NAME)

run: build
	./$(NAME)
