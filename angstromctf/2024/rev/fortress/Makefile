CC=gcc
CFLAGS=-Wall -Wpedantic -Werror -O2
BUILD_IMAGE=buildpack-deps:bookworm
RUN_IMAGE=debian:bookworm-slim
NAME=fortress

.PHONY: clean run build

$(NAME): $(NAME).c
	docker run --rm -u "$(shell id -u):$(shell id -g)" -v "$(CURDIR):/app" -w /app $(BUILD_IMAGE) $(CC) $^ $(CFLAGS) -o $@
	strip $@

build: $(NAME)

clean:
	rm $(NAME)

run: build
	docker run --rm -i -u "$(shell id -u):$(shell id -g)" -v "$(CURDIR):/app" -w /app $(RUN_IMAGE) ./$(NAME)
