#pragma once

#include "LoginWindow.h"
#include "UserListWindow.h"
#include "ClientSocket.h"

class TopWidget:public QWidget
{
	Q_OBJECT

public:
	TopWidget();
	~TopWidget();

private:

	/* QT �Ự���� */
	LoginWindow* tLoginWindow; //��½�Ự����

	/* QT �û��б� */
	UserListWindow* tUserListWindow;

	/* Qt �׽��� */
	ClientSocket* tClientSocket;

	QString ClientUser;

	void CreateUserList();

private slots:
	
	//��½���źŲۺ���
	void Login_Slots(QString _username, QString _keyword);
	
	//���յ���½���ص�ACK֡
	void LoginACK_Slots(char _type);

	//�����źŵĲۺ���
	void ChangeKeyword_Slots(QString _keyword);

	//���ܵ����ѳ�ʼ��֡
	void FriendInit(QVector<QString> _userlist);
};
