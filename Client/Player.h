#pragma once
#include "pokemon.h"

class Player {
private:
	string user_name;
	vector<pokemon_base *> pokemon_bag;	//放在背包中的精灵
	vector<pokemon_base *> pokemon_store;//放在仓库中的精灵 精灵只存在于一处
	int bag_count = 0;
	int store_count = 0;
	string out_pokemon_info();
public:
	
	Player();
	
	void set_user_name(const string &str) { user_name = str; }
	void put_pokemon_in_bag(pokemon_base* pok);
	void put_pokemon_in_store(pokemon_base* pok);
	string get_all_names();		//返回所有精灵的名字 bag和store以###隔开 精灵名以$$隔开
	void move_to_bag(int store_num);
	void move_to_store(int bag_num);
	string pokemon_pos();	//返回背包和仓库各位置的精灵unique id
	pokemon_base *find_pok_by_unique(int id);
	int get_unique_by_pos(int pos);
	int delet_pok(int unique);
};