CC=gcc
CFLAGS=-Wall -Wpedantic -Werror -O2

NAME=uninspired

.PHONY: clean run build

$(NAME): $(NAME).c
	$(CC) $(NAME).c -I. $(CFLAGS) -o $(NAME)

build: $(NAME)

clean:
	rm $(NAME)

run: build
	./$(NAME)
