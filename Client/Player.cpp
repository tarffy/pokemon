﻿#include "Player.h"
using std::to_string;
Player::Player()
{
	pokemon_bag.resize(6);
	pokemon_store.resize(100);
}

void Player::put_pokemon_in_bag(pokemon_base * pok)
{
	if (bag_count < 6)
		pokemon_bag[bag_count++] = pok;
	else
		put_pokemon_in_store(pok);
}

void Player::put_pokemon_in_store(pokemon_base * pok)
{
	pokemon_store[store_count++] = pok;
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
string Player::get_all_names()
{
	string res;
	int flag = 1;
	for (int i = 0; i < bag_count; ++i) {
		if (flag)flag = 0; else res.append("$$");
		res.append(pokemon_bag[i]->get_name_and_level());
		//res.append(to_string(pokemon_bag[i]->get_unique_id()));
		//res.append(to_string(pokemon_bag[i]->get_pokemon_id()));
	}
	if (flag)res.append("-1");
	flag = 1;
	res.append("###");
	for (int i = 0; i < store_count; ++i) {
		if (flag)flag = 0; else res.append("$$");
		res.append(pokemon_store[i]->get_name_and_level());
	}
	if (flag)res.append("-1");
	return res;
}

void Player::move_to_bag(int store_num)
{
	pokemon_bag[bag_count++] = pokemon_store[store_num];
	for (int i = store_num; i < store_count-1; i++) {
		pokemon_store[i] = pokemon_store[i+1];
	}
	--store_count;
}

void Player::move_to_store(int bag_num)
{
	if (store_count >= pokemon_store.size()) {
		pokemon_store.push_back(pokemon_bag[bag_num]);
		++store_count;
	}else
	pokemon_store[store_count++] = pokemon_bag[bag_num];
	for (int i = bag_num; i < bag_count - 1; i++) {
		pokemon_bag[i] = pokemon_bag[i + 1];
	}
	--bag_count;
}

string Player::pokemon_pos()
{
	string res;
	int flag = 1;
	for (int i = 0; i < bag_count; i++) {
		if (flag)flag = 0; else res.append(",");
		res.append(to_string(pokemon_bag[i]->get_unique_id()));
	}
	if (flag)res.append("-1");
	res.append("###");
	flag = 1;
	for (int i = 0; i < store_count; i++) {
		if (flag)flag = 0; else res.append(",");
		res.append(to_string(pokemon_store[i]->get_unique_id()));
	}
	if (flag)res.append("-1");
	return res;
}
