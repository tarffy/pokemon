#include "Handler.h"
#include <string>
using std::string;
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
	res = stmt->executeQuery(QString("select count(*) num from pokemon_base where rarity=1;\0").toUtf8().data());
	QString info="information****注册赠送三只精灵：";
	for (int i = 0; i < 3; ++i) {
		int choice = rand() % 3 + 1;
		res = stmt->executeQuery(QString("select * from pokemon_base where id=%1;\0").arg(choice).toUtf8().data());
		if (res->next()) {
			/*
			QString pokemon_info = QString("%1,%2,%3,%4,%5,%6,%7,%8,%9").arg(res->getString("name").c_str())
				.arg(res->getInt("hp")).arg(res->getInt("attack")).arg(res->getInt("defence"))
				.arg(res->getInt("speed")).arg(res->getInt("critical")).arg(res->getInt("miss"))
				.arg(res->getInt("type")).arg(res->getInt("class"));
			emit string_to_socket_ready(pokemon_info, 2);*/
			string name = res->getString("name").c_str();
			info.append(res->getString("name").c_str());
			info.append(i == 2 ? "。" : "，");
			vector<int> atti { res->getInt("hp") ,res->getInt("attack"),res->getInt("defence"),
				res->getInt("speed"),res->getInt("critical"),res->getInt("miss"),res->getInt("type"),res->getInt("class") };
			vector<int> levels{ 1,0,200 };
			pokemon_base *pok = new pokemon_r(name, atti, levels);
			player.put_pokemon_in_bag(pok);
		}
		
	}
	emit string_to_socket_ready(info, 1);
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
				stmt->executeUpdate(QString("update users set online=1 where user_name=\"%1\" ").arg(list.at(1)).toUtf8().data());
				emit string_to_socket_ready(QString("user %1 login success").arg(list.at(1)), 2);
				emit string_to_socket_ready(QString("login****success****登录成功"), 1);
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
			stmt->executeUpdate(QString("insert into users (user_name,password,online) values ('%1','%2',1)").arg(list.at(1)).arg(list.at(2)).toUtf8().data());
			user_name = list.at(1);
			player.set_user_name(user_name.toStdString());
			
			emit string_to_socket_ready(QString("user %1 register success").arg(list.at(1)), 2);
			emit string_to_socket_ready(QString("register****success****恭喜您注册成功"),1);
			put_three_pokemons_in_bag();
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
	else if (mode == "query_pokemon") {
		QString pokemon_str = QString::fromStdString(player.out_pokemon_info());
		emit string_to_socket_ready(QString("query_pokemon****")+pokemon_str, 1);
	}


}
