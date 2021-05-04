CC=gcc
CFLAGS=-O2
NAME=infinity_gauntlet

.PHONY: build run

$(NAME): $(NAME).c
	$(CC) -o $@ $^ $(CFLAGS)

build: $(NAME)

run: build
	./$(NAME)
