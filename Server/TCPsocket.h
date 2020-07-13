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
	void string_to_handler_ready(const QString &str);	//socket read���str����handler����
	void string_to_server_ready(const QString &str);	//socket ��str����server Ŀǰ����disconnect
public slots:
	void socket_init(const qintptr socketDescriptor);	//��ʼ��socket��handler����
	void string_to_handler();							//��socket�յ����ַ�������handler ����
	void string_from_handler(const QString &str,int i);		//��handler�������str���ظ�socket 
	void tell_server_disconnected();					//��disconnected��Ϣ����server
private:
	QTcpSocket *socket;				//socket
	Handler *handler;				//������Ϣ��handler
	qintptr socketdescriptor;		//���浱ǰsocket��descriptor
};
