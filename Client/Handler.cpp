#include "Handler.h"
#include <qdebug.h>
#include <qmessagebox.h>
Handler::Handler(QObject *parent)
	: QObject(parent)
{
	database_init();
}

Handler::~Handler()
{
}

pokemon_base * Handler::string_to_pokemon(const QString & str)
{
	pokemon_base *res;
	QStringList pokemon_args = str.split(",");
	//影魔,1,unique_id,pokemon_id,500,130,40,100,15,10,2,0,1,0,200
	string name = pokemon_args.at(0).toStdString();
	vector<int>id{ pokemon_args.at(2).toInt(),pokemon_args.at(3).toInt() };
	vector<int> atti;
	for (int i = 0; i < 8; i++)atti.push_back(pokemon_args.at(4 + i).toInt());
	vector<int> levels;
	for (int i = 0; i < 3; i++)levels.push_back(pokemon_args.at(12 + i).toInt());

	if (pokemon_args.at(1).toInt() == 1)res = new pokemon_r(name,id, atti, levels);
	return res;
}

void Handler::excute_one_turn(const QString & input, QString & res)
{
	/*"battle****0###
			0&&-1&&-1&&0,0,1,112&&563,142,44,110,15,10<>524,105,52,84,15,5###1&&-1&&-1&&0,1,-1,0&&563,142,44,110,15,10<>524,105,52,84,15,5###
			0&&-1&&-1&&0,0,1,112&&563,142,44,110,15,10<>412,105,52,84,15,5###1&&-1&&-1&&0,0,-1,49&&514,142,44,110,15,10<>412,105,52,84,15,5###
			0&&-1&&-1&&0,0,1,112&&514,142,44,110,15,10<>300,105,52,84,15,5###1&&-1&&-1&&1,0,-1,79&&435,142,44,110,15,10<>300,105,52,84,15,5###
			0&&-1&&-1&&0,0,1,112&&435,142,44,110,15,10<>188,105,52,84,15,5###0&&-1&&-1&&0,0,1,112&&435,142,44,110,15,10<>76,105,52,84,15,5###
			1&&-1&&-1&&0,0,-1,49&&386,142,44,110,15,10<>76,105,52,84,15,5###0&&-1&&-1&&1,0,1,157&&386,142,44,110,15,10<>-81,105,52,84,15,5###FINAL^()"*/
	QStringList turn_list = input.split("$$");
	if (turn_list.at(0).toInt()) {
		res.append("对方行动中\n");
	}
	else {
		res.append("我方行动中\n");
	}
	if (turn_list.at(1) == "-1") {
		res.append("未使用技能\n");
	}
	else {
		QStringList skill_used = turn_list.at(1).split(",");
		for (int i = 0; i < skill_used.size(); i++) {
			auto skill = skill_database[skill_used.at(i).toInt()];
			res.append(QString("使用技能 %1 %2\n").arg(skill.first).arg(skill.second));
		}
	}
	if (turn_list.at(2) == "-1") {//技能状态以<>隔开
		res.append("无技能状态更新\n");
		
	}
	else {
		QStringList status = turn_list.at(2).split("<>");
		for (int i = 0; i < status.size(); ++i) {
			QStringList sta = status.at(i).split(",");
			QString str_res = skill_status_database[sta.at(0).toInt()];
			for (int i = 1; i < sta.size(); ++i) {
				str_res = str_res.arg(sta.at(i).toInt());
			}
			res.append(str_res);
		}

	}
	QStringList demage_list = turn_list.at(3).split(",");
	if (demage_list.at(0).toInt()) {
		res.append("造成了暴击，");
		if (demage_list.at(2).toInt() == 1) {
			res.append("属性克制，");
		}
		else if (demage_list.at(2).toInt() == -1) {
			res.append("属性被克制，");
			}
		res.append(QString("造成了%1点伤害。\n").arg(demage_list.at(3)));
	}
	else if (demage_list.at(1).toInt()) {
		res.append("对方闪避了攻击,造成了0点伤害。\n");
	}
	else {
		if (demage_list.at(2).toInt() == 1) {
			res.append("属性克制，");
		}
		else if (demage_list.at(2).toInt() == -1) {
			res.append("属性被克制，");
		}
		res.append(QString("造成了%1点伤害。\n").arg(demage_list.at(3)));
	}
	QStringList pokemon_status = turn_list.at(4).split("<>");
	for (int i = 0; i < 2; i++) {
		QStringList atti = pokemon_status.at(i).split(",");
		if (i)res.append("对方精灵属性:\n");
		else res.append("我方精灵属性:\n");
		res.append(QString("HP:%1 攻击:%2 防御:%3 速度:%4 暴击率:%5% 闪避率:%6%\n")
			.arg(atti.at(0)).arg(atti.at(1)).arg(atti.at(2)).arg(atti.at(3)).arg(atti.at(4)).arg(atti.at(5)));
	}
	//qDebug() << res;
}

