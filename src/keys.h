#ifndef KEYS_H
#define KEYS_H

struct sui_state;
struct sui_keys {
	void(*down)(char);
	void(*up)(char);
};

struct sui_keys *sui_keys_init();
struct sui_state *sui_key_down(struct sui_state *state, void(*down)(char));
struct sui_state *sui_key_up(struct sui_state *state, void(*up)(char));

#endif
