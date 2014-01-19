all:
	clang main.c src/*.c src/widget/*.c $(shell pkg-config --libs --cflags glfw3) $(shell pkg-config --libs --cflags cairo) -g
