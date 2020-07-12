#pragma once

#include <QObject>

class Handler : public QObject
{
	Q_OBJECT

public:
	Handler(QObject *parent=NULL);
	~Handler();
	
public slots:
	//将socket传给handler的str进行处理 处理后触发string_to_socket_ready信号
	void get_string_from_socket(const QString &str);	
signals:
	void string_to_socket_ready(const QString &str);	
};
