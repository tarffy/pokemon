#include "Handler.h"
#include <string>
using std::string;
#define AD_NAME "AFMowqmt1ga21"
Handler::Handler(QObject *parent)
	: QObject(parent)
{
	driver = sql::mysql::get_mysql_driver_instance();
	if (driver == NULL) {
		emit string_to_socket_ready(QString("Driver failed"), 2);
	}
	con = driver->connect("tcp://localhost:3306", "root", "123456");
	if (con == NULL) {
		emit string_to_socket_ready(QString("Connect to sql failed"), 2);
	}
	stmt = con->createStatement();
	//stmt->execute("set character set gbk");
	stmt->execute("use pokemon_database");
	emit string_to_socket_ready(QString("SQL connect success"), 2);
}

Handler::~Handler()
{
}

void Handler::put_three_pokemons_in_bag()
{
	player.set_next_unique(0);
	res = stmt->executeQuery(QString("select count(*) num from pokemon_base where rarity=1;\0").toUtf8().data());
	QString info="information****注册赠送三只精灵：";
	for (int i = 0; i < 3; ++i) {	
		give_player_random_r(info);
		info.append(i == 2 ? "。" : "，");
	}
	emit string_to_socket_ready(info, 1);
}

void Handler::get_player_pokemons()
{
	res = stmt->executeQuery(QString("select * from pokemon_user where user_name='%1' order by bag_store\0").arg(user_name).toUtf8().data());
	int pokemon_num = 0;
	while (res->next()) {
		pokemon_base *tem = res_to_pokemon(res);
		int bag_store = 0;
		if (res->getInt("bag_store")<6) {
			player.put_pokemon_in_bag(tem);
		}
		else player.put_pokemon_in_store(tem);
	}

	res = stmt->executeQuery(QString("select * from users where user_name='%1'\0").arg(user_name).toUtf8().data());
	if (res->next())player.set_next_unique(res->getInt("next_unique"));
}

pokemon_base * Handler::res_to_pokemon(sql::ResultSet * res)
{
	pokemon_base *pok;
	string pokemon_name = res->getString("pokemon_name").c_str();
	vector<int> id{ res->getInt("id_unique") ,res->getInt("id_pokemon") };
	vector<int> atti{ res->getInt("hp"),res->getInt("attack"), res->getInt("defence"), res->getInt("speed"),
	res->getInt("critical"), res->getInt("miss"), res->getInt("type"), res->getInt("class") };
	vector<int> level{ res->getInt("level"), res->getInt("exp_now"), res->getInt("exp_levelup") };
	switch (res->getInt("rarity"))
	{
	case 1:
		pok = new pokemon_r(pokemon_name, id, atti, level);
		break;
	}
	vector<int> skills = { res->getInt("skill1"),res->getInt("skill2") };
	for (int i = 0; i < 2; ++i) {
		if (skills[i] != -1) {
			res2 = stmt->executeQuery(QString("select * from skill where id=%1;\0").arg(skills[i]).toUtf8().data());
			if (res2->next()) {
				pok->set_skill(i, res2->getInt("id"), "", "", vector<int>{res2->getInt("arg1"), res2->getInt("arg2"), res2->getInt("arg3"), res2->getInt("arg4")});
			}
		}
	}

	return pok;
}

pokemon_base* Handler::give_player_random_r(QString &info)
{
	pokemon_base *pok;
	res= stmt->executeQuery(QString("select count(*) num from pokemon_base where rarity=1\0").toUtf8().data());
	res->next();
	int num = res->getInt("num");
	res = stmt->executeQuery(QString("select * from pokemon_base limit 1 offset %1\0").arg(rand()%num).toUtf8().data());
	if (res->next()) {
		int skill_init = res->getInt("skill_init");
		QString pokemon_info_sql = QString("('%1',%2,%3,'%4',%5,%6,%7,%8,%9,%10,%11,%12,%13,1,0,200,%14,%15,-1,-1,-1)")
			.arg(user_name).arg(player.get_next_unique())
			.arg(res->getInt("id")).arg(res->getString("name").c_str()).arg(res->getInt("rarity"))
			.arg(res->getInt("class")).arg(res->getInt("type"))
			.arg(res->getInt("hp")).arg(res->getInt("attack")).arg(res->getInt("defence"))
			.arg(res->getInt("speed")).arg(res->getInt("critical")).arg(res->getInt("miss"))
			.arg(player.get_store_num()+10).arg(skill_init==-1?-1:skill_init);

		string name = res->getString("name").c_str();
		if(info!="")info.append(res->getString("name").c_str());
		vector<int> atti{ res->getInt("hp") ,res->getInt("attack"),res->getInt("defence"),
			res->getInt("speed"),res->getInt("critical"),res->getInt("miss"),res->getInt("type"),res->getInt("class") };
		vector<int> levels{ 1,0,200 };
		vector<int> id{ player.get_next_unique(),res->getInt("id") };
		player.unique_id_inc();
		pok = new pokemon_r(name, id, atti, levels);
		
		if (skill_init != -1) {
			res2 = stmt->executeQuery(QString("select * from skill where id=%1;\0").arg(skill_init).toUtf8().data());
			if (res2->next()) {
				pok->set_skill(0, res2->getInt("id"), "", "", vector<int>{res2->getInt("arg1"), res2->getInt("arg2"), res2->getInt("arg3"), res2->getInt("arg4")});
			}		
		}
		player.put_pokemon_in_store(pok);
		//emit string_to_socket_ready(QString("insert into pokemon_user values ") + pokemon_info_sql, 2);
		stmt->executeUpdate(QString("update users set next_unique=%1 where user_name='%2'").arg(player.get_next_unique()).arg(user_name).toUtf8().data());
		stmt->executeUpdate((QString("insert into pokemon_user values ") + pokemon_info_sql).toUtf8().data());
	}
	return pok;
}

