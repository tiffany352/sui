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


.old README.md .new:
	ctags --c-kinds=p -f- $(ELEM_HEADERS) $(HEADER_FILES) | \
		grep -v "^!" | \
		sort | \
		cut -f 3-3 | \
		awk '{gsub(/\/\^/,""); gsub(/\$$\/;"/,""); print "### " $$0;}' \
		> $@

doc: .old .new README.md
	-diff3 -m .new .old README.md > doc2.md
	mv doc2.md README.md
	mv .new .old

.PHONY: all glfw sdl doc
