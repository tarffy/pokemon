

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

	source->set_skill(0, 1, "龙之力", "攻击和防御小幅提升了", vector<int>{3,4,20,15});

	//source->out_status();
	atti[1] = 120; atti[2] = 30; atti[3] = 100; atti[4] = 10;
	pokemon_base *enemy = new pokemon_r("pikachuuu", atti, level);
	enemy->set_skill(0, 2, "毒液攻击", "使对方中毒", vector<int>{999, 999, 4});
	//enemy->out_status();
	source->battle_with(enemy);
}

