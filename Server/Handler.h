#pragma once
#pragma execution_character_set("utf-8")
#include <QObject>
#include "jdbc/mysql_connection.h"
#include "jdbc/mysql_driver.h"
#include "jdbc/cppconn/statement.h"
class Handler : public QObject
{
	Q_OBJECT

public:
	Handler(QObject *parent=NULL);
	~Handler();
private:
	sql::mysql::MySQL_Driver *driver;		//mysql驱动
	sql::Connection *con;					//连接
	sql::Statement *stmt;					//语句
	sql::ResultSet *res;					//结果集
public slots:
	//将socket传给handler的str进行处理 处理后触发string_to_socket_ready信号
	void get_string_from_socket(const QString &str);	
signals:
	void string_to_socket_ready(const QString &str,int i);		//将string发给socket i为1发给peer socket 为2发给server debug
};
