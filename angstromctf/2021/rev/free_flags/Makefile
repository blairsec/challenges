CC=clang
CFLAGS=-Wall -Wpedantic -Werror

NAME=free_flags

.PHONY: clean run build

$(NAME): $(NAME).c
	$(CC) $^ $(CFLAGS) -o $@
	# strip $@

build: $(NAME)

clean:
	rm -f $(NAME)

run: build
	./$(NAME)
