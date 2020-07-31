#include "Client.h"
#include <qmessagebox.h>
#include <qtestsupport_core.h>
Client::Client(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	socket = new QTcpSocket(this);
	socket->connectToHost(QHostAddress::LocalHost, 8888);//"188.131.139.250" QHostAddress::LocalHost
	if (socket->waitForConnected()) {
		qDebug()<< "TCP connected";
	}
	else {
		qDebug() << "TCP connect failed";
		QMessageBox::information(this, "提示", "无法连接到服务器!");
	}
	connect(socket, &QTcpSocket::readyRead, this, &Client::read_from_socket);

	handler = new Handler();
	connect(this, &Client::socket_to_handler_ready, handler, &Handler::handle_str_from_socket);
	connect(handler, &Handler::register_or_login_success, this,&Client::register_or_login_success_slot);
	connect(handler, &Handler::register_or_login_fail, [=](const QString &str) {
		QMessageBox::information(this, "提示", str);
	});
	connect(handler,&Handler::socket_information, [=](const QString &str) {
		QMessageBox::information(this, "提示", str);
	});
	//登录页
	connect(ui.button_login, &QPushButton::clicked, this, &Client::try_login);
	connect(ui.button_register, &QPushButton::clicked, this, &Client::try_register);
	//菜单页
	connect(ui.Button_logout, &QPushButton::clicked, this, &QWidget::close);
	//战斗页
	connect(ui.Button_enter_battle, &QPushButton::clicked,  this, &Client::change_to_battle);

	connect(ui.Button_return_menu1, &QPushButton::clicked, this, &Client::change_to_menu);
	connect(ui.Button_battle1, &QToolButton::clicked, this, &Client::try_battle);
	connect(handler, &Handler::repo_ready, this, &Client::show_repo);
	connect(handler, &Handler::enemy_list_ready, this, &Client::show_enemy);
	connect(ui.Button_battle_levelup, &QPushButton::clicked, this, &Client::battle_levelup);
	//查询页
	connect(ui.Button_query, &QPushButton::clicked, this, &Client::change_to_query);
	connect(ui.Button_return_menu2, &QPushButton::clicked, this, &Client::change_to_menu);
	connect(ui.Button_fresh_online_player, &QPushButton::clicked, this, &Client::try_query_player_online);
	connect(handler, &Handler::query_success, this, &Client::show_query_player_result);
	connect(ui.Button_query_player_pokemon, &QPushButton::clicked, this, &Client::try_query_player_pokemon);
	connect(handler, &Handler::player_pokemon_ready, this, &Client::show_query_player_pokemon);
	//精灵页
	connect(ui.Button_pokemons, &QPushButton::clicked, this, &Client::change_to_pokemon);
	connect(ui.Button_return_menu3, &QPushButton::clicked, this, &Client::change_to_menu);
	connect(ui.Button_return_menu3, &QPushButton::clicked, this, &Client::try_fresh_pokemon);
	connect(handler, &Handler::pokemon_info_ready, this, &Client::show_pokemon_info);
	connect(ui.Button_pokemon_to_store, &QPushButton::clicked, this, &Client::put_pokemon_to_store);
	connect(ui.Button_pokemon_to_bag, &QPushButton::clicked, this, &Client::put_pokemon_to_bag);
	//抽奖页
	connect(ui.Button_gacha, &QPushButton::clicked, this, &Client::change_to_gacha);
	connect(ui.Button_return_menu4, &QPushButton::clicked, this, &Client::change_to_menu);
	connect(ui.Button_gacha1_1, &QPushButton::clicked, this, &Client::try_gacha);
}

void Client::change_to_battle()
{
	ui.Swidgt->setCurrentIndex(2);
}

void Client::change_to_menu()
{
	ui.Swidgt->setCurrentIndex(1);
}

void Client::change_to_query()
{
	ui.Swidgt->setCurrentIndex(3);
	try_query_player_online();
}

void Client::try_query_player_online()
{
	QString res = "query_player";
	socket->write(res.toUtf8());
}

void Client::show_query_player_result(const QString & str)
{
	ui.list_player_online->clear();
	QStringList list= str.split("###");
	ui.list_player_online->addItems(list);
}

void Client::change_to_pokemon()
{
	ui.Swidgt->setCurrentIndex(4);
	/*if (get_pokemon) {
		get_pokemon = 0;
		try_query_pokemon_info();
	}*/
}

void Client::change_to_gacha()
{
	ui.Swidgt->setCurrentIndex(5);
}

void Client::show_pokemon_info(const QString &str)
{
	//采用###分割bag和store 采用&&分割不同精灵
	QStringList list = str.split("###");
	ui.list_pokemon_bag->clear();
	ui.list_pokemon_store->clear();
	if (list.at(0) != "-1") {
		ui.list_pokemon_bag->addItems(list.at(0).split("$$"));
	}
	if (list.at(1) != "-1") {
		ui.list_pokemon_store->addItems(list.at(1).split("$$"));
	}
	
}

