#include <QtCore/QCoreApplication>
#include "TCPserver.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
	TCPserver *server = new TCPserver();
	server->listen(QHostAddress::Any, 8888);
    return a.exec();
}
