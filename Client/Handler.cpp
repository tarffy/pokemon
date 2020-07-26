#include "Handler.h"
#include <qdebug.h>
Handler::Handler(QObject *parent)
	: QObject(parent)
{
}

Handler::~Handler()
{
}

void Handler::handle_str_from_socket(const QString & str)
{
	QStringList list = str.split("****");
	QString mode = list.at(0);
	if (mode == "register"||mode=="login") {
		if (list.at(1) == "success") {
			emit register_or_login_success(list.at(2));
		}
		else {
			emit register_or_login_fail(list.at(2));
		}
	}
	else if (mode == "query_player") {
		emit query_success(list.at(1));
	}
	
}
