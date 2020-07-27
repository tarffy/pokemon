#pragma once
#include "pokemon.h"
class Player {
	string user_name;
	vector<pokemon_base *> pokemon_bag;	//放在背包中的精灵
	vector<pokemon_base *> pokemon_store;//放在仓库中的精灵 精灵只存在于一处
	int bag_count = 0;
	int store_count = 0;
public:
	Player();
	void set_user_name(const string &str) { user_name = str; }
	void put_pokemon_in_bag(pokemon_base* pok);
	void put_pokemon_in_store(pokemon_base* pok);
	string out_pokemon_info();		//输出所有精灵的信息 bag和store以###隔开 精灵间以$$隔开 无精灵为-1 信息包括名字稀有度 八个属性 3个level

};