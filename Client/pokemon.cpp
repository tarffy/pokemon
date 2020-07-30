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
string pokemon_base::out_status()
{

	string res;
	string type, _class;
	switch (attributes[6])
	{
	case 0:type = " 水属性"; break;
	case 1:type = " 火属性"; break;
	case 2:type = " 草属性"; break;
	case 3:type = " 普通属性"; break;
	}
	switch (attributes[7])
	{
	case 0:_class = " 肉盾型"; break;
	case 1:_class = " 力量型"; break;
	case 2:_class = " 防御型"; break;
	case 3:_class = " 敏捷型"; break;
	}
	res.append("精灵名:"); res +=(pokemon_name+type+_class);
	res.append("\n");
	res.append("等级:"); res.append(to_string(levels[0]));
	res.append(" 当前经验:"); res.append(to_string(levels[1]));
	res.append("升级需要经验:"); res.append(to_string(levels[2]));
	res.append("\n");
	res.append("HP:"); res.append(to_string(attributes[0]));
	res.append("攻击:"); res.append(to_string(attributes[1]));
	res.append("防御:"); res.append(to_string(attributes[2]));
	res.append("速度:"); res.append(to_string(attributes[3]));
	res.append("暴击率:"); res.append(to_string(attributes[4]));
	res.append("%");
	res.append("闪避率:"); res.append(to_string(attributes[5]));
	res.append("%\n");
	return res;
}
void pokemon_base::gain_exp(int exp) {
	int tem = levels[0];
	while (levels[1] + exp >= levels[2]) {
		if (levels[0] == 15)break;
		++levels[0];
		exp = levels[1] + exp - levels[2];
		levels[1] = 0;
		levels[2] = levels[0] * 200;		//下一级升级经验
		level_up();
	}
	levels[1] += exp;
}
void pokemon_base::level_up() {
	int _class = attributes[7];
	for (int i = 0; i < 6; i++) {
		if (i == _class)attributes[i] += int(attributes[i] * 0.06);
		else attributes[i] += int(attributes[i] * 0.05);
	}
}