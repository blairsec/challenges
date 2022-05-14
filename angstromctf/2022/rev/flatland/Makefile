CC=clang
CFLAGS=-Wall -Wpedantic -Werror -O2

NAME=flatland

.PHONY: clean run build

$(NAME): $(NAME).c
	$(CC) $(NAME).c -I. $(CFLAGS) -o $(NAME)
	strip $(NAME)

build: $(NAME)

clean:
	rm $(NAME)

run: build
	./$(NAME)
