CC = gcc
CFLAGS = -Wall -Werror -fPIC
GLFLAGS = -lGL -lGLU
BUILD = build
GLFW_EG = $(BUILD)/glfw_example
SDL_EG = $(BUILD)/sdl_example
LIBRARY = $(BUILD)/libsui.so
SOURCES = $(wildcard src/*.c) $(wildcard src/*/*.c)
HEADERS = $(wildcard src/*.h) $(wildcard src/*/*.h)
OBJECTS = $(patsubst src/%.c,$(BUILD)/%.o,$(SOURCES))

# OpenGL
ifeq ($(OS),Windows_NT)
	GLFLAGS = -lopengl32 -lglu32
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Darwin)
		GLFLAGS = -framework Carbon -framework OpenGL
	endif
endif

all: lib glfw sdl

lib: $(LIBRARY)

$(LIBRARY): $(OBJECTS)
	$(CC) -shared -o $(LIBRARY) $(OBJECTS) \
		$(GLFLAGS) \
		$(shell pkg-config --libs cairo)

$(BUILD)/%.o: src/%.c $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@ \
		$(shell pkg-config --cflags cairo)

$(BUILD):
	mkdir -p $(BUILD)
	mkdir -p $(BUILD)/elem

clean:
	rm -rf $(BUILD)

glfw:
	$(CC) examples/glfw/main.c -o $(GLFW_EG) \
		$(shell pkg-config --cflags --libs glfw3) \
		$(shell pkg-config --libs cairo) \
		$(GLFLAGS) \
		 -L$(BUILD) -lsui

sdl:
	$(CC) examples/sdl/main.c -o $(SDL_EG) \
		$(shell pkg-config --libs --cflags sdl2) \
		$(shell pkg-config --cflags cairo) \
		$(GLFLAGS) \
		-L$(BUILD) -lsui

.PHONY: all lib clean glfw sdl
