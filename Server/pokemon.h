#pragma once
#include "skill.h"
#include <algorithm>
#include <unordered_map>
#include <stdlib.h>
#include <time.h>
using std::pair;
using std::unordered_map;
class pokemon_base {
private:
	string pokemon_name;
	int skill_num;
	int unique_id,pokemon_id;
	vector<int> levels;				//level exp_now exp_update 
	vector<int> attributes;			//HP attack defence speed critical miss type水火草普通 class肉盾力量防御敏捷
	vector<int> attributes_z;		//战斗时的status
	unordered_map<int, vector<int>> status;	//int id 0id 1剩余持续时间 23技能效果
	vector<skill_base*> skills;		//技能
	string repo;					//一个精灵战斗时用于保存战斗过程信息
	string status_fresh();		//战斗录像 详细信息见文档
	inline bool get_true_at_rate(double rate);
	inline bool get_true_at_rate(int rate100);
	inline double get_rand() { return rand() % 1001 * 1.0 / 1000; }
	string attack_turn(pokemon_base *enemy);//战斗双方其中一方的回合	
	void atti_reset() { attributes_z = attributes; status.clear(); }//战斗前将状态重置
	string get_exp(int exp);	//获取经验 升级会调用level_up
	virtual string use_skills(pokemon_base *enemy) { return ""; }
	void skill_cd_reset();		//战斗开始时重置所有技能冷却时间
public:
	vector<bool> need_update;	//0位置 1经验等级 234技能
	pokemon_base() { unique_id = -1; }
	pokemon_base(const string &name,const vector<int> &id,const vector<int> &attr, const vector<int> &level) 
		:pokemon_name(name), attributes(attr), levels(level) {
		srand((unsigned int)time(0));
		unique_id = id[0];
		pokemon_id = id[1];
		need_update.resize(5);//0位置 
		for (int i = 0; i < need_update.size(); ++i)need_update[i] = false;
	};
	string out_pokemon_info();	//将精灵信息输出成字符串用于socket通信
	string battle_with(pokemon_base *enemy);	//开始一场酣畅淋漓的战斗
	void level_up(int num = 1);		//升级 属性增长
	//get and set
	virtual void set_skill(int cur, int id, string name, string descrip, vector<int> skill_args);	//将第几个技能设置为后面的信息
	string get_name() { return pokemon_name; }
	string &get_repo() { return repo; }
	void set_name(const string &name) { pokemon_name = name; }
	vector<int>& get_attribute() { return attributes; }
	vector<int>& get_attribute_z()  { return attributes_z; }
	void set_attributes_z(const vector<int> &atti) { attributes_z = atti; }
	unordered_map<int, vector<int>> & get_status() { return status; }
	void set_status(const unordered_map<int, vector<int>>&stat) { status = stat; }
	vector<skill_base*>& get_skills() { return skills; }
	void set_skill_num(int i) { skill_num = i; skills.resize(i); };
	vector<int>& get_levels() { return levels; }
	virtual int get_rarity() { return -1; };
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
		:pokemon_base(name,id,attr, level) {
		rarity = 1;
		this->set_skill_num(4);
	}
	virtual int get_rarity() final { return rarity; }
	virtual string use_skills(pokemon_base *enemy) final;
};
