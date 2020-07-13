#pragma once

#include <QObject>
#include <qtcpsocket.h>
#include "Handler.h"
class TCPsocket : public QObject
{
	Q_OBJECT

public:
	TCPsocket(QObject *parent=NULL);
	~TCPsocket();
	
signals:
	void string_to_handler_ready(const QString &str);	//socket read完后将str发给handler处理
	void string_to_server_ready(const QString &str);	//socket 将str发给server 目前用来disconnect
public slots:
	void socket_init(const qintptr socketDescriptor);	//初始化socket和handler对象
	void string_to_handler();							//将socket收到的字符串发给handler 处理
	void string_from_handler(const QString &str,int i);		//将handler处理完的str发回给socket 
	void tell_server_disconnected();					//将disconnected信息发给server
private:
	QTcpSocket *socket;				//socket
	Handler *handler;				//处理信息的handler
	qintptr socketdescriptor;		//储存当前socket的descriptor
};