void Handler::update_pokemon_sql()
{
	unordered_map<int, string > map = player.get_sql_update_info();
	for (auto &it : map) {
		QString str = QString("update pokemon_user set %1 where user_name='%2' and id_unique=%3").arg(QString::fromStdString(it.second)).arg(user_name).arg(it.first);
		stmt->executeUpdate(str.toUtf8().data());
	}
}

void Handler::server_handler()
{
	user_name = AD_NAME;
	player.set_user_name(AD_NAME);
	QString a;
	stmt->execute("delete from pokemon_user where user_name ='AFMowqmt1ga21'");
	//stmt->execute("update users set next_unique = 0 where user_name='AFMowqmt1ga21'");
	player.set_next_unique(0);
	for (int i = 0; i < 10; ++i) {
		give_player_random_r(a);
	}
	get_player_pokemons();
	player.server_handle();
	for (int i = 0; i < 10; i++)check_skill_update(player.find_pok_by_unique(i));
	update_pokemon_sql();
}

void Handler::send_battle_enemy()
{
	QString str_res="battle_pokemon****";
	res = stmt->executeQuery(QString("select * from pokemon_user where user_name='%1'").arg(AD_NAME).toUtf8().data());
	int flag = 1;
	while (res->next()) {
		if (flag)flag = 0; else str_res.append("###");
		str_res.append(res->getString("pokemon_name").c_str());
		str_res.append(" lv.");
		str_res.append(res->getString("level").c_str());
		QString type;
		switch (res->getInt("type"))
		{
		case 0:type = " 水属性"; break;
		case 1:type = " 火属性"; break;
		case 2:type = " 草属性"; break;
		case 3:type = " 普通属性"; break;
		}
		str_res.append(type);
	}
	emit string_to_socket_ready(str_res, 1);
}

void Handler::check_skill_update(pokemon_base *pok)
{
	for(int i=2;i<=4;++i)
	if (pok->need_update[i] == 1) {
		res2 = stmt->executeQuery(QString("select count(*) num from skill\0").toUtf8().data());
		res2->next();
		int skill_num = res2->getInt("num");
		res2 = stmt->executeQuery(QString("select * from skill limit 1 offset %1\0").arg(rand() % skill_num).toUtf8().data());
		if (res2->next()) {
			pok->set_skill(i-1, res2->getInt("id"), "", "", vector<int>{res2->getInt("arg1"), res2->getInt("arg2"), res2->getInt("arg3"), res2->getInt("arg4")});
		}
		stmt->execute(QString("update pokemon_user set skill%1=%2 where user_name='%3' and id_unique=%4\0")
			.arg(i).arg(res2->getInt("id")).arg(user_name).arg(pok->get_unique_id()).toUtf8().data());
		pok->need_update[i] = 0;
	}
	
}

void Handler::user_disconnect()
{
	stmt->executeUpdate(QString("update users set online=0 where user_name='%1' ").arg(user_name).toUtf8().data());
}

