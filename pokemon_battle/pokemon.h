#pragma once
#include "skill.h"
#include <algorithm>
#include <unordered_map>
#include <stdlib.h>
using std::pair;
using std::unordered_map;
class pokemon_base {
	string pokemon_name;
	int skill_num;
	vector<int> levels;				//level exp_now exp_update
	int need_update_sql,sql_id;
	vector<int> attributes;			//HP attack defence speed critical miss
	vector<int> attributes_z;		//战斗时的status
	unordered_map<int, vector<int>> status;	//int id 0id 1剩余持续时间 23技能效果
	vector<skill_base*> skills;
public:
	pokemon_base() { sql_id = -1; }
	pokemon_base(const string &SQL_string) {};
	pokemon_base(const string &name,const vector<int> &attr, const vector<int> &level) 
		:pokemon_name(name), attributes(attr), levels(level) {
		srand((int)&name);
	};
	string get_name() { return pokemon_name; }
	void set_name(const string &name) { pokemon_name = name; }
	vector<int>& get_attribute() { return attributes; }
	vector<int>& get_attribute_z()  { return attributes_z; }
	void set_attributes_z(const vector<int> &atti) { attributes_z = atti; }
	unordered_map<int, vector<int>> & get_status() { return status; }
	void set_status(const unordered_map<int, vector<int>>&stat) { status = stat; }
	vector<skill_base*>& get_skills() { return skills; }
	void set_skill_num(int i) { skill_num = i; skills.resize(i); };
	vector<int> get_levels() { return levels; }
	void status_fresh();
	virtual int get_rarity() { return -1; };
	virtual void use_skills(pokemon_base *enemy) {};
	inline bool get_true_at_rate(double rate);
	inline bool get_true_at_rate(int rate100);
	virtual void battle_with(pokemon_base *enemy) { };
	void out_status() ;
	void out_z_status();
	virtual void set_skill(int cur, int id, string name, string descrip, vector<int> skill_args) ;
};
class pokemon_r:public pokemon_base
{
	int rarity;
public:
	pokemon_r() :pokemon_base() {};
	pokemon_r(const string &name, const vector<int> &attr, const vector<int> &level)
		:pokemon_base(name,attr, level) {
		set_attributes_z(attr);
		rarity = 1;
		this->set_skill_num(2);
	}
	virtual int get_rarity() final { return rarity; }
	virtual void use_skills(pokemon_base *enemy) final;
	virtual void battle_with(pokemon_base *enemy) final;
};
