#include "keys.h"

sui_keys *sui_keys_new(GLFWwindow *window)
{
	sui_keys *keys;

	keys = malloc(sizeof(sui_keys));
	keys->callbacks = NULL;
	keys->window = window;

	return keys;
}

void sui_keys_register(sui_keys *keys, int key, void(*func)(int))
{
	sui_key_cb *cb;
	node *obj;

	cb = malloc(sizeof(sui_key_cb));
	obj = malloc(sizeof(node));
	cb->func = func;
	cb->key = key;
	obj->val = cb;
	DL_APPEND(keys->callbacks, obj);
}

void sui_keys_update(sui_keys *keys)
{
	node *obj;

	DL_FOREACH(keys->callbacks, obj) {
		sui_key_cb *cb = obj->val;
		if (glfwGetKey(keys->window, cb->key) == GLFW_PRESS) {
			cb->func(glfwGetKey(keys->window, cb->key));
		}
	}
}
