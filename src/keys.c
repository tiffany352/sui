#include "keys.h"

sui_keys *sui_keys_new()
{
	sui_keys *keys;

	keys = malloc(sizeof(sui_keys));
	keys->down_callbacks = NULL;
	keys->up_callbacks = NULL;
	keys->all_down_callbacks = NULL;
	keys->all_up_callbacks = NULL;

	return keys;
}

void sui_keys_down(sui_keys *keys, int key, void(*func)(int))
{
	sui_key_cb *cb;
	node *obj;

	cb = malloc(sizeof(sui_key_cb));
	obj = malloc(sizeof(node));
	cb->func = func;
	cb->key = key;
	obj->val = cb;
	DL_APPEND(keys->down_callbacks, obj);
}

void sui_keys_up(sui_keys *keys, int key, void(*func)(int))
{
	sui_key_cb *cb;
	node *obj;

	cb = malloc(sizeof(sui_key_cb));
	obj = malloc(sizeof(node));
	cb->func = func;
	cb->key = key;
	obj->val = cb;
	DL_APPEND(keys->up_callbacks, obj);
}

void sui_keys_all_down(sui_keys *keys, void(*func)(int))
{
	sui_all_key_cb *cb;
	node *obj;

	cb = malloc(sizeof(sui_key_cb));
	obj = malloc(sizeof(node));
	cb->func = func;
	obj->val = cb;
	DL_APPEND(keys->all_down_callbacks, obj);
}

void sui_keys_all_up(sui_keys *keys, void(*func)(int))
{
	sui_all_key_cb *cb;
	node *obj;

	cb = malloc(sizeof(sui_key_cb));
	obj = malloc(sizeof(node));
	cb->func = func;
	obj->val = cb;
	DL_APPEND(keys->all_up_callbacks, obj);
}

void sui_keys_register_down(sui_keys *keys, int key)
{
	node *obj;

	DL_FOREACH(keys->down_callbacks, obj) {
		sui_key_cb *cb = obj->val;
		if (cb->key == key)
			cb->func(key);
	}

	obj = NULL;

	DL_FOREACH(keys->all_down_callbacks, obj) {
		sui_all_key_cb *cb = obj->val;
		cb->func(key);
	}
}

void sui_keys_register_up(sui_keys *keys, int key)
{
	node *obj;

	DL_FOREACH(keys->up_callbacks, obj) {
		sui_key_cb *cb = obj->val;
		if (cb->key == key)
			cb->func(key);
	}

	obj = NULL;

	DL_FOREACH(keys->all_up_callbacks, obj) {
		sui_all_key_cb *cb = obj->val;
		cb->func(key);
	}
}