void Handler::database_init()
{
	skill_database[2] = pair<QString, QString>{ "火舞", "下次攻击必定暴击。" };
	skill_database[3] = pair<QString, QString>{ "狂热","攻击力提升20%，速度提升10%。" };
	skill_database[4] = pair<QString, QString>{ "灼烧","使对方持续损失生命且防御力下降10%。" };
	skill_database[5] = pair<QString, QString>{ "火焰盾","增加10%的防御和10%的闪避。" };
	skill_database[10] = pair<QString, QString>{ "洪流","攻击力提升15%，并立刻发动下一次攻击。" };
	skill_database[11] = pair<QString, QString>{ "极寒","使对方持续损失生命值且攻击力下降10%。" };
	skill_database[12] = pair<QString, QString>{ "雾气弥漫","闪避几率提升20%。" };
	skill_database[13] = pair<QString, QString>{ "湿润皮肤","每次行动回复5%的生命值。" };
	skill_database[20] = pair<QString, QString>{ "扎根","恢复20%的生命值并增加20%防御。" };
	skill_database[21] = pair<QString, QString>{ "猛毒素","使对方持续损失生命值且速度下降5%。" };
	skill_database[22] = pair<QString, QString>{ " 寄生种子","每回合吸取对方4%的生命值。" };
	skill_database[23] = pair<QString, QString>{ "光合作用","本次攻击，攻击力提升40%。" };
	skill_database[30] = pair<QString, QString>{ "愈战愈勇","每次行动都会增加5%的攻击和防御。" };
	skill_database[31] = pair<QString, QString>{ "破甲","降低对方20%防御力" };
	skill_database[32] = pair<QString, QString>{ "剑舞","提升20%攻击力。" };
	skill_database[33] = pair<QString, QString>{ "专注","提升10%暴击几率。" };


	//skill_database[] = pair<QString, QString>{ "","" };
	skill_status_database[4] = "因为灼烧受到了%1点伤害\n";
	skill_status_database[11] = "因为极寒受到了%1点伤害\n";
	skill_status_database[21] = "因为中毒受到了%1点伤害\n";

	pok_names[1] = "巴鲁斯";
	pok_names[2] = "火爆猴";
	pok_names[3] = "步步种子";
	//pok_names[] = "";
}

