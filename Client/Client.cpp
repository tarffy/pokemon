#include "Client.h"

Client::Client(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

	socket = new QTcpSocket(this);
	socket->connectToHost(QHostAddress::LocalHost, 8888);
	if (socket->waitForConnected()) {
		ui.Text_status->setText("connected");
	}
	else {
		ui.Text_status->setText("connect failed");
	}
	connect(ui.Button_socket, &QPushButton::clicked, this, &Client::send_to_socket);
	connect(socket, &QTcpSocket::readyRead, this, &Client::read_from_socket);
	connect(ui.Button1, &QPushButton::clicked,  this, &Client::change_to_battle);
	connect(ui.Button2, &QPushButton::clicked, this, &Client::back_to_menu);
}

void Client::change_to_battle()
{
	ui.Swidgt->setCurrentIndex(1);
}

void Client::back_to_menu()
{
	ui.Swidgt->setCurrentIndex(0);
}

void Client::send_to_socket()
{
	QString str = ui.Text_send->toPlainText();
	socket->write(str.toLocal8Bit());
}

void Client::read_from_socket()
{
	QString str = socket->readAll();
	ui.Text_read->append(str);
}
