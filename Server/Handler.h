#pragma once

#include <QObject>

class Handler : public QObject
{
	Q_OBJECT

public:
	Handler(QObject *parent=NULL);
	~Handler();
	
public slots:
	//��socket����handler��str���д��� ����󴥷�string_to_socket_ready�ź�
	void get_string_from_socket(const QString &str);	
signals:
	void string_to_socket_ready(const QString &str);	
};
