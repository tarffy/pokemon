#pragma once
#include "skill.h"
#include <algorithm>
#include <unordered_map>
#include <stdlib.h>
#include <time.h>
using std::pair;
using std::unordered_map;
class pokemon_base {
	string pokemon_name;
	int skill_num;
	int unique_id, pokemon_id;
	vector<int> levels;				//level exp_now exp_update 
	vector<int> attributes;			//HP attack defence speed critical miss type水火草普通 class肉盾力量防御敏捷
	vector<int> attributes_z;		//战斗时的status
	unordered_map<int, vector<int>> status;	//int id 0id 1剩余持续时间 23技能效果
	vector<skill_base*> skills;
	string repo;
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
	void level_up();		//升级 属性增长 可能会调用involve


	virtual string use_skills(pokemon_base *enemy) { return ""; }

	virtual void set_skill(int cur, int id, string name, string descrip, vector<int> skill_args);
	//get and set
	string get_name() { return pokemon_name; }
	string get_name_and_level() { return pokemon_name + "lv."+std::to_string(levels[0]); }
	string &get_repo() { return repo; }
	void set_name(const string &name) { pokemon_name = name; }
	vector<int>& get_attribute() { return attributes; }
	vector<int>& get_attribute_z()  { return attributes_z; }
	void set_attributes_z(const vector<int> &atti) { attributes_z = atti; }
	unordered_map<int, vector<int>> & get_status() { return status; }
	void set_status(const unordered_map<int, vector<int>>&stat) { status = stat; }
	vector<skill_base*>& get_skills() { return skills; }
	void set_skill_num(int i) { skill_num = i; skills.resize(i); };
	vector<int> get_levels() { return levels; }
	virtual int get_rarity() { return -1; };
	string out_pokemon_info();
	int get_unique_id() { return unique_id; }
	void set_unique_id(int num) { unique_id = num; }
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
		this->set_skill_num(2);
	}
	virtual int get_rarity() final { return rarity; }
	virtual string use_skills(pokemon_base *enemy) final;
};
