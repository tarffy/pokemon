#include "Player.h"
using std::unordered_map;
using std::pair;
using std::to_string;
Player::Player()
{
	pokemon_bag.resize(6);
	pokemon_store.resize(50);
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
	if (store_count == pokemon_store.size())pokemon_store.resize(pokemon_store.size() + 50);
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

void Player::fresh_pokemon_pos(const vector<int>& bag, const vector<int>& store)
{
	unordered_map<int, pair<pokemon_base*, int> > pos;//unique_id ptr pos
	for (int i = 0; i < bag_count; i++) {
		pos[pokemon_bag[i]->get_unique_id()] = pair<pokemon_base*, int>{ pokemon_bag[i],i };
	}
	for (int i = 0; i < store_count; i++) {
		pos[pokemon_store[i]->get_unique_id()] = pair<pokemon_base*, int>{ pokemon_store[i],10+i };
	}
	int delt = 0;
	if (bag[0] != -1) {
		bag_count = bag.size();
		for (int i = 0; i < bag.size(); ++i) {
			auto pok = pos[bag[i]];
			if (pok.second != i) {
				pokemon_bag[i] = pok.first;
				pokemon_bag[i]->need_update[0] = true;
			}
		}
	}
	else {
		bag_count = 0;
	}
	if (store[0] != -1) {
		store_count = store.size();
		if (store_count >= pokemon_store.size())pokemon_store.resize(store_count);
		for (int i = 0; i < store.size(); ++i) {
			auto pok = pos[store[i]];
			if (pok.second != 10 + i) {
				pokemon_store[i] = pok.first;
				pokemon_store[i]->need_update[0] = true;
			}
		}
	}
	else {
		store_count = 0;
	}

}

unordered_map<int, string> Player::get_sql_update_info()
{
	unordered_map<int, string> res;
		for (int i = 0; i < bag_count; i++) {
			pokemon_base *pok = pokemon_bag[i];
			string tem("");
			for (int j = 0; j < pok->need_update.size(); ++j)
				if (pok->need_update[j]) {
					switch (j)
					{
					case 0:tem.append(" bag_store=");
						tem.append(to_string(i));
						tem.append(" ");
						pok->need_update[j] = false;
					}
			}
			if (tem != "")res[pok->get_unique_id()] = tem;
		}
		for (int i = 0; i < store_count; i++) {
			pokemon_base *pok = pokemon_store[i];
			string tem("");
			for (int j = 0; j < pok->need_update.size(); ++j)
				if (pok->need_update[j]) {
					switch (j)
					{
					case 0:tem.append(" bag_store=");
						tem.append(to_string(i+10));
						tem.append(" ");
						pok->need_update[j] = false;
					}
				}
			if (tem != "")res[pok->get_unique_id()] = tem;
		}
		return res;
}

string Player::battle_test()
{
	return pokemon_bag[0]->battle_with(pokemon_store[0]);
}







