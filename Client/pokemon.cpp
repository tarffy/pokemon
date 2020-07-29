#include "pokemon.h"
#include <iostream>
using std::cout;
using std::endl;
using std::to_string;
string pokemon_r::use_skills(pokemon_base * enemy)
{
	auto &skills = this->get_skills();
	string skill_used;
	//skills[0]->skill_use(this,enemy);
	int flag = 1;
	for (auto &skill : skills) {
		if (skill) { 
			int used=skill->skill_use(this, enemy); 
			if (used) {
				if (flag)flag = 0; else skill_used.append(",");
				skill_used.append(to_string(skill->get_id()));
			}
		}
	}
	if (flag)skill_used.append(to_string(-1));
	skill_used.append("&&");
	return skill_used;
}



void pokemon_base::set_skill(int cur, int id, string name, string descrip, vector<int> skill_args)
{
	skill_base *skill_set = new skill_base(name,descrip , id, skill_args);
	this->get_skills()[cur] = skill_set;
}

string pokemon_base::out_pokemon_info()
{
	string dot = ",";
	string res = pokemon_name + dot + to_string(this->get_rarity())+dot;
	for (int i = 0; i < 8; i++)res.append(to_string(attributes[i] )+dot);
	for (int i = 0; i < 3; i++)res.append(to_string(levels[i]) + dot);
	
	return res;
}
