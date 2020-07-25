#pragma once

#include <qtcpserver.h>
#include <qthread.h>
#include <vector>
#include "TCPsocket.h"
using std::vector;
class TCPserver : public QTcpServer
{
	Q_OBJECT

public:
	TCPserver(QObject *parent=NULL);
	~TCPserver();
	void incomingConnection(qintptr socketDescriptor);	//重写QTcpServer函数 当有新连接是会被调用

public slots:
	void handle_string_from_socket(const QString &str);		//处理socket传给server的str 目前功能有 disconnect时删除hash 输出socket传给server的debug信息
signals:
	void socket_init(const qintptr socketDescriptor);		//用于将新建的TCPsocket移动到新线程后 在新线程初始化成员
private:
	QHash<qintptr, QThread*> threads;			//Hash表 存储每个socket链接使用的QThread指针
};