void Handler::get_string_from_socket(const QString & str)
{
	QStringList list = str.split("****");
	QString mode = list.at(0);
	if (mode == "login") {
		if (list.at(1).length() < 6 || list.at(2).length() < 6) {	//冗余设置
			emit string_to_socket_ready(QString("login****fail****用户名或密码长度不足6位，请重新输入。"), 1);
			return;
		}
		res = stmt->executeQuery(QString("select * from users where user_name='%1'\0").arg(list.at(1)).toUtf8().data());
		if (res->next()) {
			QString password = res->getString("password").c_str();
			if (password == list.at(2)) {
				user_name = list.at(1);
				player.set_user_name(user_name.toStdString());
				stmt->executeUpdate(QString("update users set online=1 where user_name='%1' ").arg(list.at(1)).toUtf8().data());
				emit string_to_socket_ready(QString("user %1 login success").arg(list.at(1)), 2);
				emit string_to_socket_ready(QString("login****success****登录成功"), 1);
				get_player_pokemons();

				QString pokemon_str = QString::fromStdString(player.out_pokemon_info());
				emit string_to_socket_ready(QString("query_pokemon****") + pokemon_str, 1);
				send_battle_enemy();
			}
			else {
				emit string_to_socket_ready("login failed wrong password ", 2);
				emit string_to_socket_ready(QString("login****fail****密码错误，请检查后重新登录。"), 1);
			}
		}
		else {
			emit string_to_socket_ready("login failed", 2);
			emit string_to_socket_ready(QString("login****fail****用户名%1未被注册，请先注册用户。").arg(list.at(1)), 1);
		}

	}
	else if (mode == "register") {
		if (list.at(1).length() < 6 || list.at(2).length() < 6) {	//冗余设置
			emit string_to_socket_ready(QString("register****fail****用户名或密码长度不足6位，请重新输入。"), 1);
			return;
		}
		res = stmt->executeQuery(QString("select * from users where user_name='%1'\0").arg(list.at(1)).toUtf8().data());
		if (res->next()) {
			emit string_to_socket_ready("register failed",2);
			emit string_to_socket_ready(QString("register****fail****用户名%1已被注册，请换用户名重新注册。").arg(list.at(1)), 1);
		}
		else {
			stmt->executeUpdate(QString("insert into users (user_name,password,online,next_unique) values ('%1','%2',1,3)").arg(list.at(1)).arg(list.at(2)).toUtf8().data());
			user_name = list.at(1);
			player.set_user_name(user_name.toStdString());
			
			emit string_to_socket_ready(QString("user %1 register success").arg(list.at(1)), 2);
			emit string_to_socket_ready(QString("register****success****恭喜您注册成功"),1);
			put_three_pokemons_in_bag();
			send_battle_enemy();
		}
	}
	else if (mode == "query_player") {
		res = stmt->executeQuery("select * from users where online=1\0");
		QString res_str="query_player****";
		int count = 0;
		while (res->next()) {
			if (count)res_str.append("###");
			++count;
			res_str.append(res->getString("user_name").c_str());
		}
		if (count == 0)res_str.append("目前无用户在线");
		emit string_to_socket_ready(QString("user %1 query player success").arg(user_name), 2); 
		emit string_to_socket_ready(res_str, 1);
	}
	else if (mode == "query_pokemon") {//移到了登陆成功
		QString pokemon_str = QString::fromStdString(player.out_pokemon_info());
		emit string_to_socket_ready(QString("query_pokemon****")+pokemon_str, 1);
		emit string_to_socket_ready(QString("user %1 query pokemon success").arg(user_name), 2);
	}
	else if (mode=="query_player_pokemon") {
		res = stmt->executeQuery(QString("select * from pokemon_user where user_name = '%1'\0").arg(list.at(1)).toUtf8().data());
		QString query_res = "query_player_pokemon****";
		int flag = 1;
		while (res->next()) {
			if (flag)flag = 0; else query_res.append("###");
			query_res.append(res->getString("pokemon_name").c_str());
			query_res.append(QString("lv.%1,%2").arg(res->getInt("level")).arg(res->getInt("id_unique")));
		}
		if (flag)query_res.append("-1");
		emit string_to_socket_ready(query_res, 1);		
		emit string_to_socket_ready(QString("%1 query %2 success").arg(user_name).arg(list.at(1)), 2);
	}
	else if (mode == "gacha") {
		QStringList gacha_list = list.at(1).split("###");
		switch (gacha_list.at(0).toInt())
		{
		case 1:
			if (gacha_list.at(1).toInt() == 1) {
				QString str="information****恭喜你获得";
				pokemon_base *pok= give_player_random_r(str);
				str.append("！");
				QString pokemon_str = QString("add_pokemon****")+QString::fromStdString(pok->out_pokemon_info());
				emit string_to_socket_ready(  pokemon_str, 1);
				emit string_to_socket_ready(str, 1);
			}
		}
	}
	else if (mode=="pokemon_fresh") {

		QStringList temlist = list.at(1).split("###");
		QStringList baglist = temlist.at(0).split(","), storelist = temlist.at(1).split(",");
		vector<int> bag, store;
		for (int i = 0; i < baglist.size(); ++i)bag.push_back(baglist.at(i).toInt());
		for (int i = 0; i < storelist.size(); ++i)store.push_back(storelist.at(i).toInt());
		player.fresh_pokemon_pos(bag, store);
		update_pokemon_sql();
	}
	else if (mode == "battle") {
		QString str = QString("battle****")+QString::fromStdString(player.battle_test());
		emit string_to_socket_ready(str, 1);
		update_pokemon_sql();
	}
	else if (mode == "battle_levelup") {
	QString str = QString("battle****");
	QStringList unique_ids = list.at(1).split("###");
	res = stmt->executeQuery(QString("select * from pokemon_user where user_name='%1' and id_unique =%2 \0").arg(AD_NAME).arg(unique_ids.at(1)).toUtf8().data());
	if (res->next()) {
		pokemon_base *enemy = res_to_pokemon(res);
		pokemon_base *source = player.find_pok_by_unique(unique_ids.at(0).toInt());
		str += QString::fromStdString( source->battle_with(enemy));
		check_skill_update(source);
		update_pokemon_sql();
		emit string_to_socket_ready(str, 1);

	}
	}


}