void Handler::handle_str_from_socket(const QString & str)
{
	QStringList strs = str.split("^()");
	for (int i = 0; i < strs.size(); i++) {
		if (strs.at(i).length() == 0)continue;
		QStringList list = strs.at(i).split("****");
		QString mode = list.at(0);
		if (mode == "register" || mode == "login") {
			if (list.at(1) == "success") {
				emit register_or_login_success(list.at(2));
			}
			else {
				emit register_or_login_fail(list.at(2));
			}
		}
		else if (mode == "query_player") {
			emit query_success(list.at(1));
		}
		else if (mode == "information") {
			emit socket_information(list.at(1));
		}
		else if (mode == "query_pokemon") {
			QStringList pokemons = list.at(1).split("###");
			QStringList bag_name;
			QStringList store_name;
			if (pokemons.at(0) != "-1") {
				QStringList pokemon_bag = pokemons.at(0).split("$$");
				for (int i = 0; i < pokemon_bag.size(); ++i) {
					player.put_pokemon_in_bag(string_to_pokemon(pokemon_bag.at(i)));
				}
			}
			if (pokemons.at(1) != "-1") {
				QStringList pokemon_store = pokemons.at(1).split("$$");
				for (int i = 0; i < pokemon_store.size(); ++i) {
					player.put_pokemon_in_store(string_to_pokemon(pokemon_store.at(i)));
				}
			}
			QString names = QString::fromStdString(player.get_all_names()) ;

			emit pokemon_info_ready(names);
			
		}
		else if (mode=="query_player_pokemon") {//精灵以###隔开 名字登记,unique id
			emit player_pokemon_ready(list);
		}
		else if (mode == "add_pokemon") {
			player.put_pokemon_in_store(string_to_pokemon(list.at(1)));
			QString names = QString::fromStdString(player.get_all_names());
			emit pokemon_info_ready(names);
		}
		else if (mode == "battle") {
			QStringList repo;			
			QStringList turns = list.at(1).split("###");
			int win_flag = turns.at(0).toInt();	//1为获胜
			QStringList pok_ids = turns.at(2).split(",");
			QString start;	//at(1)
			QStringList pokemon_status = turns.at(1).split("<>");
			for (int i = 0; i < 2; i++) {
				QStringList atti = pokemon_status.at(i).split(",");
				if (i)start.append(QString("%1 lv.%2\n").arg(pok_names[pok_ids[3].toInt()]).arg(pok_ids[5].toInt()));
				else start.append(QString("%1 lv.%2\n").arg(pok_names[pok_ids[2].toInt()]).arg(pok_ids[4].toInt()));
				start.append(QString("HP:%1 攻击:%2 防御:%3\n速度:%4 暴击率:%5% 闪避率:%6%\n")
					.arg(atti.at(0)).arg(atti.at(1)).arg(atti.at(2)).arg(atti.at(3)).arg(atti.at(4)).arg(atti.at(5)));
				start.append("<>");
			}
			repo.append(start);
			
			pokemon_base *pok = player.find_pok_by_unique(pok_ids[0].toInt());
			repo.append(QString("%1,%2").arg(pok_ids[2].toInt()).arg(pok_ids[3].toInt()));
			
			

			for (int i = 3; i < turns.size()-1; i++) {
				QString turn;
				excute_one_turn(turns.at(i), turn);
				repo.append(turn);
			}
			if (win_flag)repo.append("\n我方获胜！");
			else repo.append("\n对方获胜!");
			//150,6,6,1050,1200,
			//150,6,7,0,1400,849,158,52,132,###1,10	升级的
			QStringList levels = list.at(2).split("###");
			QStringList exps = levels[0].split(",");
			
			repo.append(QString("获得了%1点经验，").arg(exps[0]));
			pok->gain_exp(exps[0].toInt());
			if (exps[1] == exps[2]) {
				repo.append(QString("当前经验值:%1,升级需要经验值:%2。\n").arg(exps[3]).arg(exps[4]));
				
			}
			else {
				repo.append(QString("精灵升到%1级!\n升级后属性：\nHP:%2 攻击:%3 防御:%4 速度:%5\n")
					.arg(exps[2]).arg(exps[5]).arg(exps[6]).arg(exps[7]).arg(exps[8]));
				repo.append(QString("当前经验值:%1,升级需要经验值:%2。\n").arg(exps[3]).arg(exps[4]));
				
			}
			if (list.at(3) != "-1") {
				QStringList three = list.at(3).split(",");
				if (three.size() == 1) {
					repo.append("决胜赛获胜，恭喜获得该精灵\n");
					repo.append("0");
				}
				else {
					repo.append("决胜赛失败，你必须送出一个精灵。\n");
					repo.append("1,"+list.at(3));
				}
			}
			else {
				repo.append("0");
			}
			emit repo_ready(repo);
		}
		else if (mode == "battle_pokemon") {
			QStringList enemy_list = list.at(1).split("###");
			emit enemy_list_ready(enemy_list);
		}
		else if (mode == "player_info") {
		emit player_info_ready(list.at(1));
		}
	}
}
