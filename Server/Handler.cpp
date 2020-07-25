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
		if (list.at(1).length() < 6 || list.at(2).length() < 6) {	//冗余设置
			emit string_to_socket_ready(QString("login****fail****用户名或密码长度不足6位，请重新输入。"), 1);
			return;
		}
		res = stmt->executeQuery(QString("select * from users where user_name='%1'\0").arg(list.at(1)).toUtf8().data());
		if (res->next()) {
			QString password = res->getString("password").c_str();
			if (password == list.at(2)) {
				emit string_to_socket_ready(QString("user %1 login success").arg(list.at(1)), 2);
				emit string_to_socket_ready(QString("login****success****登录成功"), 1);
			}
			else {
				emit string_to_socket_ready("login failed wrong password", 2);
				emit string_to_socket_ready(QString("login****fail****密码错误，请检查后重新登录。"), 1);
			}
		}
		else {
			emit string_to_socket_ready("login failed", 2);
			emit string_to_socket_ready(QString("login****fail****用户名%1未被注册，请先注册用户。").arg(list.at(1)), 1);
		}

	}
	else if (mode == "register") {
		if (list.at(1).length() < 6 || list.at(2).length() < 6) {	//冗余设置
			emit string_to_socket_ready(QString("register****fail****用户名或密码长度不足6位，请重新输入。"), 1);
			return;
		}
		res = stmt->executeQuery(QString("select * from users where user_name='%1'\0").arg(list.at(1)).toUtf8().data());
		if (res->next()) {
			emit string_to_socket_ready("register failed",2);
			emit string_to_socket_ready(QString("register****fail****用户名%1已被注册，请换用户名重新注册。").arg(list.at(1)), 1);
		}
		else {
			stmt->executeUpdate(QString("insert into users (user_name,password) values ('%1','%2')").arg(list.at(1)).arg(list.at(2)).toUtf8().data());
			emit string_to_socket_ready(QString("user %1 register success").arg(list.at(1)), 2);
			emit string_to_socket_ready(QString("register****success****恭喜您注册成功"),1);
		}
	}


}
