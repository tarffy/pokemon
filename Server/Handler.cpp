#include "Handler.h"
#include <string>
using std::string;
Handler::Handler(QObject *parent)
	: QObject(parent)
{
	driver = sql::mysql::get_mysql_driver_instance();
	if (driver == NULL) {
		emit string_to_socket_ready(QString("Driver failed"), 2);
	}
	con = driver->connect("tcp://localhost:3306", "root", "123456");
	if (con == NULL) {
		emit string_to_socket_ready(QString("Connect to sql failed"), 2);
	}
	stmt = con->createStatement();
	stmt->execute("set character set gbk");
	stmt->execute("use pokemon_database");
	emit string_to_socket_ready(QString("SQL connect success"), 2);
}

Handler::~Handler()
{
}

void Handler::get_string_from_socket(const QString & str)
{
	QStringList list = str.split("****");
	QString mode = list.at(0);
	if (mode == "login") {
		if (list.at(1).length() < 6 || list.at(2).length() < 6) {	//��������
			emit string_to_socket_ready(QString("login****fail****�û��������볤�Ȳ���6λ�����������롣"), 1);
			return;
		}
		res = stmt->executeQuery(QString("select * from users where user_name='%1'\0").arg(list.at(1)).toUtf8().data());
		if (res->next()) {
			QString password = res->getString("password").c_str();
			if (password == list.at(2)) {
				emit string_to_socket_ready(QString("user %1 login success").arg(list.at(1)), 2);
				emit string_to_socket_ready(QString("login****success****��¼�ɹ�"), 1);
			}
			else {
				emit string_to_socket_ready("login failed wrong password", 2);
				emit string_to_socket_ready(QString("login****fail****���������������µ�¼��").arg(list.at(1)), 1);
			}
		}
		else {
			emit string_to_socket_ready("login failed", 2);
			emit string_to_socket_ready(QString("login****fail****�û���%1δ��ע�ᣬ����ע���û���").arg(list.at(1)), 1);
		}

	}
	else if (mode == "register") {
		if (list.at(1).length() < 6 || list.at(2).length() < 6) {	//��������
			emit string_to_socket_ready(QString("register****fail****�û��������볤�Ȳ���6λ�����������롣"), 1);
			return;
		}
		res = stmt->executeQuery(QString("select * from users where user_name='%1'\0").arg(list.at(1)).toUtf8().data());
		if (res->next()) {
			emit string_to_socket_ready("register failed",2);
			emit string_to_socket_ready(QString("register****fail****�û���%1�ѱ�ע�ᣬ�뻻�û�������ע�ᡣ").arg(list.at(1)), 1);
		}
		else {
			stmt->executeUpdate(QString("insert into users (user_name,password) values ('%1','%2')").arg(list.at(1)).arg(list.at(2)).toUtf8().data());
			emit string_to_socket_ready(QString("user %1 register success").arg(list.at(1)), 2);
			emit string_to_socket_ready(QString("register****success****��ϲ��ע��ɹ�"),1);
		}
	}


}
