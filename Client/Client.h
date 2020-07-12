#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Client.h"
#include <qpushbutton.h>
#include <QtNetwork>
class Client : public QMainWindow
{
    Q_OBJECT

public:
    Client(QWidget *parent = Q_NULLPTR);

private:
    Ui::ClientClass ui;
	QTcpSocket *socket;
public slots:
	void send_to_socket();	//���ı�����Ϣ����socket �϶��ø�
	void read_from_socket();//��socket����Ϣ
public:
	void change_to_battle();
	void back_to_menu();

};
