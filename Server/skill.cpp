#include "skill.h"
#include "pokemon.h"
#include <iostream>
using std::cout;
using std::endl;
/*void skill_enhance::skill_use(pokemon_base * source, pokemon_base * enemy)
{
	skill_base *skill = source->get_skills()[0];
	vector<int> args = this->get_args();
	switch (this->get_id())
	{
	case 1: {
		if (skill_cd!=cd_now++) {
			cout << "skill cding\n";
			break;
		}
		cd_now = 0;
		cout << "使用技能" << this->get_name() << " " << this->get_description();
		int attack_increase_rate = args[0];
		int defence_increase_rate = args[1];
		vector<int> atti = source->get_attribute();
		vector<int> atti_z = source->get_attribute_z();

		int attack_inc = int(1.0*attack_increase_rate / 100 * atti[1]);
		int defence_inc = int(1.0*defence_increase_rate / 100 * atti[2]);
		auto &status = source->get_status();
		vector<int> args{ this->get_id(),3,attack_inc,defence_inc };
		status.push_back(args);
		atti_z[1] += attack_inc;
		atti_z[2] += defence_inc;
		source->set_attributes_z(atti_z);
		skill_used = 1;
		break;
	}

	default:
		cout << "No such skill!";
	}
}*/

int skill_base::skill_use(pokemon_base * source, pokemon_base * enemy)
{
	skill_base *skill = source->get_skills()[0];
	vector<int> &skill_args = this->get_args();
	if (is_skill_cd()) {
		//cout << "skill cding\n";
		return 0;
	}
	//cout << "使用技能" << this->get_name() << " " << this->get_description()<<endl;
	vector<int> &source_atti = source->get_attribute();
	vector<int> &source_atti_z = source->get_attribute_z();
	vector<int> &enemy_atti = enemy->get_attribute();
	vector<int> &enemy_atti_z = enemy->get_attribute_z();
	int status_add_flag1 = 0, status_add_flag2 = 0;
	auto &source_status = source->get_status();
	auto &enemy_status = enemy->get_status();
	vector<int> status_args1,status_args2;

	switch (this->get_id())
	{
		/*case 1: {
			int attack_inc = int(1.0*skill_args[2] / 100 * source_atti[1]);
			int defence_inc = int(1.0*skill_args[3] / 100 * source_atti[2]);
			status_args1=vector<int>{ this->get_id(),skill_args[0],attack_inc,defence_inc };
			source_atti_z[1] += attack_inc;
			source_atti_z[2] += defence_inc;
			status_add_flag1 = 1;
			break;
		}*/

	case 2: {//暴击
		int baoji = 100;
		status_args1 = vector<int>{ this->get_id(),skill_args[0], source_atti_z[4] };
		source_atti_z[4] = 100;
		status_add_flag1 = 1;
		break;
	}
	case 3: {//强化
		int attack_inc = int(1.0*skill_args[2] / 100 * source_atti[1]);
		int defence_inc = int(1.0*skill_args[3] / 100 * source_atti[3]);
		source_atti_z[1] += attack_inc;
		source_atti_z[2] += defence_inc;
		status_args1 = vector<int>{ this->get_id(),skill_args[0],attack_inc,defence_inc };
		status_add_flag1 = 1;
		break;
	}
	case 4: {//持续debuff
		int defence_descease = int(1.0*skill_args[3] / 100 * enemy_atti[2]);
		int demage = int(1.0*enemy_atti[0] * skill_args[2]/4);
		enemy_atti_z[2] -= defence_descease;
		status_args2 = vector<int>{ this->get_id(),skill_args[0],demage,defence_descease };
		status_add_flag2 = 1;
		break;
	}
	case 5:
	{

		int defence_inc = int(1.0*skill_args[2] / 100 * source_atti[2]);
		int miss_inc = int(1.0*skill_args[3] / 100 * source_atti[5]);
		source_atti_z[2] += defence_inc;
		source_atti_z[5] += miss_inc;
		status_args1 = vector<int>{ this->get_id(),skill_args[0],defence_inc ,miss_inc };
		status_add_flag1 = 1;
		break;
	}
	case 10: {
		int attack_inc = int(1.0*skill_args[2] / 100 * source_atti[1]);
		int speed = source_atti[3];
		source_atti_z[1] += attack_inc;
		source_atti_z[3] = 1000;
		status_args1 = vector<int>{ this->get_id(),skill_args[0],attack_inc ,speed };
		status_add_flag1 = 1;
		break;
	}
	case 11: {//持续debuff
		int attack_descease = int(1.0*skill_args[3] / 100 * enemy_atti[1]);
		int demage = int(1.0*enemy_atti[0] * skill_args[2]/100);
		enemy_atti_z[1] -= attack_descease;
		status_args2 = vector<int>{ this->get_id(),skill_args[0],demage,attack_descease };
		status_add_flag2 = 1;
		break;
	}
	case 12: {
		int miss_inc = skill_args[2];
		source_atti_z[5] += miss_inc;
		status_args1 = vector<int>{ this->get_id(),skill_args[0],miss_inc };
		status_add_flag1 = 1;
		break;
	}
	case 13: {
		int recover = int(1.0*skill_args[2] * source_atti[0] / 100);
		status_args1 = vector<int>{ this->get_id(),skill_args[0],recover };
		status_add_flag1 = 1;
		break;
	}
	case 20: {
		int recover = int(1.0*skill_args[2] * source_atti[0] / 100);
		source_atti_z[0] += recover;
		int defence_inc = int(1.0*skill_args[3] / 100 * source_atti[2]);
		source_atti_z[2] += defence_inc;
		status_args1 = vector<int>{ this->get_id(),skill_args[0],defence_inc };
		status_add_flag1 = 1;
		break;
	}
	case 21: {//持续debuff
		int speed_descease = int(1.0*skill_args[3] / 100 * enemy_atti[3]);
		int demage = int(1.0*enemy_atti[0] * skill_args[2]/100);
		enemy_atti_z[3] -= speed_descease;
		status_args2 = vector<int>{ this->get_id(),skill_args[0],demage,speed_descease };
		status_add_flag2 = 1;
		break;
	}
	case 22: {//持续debuff
		
		int demage = int(1.0*enemy_atti[0] * skill_args[2]/100);
		status_args1 = vector<int>{ this->get_id(),skill_args[0],demage };
		status_add_flag1 = 1;
		status_args2 = vector<int>{ this->get_id(),skill_args[0],-demage};
		status_add_flag2 = 1;
		break;
	}
	case 23:{
		int attack_inc = int(1.0*skill_args[2] / 100 * source_atti[1]);
		source_atti_z[1] += attack_inc;
		status_args1 = vector<int>{ this->get_id(),skill_args[0],attack_inc };
		status_add_flag1 = 1;
		break;
			 }
	case 30: {
		int attack_inc = int(1.0*skill_args[2] / 100 * source_atti[1]);
		int defence_inc = int(1.0*skill_args[3] / 100 * source_atti[2]);
		source_atti_z[1] += attack_inc;
		source_atti_z[2] += defence_inc;
		break;
	}
	case 31: {
		int defence_descease = int(1.0*skill_args[2] / 100 * enemy_atti[2]);
		enemy_atti_z[2] -= defence_descease;
		status_args2 = vector<int>{ this->get_id(),skill_args[0],defence_descease };
		status_add_flag2 = 1;
		break;
	}
	case 32: {
		int attack_inc = int(1.0*skill_args[2] / 100 * source_atti[1]);
		source_atti_z[1] += attack_inc;
		status_args1 = vector<int>{ this->get_id(),skill_args[0],attack_inc };
		status_add_flag1 = 1;
		break;
	}
	case 33: {
		int critical_inc = skill_args[2];
		source_atti_z[4] += critical_inc;
		status_args1 = vector<int>{ this->get_id(),skill_args[0],critical_inc };
		status_add_flag1 = 1;
		break;
	}
	case 101: {
		int demege = int(1.0* enemy_atti[0] * skill_args[2]/100);
		status_args2 = vector<int>{ this->get_id(),skill_args[0],demege };
		status_add_flag2 = 1;
		break;
	}
	case 102: {
		int defence_descease = int(1.0*skill_args[2] / 100 * enemy_atti[2]);
		enemy_atti_z[2] -= defence_descease;
		status_args2 = vector<int>{ this->get_id(),skill_args[0],defence_descease };
		status_add_flag2 = 1;
		break;
	}
	/*case 201: {
		vector<int> to_delete;
		for (auto &it : source_status) {
			if (it.first >= 101 && it.first <= 200) {
				//to_delete.push_back(it.first);
				it.second[1] = 1;
			}
		}
		//for (auto &i : to_delete)source_status.erase(i);
		break;
	}*/
	default:
		break;
		//cout << "No such skill!";
	}

	if (status_add_flag1)source_status[this->get_id()] = status_args1;
	if (status_add_flag2)enemy_status[this->get_id()] = status_args2;
	return 1;
}

inline bool skill_base::is_skill_cd()
{  
	if (skill_args[1] != cd_now++) {
		 return true;
	}
	else {
		cd_now = 1;
		return false;
	}
}


