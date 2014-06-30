CC = gcc
CFLAGS = -Wall -Werror -I/usr/local/include
BUILD = build
GLFW_EG = glfw_example
SDL_EG = sdl_example
EXAMPLES = $(GLFW_EG) $(SDL_EG)
LIBRARY = $(BUILD)/libsui.a
SOURCES = $(wildcard src/*.c) $(wildcard src/*/*.c)
HEADERS = $(wildcard src/*.h) $(wildcard src/*/*.h)
OBJECTS = $(patsubst src/%.c,$(BUILD)/%.o,$(SOURCES))

all: lib glfw sdl

lib: $(LIBRARY)

$(LIBRARY): $(OBJECTS)
	ar crl $(LIBRARY) $(OBJECTS)
	ranlib $(LIBRARY)

$(BUILD)/%.o: src/%.c $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD):
	mkdir -p $(BUILD)
	mkdir -p $(BUILD)/elem

clean:
	rm -rf $(EXAMPLES)
	rm -rf $(BUILD)

glfw:
	$(CC) examples/glfw/main.c -o glfw_example \
		$(shell pkg-config --libs --cflags glfw3) \
		$(shell pkg-config --libs --cflags glew) \
		$(shell pkg-config --libs --cflags cairo) \
		-L$(BUILD) -lsui

sdl:
	$(CC) examples/sdl/main.c  -o sdl_example \
		-I/usr/local/include \
		$(shell pkg-config --libs --cflags sdl2) \
		$(shell pkg-config --libs --cflags glew) \
		$(shell pkg-config --libs --cflags cairo) \
		-L$(BUILD) -lsui

.PHONY: all lib clean glfw sdl
