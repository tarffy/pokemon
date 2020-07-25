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
public:

signals:
	void socket_to_handler_ready(const QString &str);	//信号激活后将socket读到的信息发给handler处理
public slots:
	void send_to_socket();	//将文本框信息发给socket 肯定得改
	void read_from_socket();//从socket读信息
	void try_login();			//尝试登录
	void try_register();		//尝试注册
	void change_to_battle();
	void change_to_menu();

};
