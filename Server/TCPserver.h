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
	void incomingConnection(qintptr socketDescriptor);	//��дQTcpServer���� �����������ǻᱻ����

public slots:
	void handle_string_from_socket(const QString &str);		//����socket����server��str Ŀǰ������ disconnectʱɾ��hash ���socket����server��debug��Ϣ
signals:
	void socket_init(const qintptr socketDescriptor);		//���ڽ��½���TCPsocket�ƶ������̺߳� �����̳߳�ʼ����Ա
private:
	QHash<qintptr, QThread*> threads;			//Hash�� �洢ÿ��socket����ʹ�õ�QThreadָ��
};
