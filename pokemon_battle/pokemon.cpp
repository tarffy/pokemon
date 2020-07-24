#include "pokemon.h"
#include <iostream>
using std::cout;
using std::endl;
void pokemon_r::use_skills(pokemon_base * enemy)
{
	auto &skills = this->get_skills();
	//skills[0]->skill_use(this,enemy);
	for (auto &skill : skills) {
		if(skill)skill->skill_use(this, enemy);
	}
}

void pokemon_r::battle_with(pokemon_base * enemy)
{
	int distence = 1000, source_distence = 0, enemy_distence = 0;
	vector<int> &source_atti_z = this->get_attribute_z();
	vector<int> &enemy_atti_z = enemy->get_attribute_z();
	int battle_continue = 1,win_flag;
	cout << "战斗开始了bro\n";
	this->out_z_status();
	enemy->out_z_status();
	while (battle_continue) {
		source_distence += source_atti_z[3];
		enemy_distence += enemy_atti_z[3];
		if (source_distence>=enemy_distence&& source_distence > distence) {
			cout << "\n我方行动中！"<<endl;
			source_distence -= distence;

			this->use_skills(enemy);
			int demage = source_atti_z[1] - enemy_atti_z[2];
			if (get_true_at_rate(source_atti_z[4])) {
				demage+=demage/2;
				cout << "造成了暴击！\n";
			}
			else if (get_true_at_rate(enemy_atti_z[5])) {
				demage = 0;
				cout << "对方闪避了伤害！\n";
			}

			cout << "我方造成了" << demage << "点伤害!\n";
			enemy_atti_z[0] -= demage;

			this->status_fresh();

			this->out_z_status();
			enemy->out_z_status();

			if (enemy_atti_z[0] <= 0) { 
			win_flag = 1;
			break;

			}
		}
		if (enemy_distence>=source_distence&& enemy_distence > distence) {
			cout << "\n敌方行动中！"<<endl;
			enemy_distence -= distence;

			enemy->use_skills(this);
			int demage = enemy_atti_z[1] - source_atti_z[2];
			if (get_true_at_rate(enemy_atti_z[4])) {
				demage += demage / 2;
				cout << "造成了暴击！\n";
			}
			else if (get_true_at_rate(source_atti_z[5])) {
				demage = 0;
				cout << "对方闪避了伤害！\n";
			}

			source_atti_z[0] -= demage;
			cout << "啊受到了" << demage << "点伤害!\n";

			enemy->status_fresh();

			this->out_z_status();
			enemy->out_z_status();

			

			if (source_atti_z[0] <= 0) {
				win_flag = 0;
				break;
			}
		}

	}
	if (win_flag)cout << "老弟真的猛\n";
	else cout << "哈哈被打丢了吧\n";
}




void pokemon_base::status_fresh()
{
		auto& status = this->get_status();
		for (auto &i : status) {
			auto it = i.second;
			--it[1];
			switch (it[0]) {
			case 1: {
				if (!it[1]) {
					auto& atti_z = this->get_attribute_z();
					atti_z[1] -= it[2];
					atti_z[2] -= it[3];
				}
				break;
			}
			case 2: {
				attributes_z[0] -= it[2];
				cout << "因为中毒受到了" << it[2] << "点伤害\n";
				break;
			}
			}
		}
}

bool pokemon_base::get_true_at_rate(double rate)
{ 
	return rate > (rand() % 10000)*1.0 / 10000;
}

bool pokemon_base::get_true_at_rate(int rate100)
{
	return get_true_at_rate(1.0*rate100 / 100);
}

void pokemon_base::out_status()
{

	cout << "Name:" << pokemon_name << endl;
	cout << "Level: " << levels[0] << " Exp_now: " << levels[1] << " Exp_levelup: " << levels[2] << endl;
	cout << "Attributes: HP:" << attributes[0] << " Attack: " << attributes[1] << " Defence: " << attributes[2]
		<< " Speed: " << attributes[3] << " Critical: " << attributes[4] << " Miss: " << attributes[5] << endl;
	cout << "Attributes: HP:" << attributes_z[0] << " Attack: " << attributes_z[1] << " Defence: " << attributes_z[2]
		<< " Speed: " << attributes_z[3] << " Critical: " << attributes_z[4] << " Miss: " << attributes_z[5] << endl;

}

void pokemon_base::out_z_status()
{
	cout << "Name:" << pokemon_name << endl;
	cout << "Attributes: HP:" << attributes_z[0] << " Attack: " << attributes_z[1] << " Defence: " << attributes_z[2]
		<< " Speed: " << attributes_z[3] << " Critical: " << attributes_z[4] << " Miss: " << attributes_z[5] << endl;
}

void pokemon_base::set_skill(int cur, int id, string name, string descrip, vector<int> skill_args)
{
	skill_base *skill_set = new skill_base(name,descrip , id, skill_args);
	this->get_skills()[cur] = skill_set;
}
