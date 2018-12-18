#pragma once

//1级窗口 - 主页面

#include "ChangeKeywordWidget.h"
#include "GroupChatWidget.h"
#include "ClientSocket.h"
#include "LoginWidget.h"
#include "Qt.h"


#include <QTabWidget>
#include <QStackedWidget>


class MainWidget : public QWidget
{
	Q_OBJECT

public:
	MainWidget(QWidget *parent = Q_NULLPTR);
	~MainWidget();

private:

	//多个页面
	QStackedWidget* mStackedWidget;

	//Tab窗口
	QTabWidget* mTabWidget;

	//页面
	LoginWidget* mLogin;				//登陆界面
	ChangeKeywordWidget* mChangeKeyword;//改密界面
	GroupChatWidget* mGroupChat;		//群聊页面

	//布局
	QVBoxLayout* Layout;

	//Socket
	ClientSocket* socket;


//信号槽函数
//private slots:
	void LoginSlots();		//登陆函数
	void GetInfoSlots();	//得到服务端数据

};
