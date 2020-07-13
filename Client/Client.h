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
	void socket_to_handler_ready(const QString &str);	//�źż����socket��������Ϣ����handler����
public slots:
	void send_to_socket();	//���ı�����Ϣ����socket �϶��ø�
	void read_from_socket();//��socket����Ϣ
	void try_login();			//���Ե�¼
	void try_register();		//����ע��
	void change_to_battle();
	void change_to_menu();

};
