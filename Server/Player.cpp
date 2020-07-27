#include "Player.h"

Player::Player()
{
	pokemon_bag.resize(6);
}

void Player::put_pokemon_in_bag(pokemon_base * pok)
{
	pokemon_bag[bag_count++] = pok;
}
void Player::put_pokemon_in_store(pokemon_base * pok)
{
	pokemon_bag[store_count++] = pok;
}
string Player::out_pokemon_info()
{
	string res;
	int flag = 1;
	for (int i = 0; i < bag_count; i++) {
		if (flag)flag = 0; else res.append("$$");
		res.append(pokemon_bag[i]->out_pokemon_info());
	}
	if (bag_count == 0)res.append("-1");
	res.append("###");
	flag = 1;
	for (int i = 0; i < store_count; i++) {
		if (flag)flag = 0; else res.append("$$");
		res.append(pokemon_store[i]->out_pokemon_info());
	}
	if (store_count == 0)res.append("-1");
	return res;
}


