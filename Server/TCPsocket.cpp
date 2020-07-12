#include "TCPsocket.h"

TCPsocket::TCPsocket(QObject *parent)
	: QObject(parent)
{
}

TCPsocket::~TCPsocket()
{
}

void TCPsocket::tell_server_disconnected()
{
	QString res = QString("disconnect****%1").arg(socketdescriptor);
	emit string_to_server_ready(res);
}

void TCPsocket::string_from_handler(const QString & str)
{
	socket->write(str.toUtf8());
}

void TCPsocket::string_to_handler()
{
	QString str = socket->readAll();
	emit string_to_handler_ready(str);
}

void TCPsocket::socket_init(const qintptr socketDescriptor)
{

	socket = new QTcpSocket(this);
	socket->setSocketDescriptor(socketDescriptor);
	connect(socket, &QTcpSocket::readyRead, this, &TCPsocket::string_to_handler);
	handler = new Handler(this);
	connect(this, &TCPsocket::string_to_handler_ready, handler, &Handler::get_string_from_socket);
	connect(handler, &Handler::string_to_socket_ready, this, &TCPsocket::string_from_handler);
	socketdescriptor = socketDescriptor;
	connect(socket, &QTcpSocket::disconnected, this, &TCPsocket::tell_server_disconnected);
}
