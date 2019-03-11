#pragma once

//2级窗口 - 聊天窗口

#include "Qt.h"
#include <QDialog>
#include "Chat_ShowMessageWidget.h"
#include "Chat_SendMessageWidget.h"
#include "ClientSocket.h"


class ChatWidget : public QDialog
{
	Q_OBJECT

public:
	ChatWidget(QString _destuser, QString _me, ClientSocket* _socket,QWidget *parent = Q_NULLPTR);
	~ChatWidget();

	void closeEvent(QCloseEvent* event);

	void RecvMessage(QString _text, QString _from);

private:

	/* QT 动作 */
	void SendMessage();

	/* 三个窗口 */
	Chat_SendMessageWidget* gSendMessage; //发送消息窗口
	Chat_ShowMessageWidget* gShowMessgae; //消息显示窗口

	/* Qt布局 */
	QVBoxLayout* gLeftLayout;

	QHBoxLayout* Layout;

	/* 参数 */
	QString MyUser;

	/* 面向的聊天对象 */
	QString dest_user;

	//套接字
	ClientSocket* socket;

signals:
	//发送信息信号
	void CloseSignal(QString _user);
};
