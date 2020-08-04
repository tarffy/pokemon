#pragma once
#pragma execution_character_set("utf-8")
#include "Player.h"
#include <QObject>
#include "jdbc/mysql_connection.h"
#include "jdbc/mysql_driver.h"
#include "jdbc/cppconn/statement.h"
class Handler : public QObject
{
	Q_OBJECT

public:
	Handler(QObject *parent=NULL);
	~Handler();
	void put_three_pokemons_in_bag();	//注册成功时调用
	void get_player_pokemons();			//登录成功后从SQL获取用户的精灵信息
	pokemon_base *res_to_pokemon(sql::ResultSet *res);//将一条结果信息变成pokemon
	pokemon_base* give_player_random_r(QString &info,int choice=-1);
	void update_pokemon_sql();
	void server_handler();
	void send_battle_enemy();	//登录注册后发送精灵给用户
	void check_skill_update(pokemon_base *pok);
	void insert_pok_to_sql(pokemon_base *pok);
	QString get_player_sql_info(const QString &user_name_);
	void update_sql_battle_num(int win_flag);
	void fresh_player_madels();
private:
	QString user_name;
	sql::mysql::MySQL_Driver *driver;		//mysql驱动
	sql::Connection *con;					//连接
	sql::Statement *stmt;					//语句
	sql::ResultSet *res;					//结果集
	sql::ResultSet *res2;					//结果集
	Player player;
	int no_pokemon = 0;
	int battle_num, battle_win;
	int madels_poknum, madels_pokh;
public slots:
	//将socket传给handler的str进行处理 处理后触发string_to_socket_ready信号
	void get_string_from_socket(const QString &str);	
	void user_disconnect();		//用户disconnect后将SQL 用户online改为0
signals:
	void string_to_socket_ready(const QString &str,int i);		//将string发给socket i为1发给peer socket 为2发给server debug
};
