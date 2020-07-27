#include "Client.h"
#include <qmessagebox.h>
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
	connect(ui.Button_socket, &QPushButton::clicked, this, &Client::send_to_socket);
	connect(socket, &QTcpSocket::readyRead, this, &Client::read_from_socket);

	handler = new Handler();
	connect(this, &Client::socket_to_handler_ready, handler, &Handler::handle_str_from_socket);
	connect(handler, &Handler::register_or_login_success, [=](const QString &str) {
		QMessageBox::information(this, "提示", str);
		change_to_menu();
	});
	connect(handler, &Handler::register_or_login_fail, [=](const QString &str) {
		QMessageBox::information(this, "提示", str);
	});
	connect(handler,&Handler::socket_information, [=](const QString &str) {
		QMessageBox::information(this, "提示", str);
	});
	connect(ui.Button_disconnect, &QPushButton::clicked, socket, &QTcpSocket::disconnectFromHost);
	//登录页
	connect(ui.button_login, &QPushButton::clicked, this, &Client::try_login);
	connect(ui.button_register, &QPushButton::clicked, this, &Client::try_register);
	//菜单页
	connect(ui.Button_logout, &QPushButton::clicked, this, &QWidget::close);
	//战斗页
	connect(ui.Button_enter_battle, &QPushButton::clicked,  this, &Client::change_to_battle);
	connect(ui.Button_return_menu1, &QPushButton::clicked, this, &Client::change_to_menu);

	
	//查询页
	connect(ui.Button_query, &QPushButton::clicked, this, &Client::change_to_query);
	connect(ui.Button_return_menu2, &QPushButton::clicked, this, &Client::change_to_menu);
	connect(ui.Button_fresh_online_player, &QPushButton::clicked, this, &Client::try_query_player_online);
	connect(handler, &Handler::query_success, this, &Client::show_query_player_result);

	//精灵页
	connect(ui.Button_pokemons, &QPushButton::clicked, this, &Client::change_to_pokemon);
	connect(ui.Button_return_menu3, &QPushButton::clicked, this, &Client::change_to_menu);
	connect(handler, &Handler::pokemon_info_ready, this, &Client::show_pokemon_info);
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
	try_query_pokemon_info();
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

void Client::send_to_socket()
{
	QString str = ui.Text_send->toPlainText();
	socket->write(str.toUtf8());
}

void Client::try_login()
{
	QString user_name = ui.line_username->text();
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
	QString user_name = ui.line_username->text();
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
	QString str = socket->readAll();
	qDebug() << "from socket:"<<str;
	emit socket_to_handler_ready(str);
}
