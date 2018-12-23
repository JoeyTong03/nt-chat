#pragma once
#pragma execution_character_set("utf-8")

#include "LoginWindow.h"
#include "UserListWindow.h"
#include "ClientSocket.h"

class TopWidget:public QWidget
{
	Q_OBJECT

public:
	TopWidget(QApplication* a);
	~TopWidget();

private:

	/* QT 会话窗口 */
	LoginWindow* tLoginWindow; //登陆会话窗口

	/* QT 用户列表 */
	UserListWindow* tUserListWindow;

	/* Qt 套接字 */
	ClientSocket* tClientSocket;

	QString ClientUser;

	QApplication* app;

	void CreateUserList();
	
	void quit();

private slots:
	
	//登陆的信号槽函数
	void Login_Slots(QString _username, QString _keyword);
	
	//接收到登陆返回的ACK帧
	void LoginACK_Slots(char _type);

	//改密信号的槽函数
	void ChangeKeyword_Slots(QString _keyword);

	//接受到好友初始化帧
	void FriendInit(QVector<QString> _userlist);
};