void Client::register_or_login_success_slot(const QString &str)
{
		QMessageBox::information(this, "提示", str);
		ui.label_menu_info->setText(QString("用户名:%1\n勋章：没").arg(user_name));
		change_to_menu();
		
}

void Client::try_gacha()
{
	QString str = "gacha****";
	if (sender() == ui.Button_gacha1_1) {
		str.append("1###1");
	}
	else return;
	socket->write(str.toUtf8());
}

void Client::put_pokemon_to_store()
{
	int num = ui.list_pokemon_bag->currentRow();
	if (num == -1) {
		QMessageBox::information(this, "提示", "未选择精灵。");
		return;
	}
	ui.list_pokemon_store->addItem(ui.list_pokemon_bag->takeItem(num));
	handler->player.move_to_store(num);
}

void Client::put_pokemon_to_bag()
{
	if (ui.list_pokemon_bag->count() == 6) {
		QMessageBox::information(this,"提示","背包已满。");
		return;
	}
	int num = ui.list_pokemon_store->currentRow();
	if (num == -1) {
		QMessageBox::information(this, "提示", "未选择精灵。");
		return;
	}
	ui.list_pokemon_bag->addItem(ui.list_pokemon_store->takeItem(num));
	handler->player.move_to_bag(num);
	
}

void Client::try_query_player_pokemon()
{
	QString query_user_name = ui.list_player_online->currentItem()->text();
	QString str = QString("query_player_pokemon****%1").arg(query_user_name);
	socket->write(str.toUtf8());
}

void Client::show_query_player_pokemon(const QString &str)
{
	ui.list_player_pokemon->clear();
	if (str == "-1") { 
		ui.list_player_pokemon->addItem("用户无精灵"); 
		return;
	}
	QStringList list = str.split("###");
	for (int i = 0; i < list.size(); i++) {
		ui.list_player_pokemon->addItem(list[i].split(",").at(0));
	}
}

void Client::try_fresh_pokemon()
{
	QString str = "pokemon_fresh****";
	str.append(QString::fromStdString( handler->player.pokemon_pos()));//用###分割背包仓库,分割不同精灵
	socket->write(str.toUtf8());
}

void Client::try_battle()
{
	QString str = "battle****";
	socket->write(str.toUtf8());
}

void Client::show_repo(const QStringList & list)
{
	
	ui.Text_battle_2->clear();
	ui.Swidgt->setCurrentIndex(6);
	for (int i = 0; i < list.size(); i++) {
		
		ui.Text_battle_2->append(list.at(i));
		QTest::qWait(1000);
	}
	ui.Swidgt->setCurrentIndex(2);
}

void Client::show_enemy(const QStringList & list)
{
	ui.list_battle_enemy->clear();
	ui.list_battle_enemy->addItems(list);
}

void Client::battle_levelup()
{
	int num = ui.list_battle_enemy->currentRow();
	if (num == -1) {
		QMessageBox::information(this, "提示", "未选择升级战对战精灵。");
		return;
	}
	QString res = QString("battle_levelup****%1###%2").arg(handler->player.get_unique_by_pos(0)).arg(num);
	socket->write(res.toUtf8());
}


void Client::try_login()
{
	user_name = ui.line_username->text();
	if (user_name.length() < 6) {
		QMessageBox::information(this, "提示", "用户名长度应至少六位，请重新输入。");
		return;
	}
	QString password = ui.line_password->text();
	if (password.length() < 6) {
		QMessageBox::information(this, "提示", "密码长度应至少六位，请重新输入。");
		return;
	}
	handler->player.set_user_name(user_name.toStdString());
	QString res = QString("login****%1****%2").arg(user_name).arg(password);
	socket->write(res.toUtf8());
}

void Client::try_register()
{
	user_name = ui.line_username->text();
	if (user_name.length() < 6) {
		QMessageBox::information(this, "提示", "用户名长度应至少六位，请重新输入。");
		return;
	}
	QString password = ui.line_password->text();
	if (password.length() < 6) {
		QMessageBox::information(this, "提示", "密码长度应至少六位，请重新输入。");
		return;
	}
	handler->player.set_user_name(user_name.toStdString());
	QString res = QString("register****%1****%2").arg(user_name).arg(password);
	socket->write(res.toUtf8());
}

void Client::try_query_pokemon_info()
{
	QString res = "query_pokemon";
	socket->write(res.toUtf8());
}

void Client::read_from_socket()
{	
	QTest::qWait(50);
	QString str = socket->readAll();
	qDebug() << "from socket:"<<str;
	emit socket_to_handler_ready(str);
}
