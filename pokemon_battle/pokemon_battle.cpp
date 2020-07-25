

#include <iostream>
#include "skill.h"
#include "pokemon.h"

using std::string;
int main()	//literally handler
{

	vector<int> atti(6);
	atti[0] = 600;atti[1] = 100;atti[2] = 50;atti[3] = 80;atti[4] = 15;atti[5] = 5;
	vector<int> level(3);
	level[0] = 1; level[1] = 20; level[2] = 200;
	pokemon_base *source = new pokemon_r("pikachu", atti, level);

	source->set_skill(0, 1, "龙之力", "攻击和防御小幅提升了", vector<int>{3,5,20,15});
	source->set_skill(1, 201, "净化之力", "消除所有负面状态", vector<int>{-1, 6});
	//source->out_status();
	atti[1] = 120; atti[2] = 30; atti[3] = 100; atti[4] = 10;
	pokemon_base *enemy = new pokemon_r("pikachuuu", atti, level);
	enemy->set_skill(0, 101, "毒液攻击", "使对方中毒", vector<int>{4, 5, 3});
	enemy->set_skill(1, 102, "护甲溶解", "使对方的防御大幅下降", vector<int>{2, 4, 25});
	//enemy->out_status();
	source->battle_with(enemy);
}

