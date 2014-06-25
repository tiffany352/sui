#ifndef KEYS_H
#define KEYS_H

#include "internal.h"

typedef struct sui_keys {
	node *down_callbacks;
	node *up_callbacks;
	node *all_down_callbacks;
	node *all_up_callbacks;
} sui_keys;

typedef struct sui_key_cb {
	int key;
	void(*func)(int);
} sui_key_cb;

typedef struct sui_all_key_cb {
	void(*func)(int);
} sui_all_key_cb;

sui_keys *sui_keys_new();
void sui_keys_down(sui_keys *keys, int key, void(*cb)(int));
void sui_keys_up(sui_keys *keys, int key, void(*cb)(int));
void sui_keys_all_down(sui_keys *keys, void(*cb)(int));
void sui_keys_all_up(sui_keys *keys, void(*cb)(int));
void sui_keys_register_down(sui_keys *keys, int key);
void sui_keys_register_up(sui_keys *keys, int key);

#endif
