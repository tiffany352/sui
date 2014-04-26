SRC_FILES = src/*.c
WIDGET_FILES = src/widget/*.c

all:
	clang main.c $(SRC_FILES) $(WIDGET_FILES) $(shell pkg-config --libs --cflags glfw3) $(shell pkg-config --libs --cflags cairo) -g

.old README.md .new:
	ctags -f- $(SRC_FILES) | grep -v "^!" | sort | cut -f 3-3 | awk '{gsub(/\/\^/,""); gsub(/\$$\/;"/,""); print "### " $$0;}' > $@

doc: .old .new README.md
	-diff3 -m .new .old README.md > doc2.md
	mv doc2.md README.md
	mv .new .old

.PHONY: all doc
