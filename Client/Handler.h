﻿#pragma once

#include <QObject>
#include "Player.h"
class Handler : public QObject
{
	Q_OBJECT	
public:
	Handler(QObject *parent=NULL);
	Player player;
	~Handler();
	pokemon_base *string_to_pokemon(const QString &str);
public slots:
	void handle_str_from_socket(const QString &str);		//处理socket读到的string
signals:
	void register_or_login_success(const QString &str);		//收到服务端 注册或登录成功时激活
	void register_or_login_fail(const QString &str);		//失败时激活
	void query_success(const QString &str);					//查询成功 str为###分割的用户名
	void socket_information(const QString &str);			//收到socket发来的信息 以massagebox形式展示
	void pokemon_info_ready(const QString &str);			//查询得到背包和仓库信息准备好
	void player_pokemon_ready(const QString &str);
};
