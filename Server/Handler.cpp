#include "Handler.h"

Handler::Handler(QObject *parent)
	: QObject(parent)
{
}

Handler::~Handler()
{
}

void Handler::get_string_from_socket(const QString & str)
{
	QString res = str + QString(" desu");
	emit string_to_socket_ready(res);
}
