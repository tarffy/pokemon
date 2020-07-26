#pragma once

#include <QObject>

class Handler : public QObject
{
	Q_OBJECT

public:
	Handler(QObject *parent=NULL);
	~Handler();
	
public slots:
	void handle_str_from_socket(const QString &str);		//处理socket读到的string
signals:
	void register_or_login_success(const QString &str);		//收到服务端 注册或登录成功时激活
	void register_or_login_fail(const QString &str);		//失败时激活
	void query_success(const QString &str);
};
