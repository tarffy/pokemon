#include "Client.h"
#include <qmessagebox.h>
Client::Client(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);




	socket = new QTcpSocket(this);
	socket->connectToHost(QHostAddress::LocalHost, 8888);
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
	connect(ui.button_login, &QPushButton::clicked, this, &Client::try_login);
	connect(ui.button_register, &QPushButton::clicked, this, &Client::try_register);

	connect(ui.Button1, &QPushButton::clicked,  this, &Client::change_to_battle);
	connect(ui.Button2, &QPushButton::clicked, this, &Client::change_to_menu);
	connect(ui.Button_disconnect, &QPushButton::clicked, socket, &QTcpSocket::disconnectFromHost);


}

void Client::change_to_battle()
{
	ui.Swidgt->setCurrentIndex(1);
}

void Client::change_to_menu()
{
	ui.Swidgt->setCurrentIndex(0);
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
	QString res = QString("register****%1****%2").arg(user_name).arg(password);
	socket->write(res.toUtf8());
}

void Client::read_from_socket()
{
	QString str = socket->readAll();
	qDebug() << "from socket:"<<str;
	emit socket_to_handler_ready(str);
}
