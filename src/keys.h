#ifndef KEYS_H
#define KEYS_H

#include "internal.h"

typedef struct sui_keys {
	node *callbacks;
	int num_callbacks;
	GLFWwindow *window;
} sui_keys;

typedef struct sui_key_cb {
	int key;
	void(*func)(int);
} sui_key_cb;

sui_keys *sui_keys_new(GLFWwindow *window);
void sui_keys_register(sui_keys *keys, int key, void(*cb)(int));
void sui_keys_update(sui_keys *keys);

#endif
