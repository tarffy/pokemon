#pragma once
#include <string>
#include <vector>
using std::string;
using std::vector;
class pokemon_base;
class skill_base {
	string skill_name;
	string skill_decription;
	int skill_used = 0, cd_now;		//当cd_now等于冷却时间时技能可以使用
	int skill_id;	
	vector<int> skill_args;		//持续时间	冷却时间 技能参数
public:
	skill_base() { skill_id = -1; }
	skill_base(const string &name, const string &descrip, int id, const vector<int> &args) 
		:skill_name(name), skill_decription(descrip), skill_id(id),skill_args(args){
		cd_now=skill_args[1];
	}
	int skill_use(pokemon_base *source, pokemon_base *enemy) ;
	
	int get_id() const { return skill_id; }
	vector<int>& get_args() { return skill_args; }
	string get_name() const{ return skill_name; }
	string get_description() const { return skill_decription; }
	inline bool is_skill_cd();
};
/*
class skill_enhance :public skill_base {
	int skill_used = 0,skill_cd,cd_now;
public:
	skill_enhance(const string &name, const string &descrip, int id, const vector<int> &args)
		:skill_base(name, descrip, id, args) {
		skill_cd =cd_now= 4;//要改
	};
	virtual void skill_use(pokemon_base *source, pokemon_base *enemy);

};
class skill_status :public skill_base {

public:
	skill_status(const string &name, const string &descrip, int id, const vector<int> &args)
		:skill_base(name, descrip, id, args) {};
	virtual void skill_use(pokemon_base *source, pokemon_base *enemy);
};
*/