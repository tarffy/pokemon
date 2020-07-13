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
	sql::mysql::MySQL_Driver *driver;		//mysql����
	sql::Connection *con;					//����
	sql::Statement *stmt;					//���
	sql::ResultSet *res;					//�����
public slots:
	//��socket����handler��str���д��� ����󴥷�string_to_socket_ready�ź�
	void get_string_from_socket(const QString &str);	
signals:
	void string_to_socket_ready(const QString &str,int i);		//��string����socket iΪ1����peer socket Ϊ2����server debug
};
