SRC_FILES = src/*.c
HEADER_FILES = src/*.h
ELEM_FILES = src/elem/*.c
ELEM_HEADERS = src/elem/*.h

all: glfw sdl

glfw:
	clang examples/glfw/main.c -o glfw_example $(SRC_FILES) $(ELEM_FILES) \
		-I/usr/local/include \
		$(shell pkg-config --libs --cflags glfw3) \
		$(shell pkg-config --libs --cflags glew) \
		$(shell pkg-config --libs --cflags cairo) -g

sdl:
	clang examples/sdl/main.c  -o sdl_example $(SRC_FILES) $(ELEM_FILES) \
		-I/usr/local/include \
		$(shell pkg-config --libs --cflags sdl2) \
		$(shell pkg-config --libs --cflags glew) \
		$(shell pkg-config --libs --cflags cairo) -g

clean:
	rm -r *_example*

.PHONY: all glfw sdl clean
