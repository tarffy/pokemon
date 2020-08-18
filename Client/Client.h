#pragma once
#pragma execution_character_set("utf-8")
#include <QtWidgets/QMainWindow>
#include "ui_Client.h"
#include <qpushbutton.h>
#include <QtNetwork>
#include "Handler.h"
class Client : public QMainWindow
{
    Q_OBJECT

public:
    Client(QWidget *parent = Q_NULLPTR);

private:
    Ui::ClientClass ui;
	QTcpSocket *socket;
	Handler *handler;
	int get_pokemon = 1;
	QString user_name;
	int first_, second_, third_;
	int battle_num, battle_win;
	vector<bool> madels;
	void set_palettes();
public:
	void try_query_pokemon_info();
signals:
	void socket_to_handler_ready(const QString &str);	//信号激活后将socket读到的信息发给handler处理
public slots:
	void read_from_socket();//从socket读信息
	void try_login();			//尝试登录
	void try_register();		//尝试注册
	void change_to_battle();	//切换到战斗界面
	void change_to_menu();		//切换到菜单界面
	void change_to_query();		//切换到查询界面
	void change_to_pokemon();	//切换到精灵界面
	void change_to_gacha();
	void change_to_send_pok();
	void try_query_player_online();	//查询在线的玩家
	void show_query_player_result(const QString &str);	//根据查询结果 显示在线的玩家
	void show_pokemon_info(const QString &str);		//根据返回结果显示背包和仓库内的精灵
	void register_or_login_success_slot(const QString &str);
	void try_gacha();
	void put_pokemon_to_store();
	void put_pokemon_to_bag();
	void try_query_player_pokemon();
	void show_query_player_pokemon(const QStringList &list);
	void try_fresh_pokemon();
	void show_repo(const QStringList &list);
	void show_enemy(const QStringList &list);
	void battle_levelup();
	void battle_duel();
	void try_send_pok();
	void try_fresh_player();
	void show_player_info(const QString &str);
	void show_pok_info_in_pokpage(int row);
	void try_query_all_pokemon();
	void show_all_pokemon(const QString &list);
};
