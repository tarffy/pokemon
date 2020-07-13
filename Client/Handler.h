#pragma once

#include <QObject>

class Handler : public QObject
{
	Q_OBJECT

public:
	Handler(QObject *parent=NULL);
	~Handler();
	
public slots:
	void handle_str_from_socket(const QString &str);		//����socket������string
signals:
	void register_or_login_success(const QString &str);		//�յ������ ע����¼�ɹ�ʱ����
	void register_or_login_fail(const QString &str);		//ʧ��ʱ����
};
