#pragma once

//1级窗口 - 主页面

#include "Qt.h"
#include "GroupChatWidget.h"
#include <QTabWidget>

class MainWidget : public QWidget
{
	Q_OBJECT

public:
	MainWidget(QWidget *parent = Q_NULLPTR);
	~MainWidget();

private:

	//Tab窗口
	QTabWidget* mTabWidget;

	//页面
	GroupChatWidget* mGroupChat; //群聊页面

	//布局
	QVBoxLayout* Layout;
};
