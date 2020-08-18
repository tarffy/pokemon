#pragma once
#pragma execution_character_set("utf-8")
#include <string>
#include <algorithm>
#include <unordered_map>
#include <stdlib.h>
#include <time.h>
#include <vector>
using std::pair;
using std::unordered_map;
using std::string;
using std::vector;
class pokemon_base {
	string pokemon_name;
	int skill_num;
	int unique_id, pokemon_id;
	vector<int> levels;				//level exp_now exp_update 
	vector<int> attributes;			//HP attack defence speed critical miss type水火草普通 class肉盾力量防御敏捷
	vector<int> attributes_z;		//战斗时的status
	void level_up();		//升级 属性增长 可能会调用involve
	
public:
	pokemon_base() { unique_id = -1; }
	pokemon_base(const string &SQL_string) {};
	pokemon_base(const string &name, const vector<int> &id,const vector<int> &attr, const vector<int> &level)
		:pokemon_name(name), attributes(attr), levels(level) {
		srand((unsigned int)time(0));
		unique_id = id[0];
		pokemon_id = id[1];
	};
	string out_status();
	void gain_exp(int expa);	//获取经验 升级会调用level_up
	//get and set
	string get_name() { return pokemon_name; }
	string get_name_and_level() { return pokemon_name + "lv."+std::to_string(levels[0]); }
	void set_name(const string &name) { pokemon_name = name; }
	vector<int> get_levels() { return levels; }
	virtual int get_rarity() { return -1; };
	string out_pokemon_info();
	int get_unique_id() { return unique_id; }
	int get_pokemon_id() { return pokemon_id; }
};
class pokemon_r:public pokemon_base
{
	int rarity;
public:
	pokemon_r() :pokemon_base() {};
	pokemon_r(const string &name, const vector<int> &id, const vector<int> &attr, const vector<int> &level)
		:pokemon_base(name, id, attr, level) {
		rarity = 1;
	}
	virtual int get_rarity() final { return rarity; }
};
