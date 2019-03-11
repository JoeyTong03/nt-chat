#pragma once

#include <QQueue>
#include <QWidget>
#include <QDateTime>
#include <QListWidget>
#include <QVBoxLayout>
#include "Chat_MessageWidget.h"

class Chat_ShowMessageWidget : public QWidget
{
	Q_OBJECT

public:
	Chat_ShowMessageWidget(QWidget *parent = Q_NULLPTR);
	~Chat_ShowMessageWidget();

	//增加一个消息
	void Add_Message(QString _username,QString _text,Chat_MessageWidget::MessageType _mt);

private:

	//消息窗口
	QListWidget* Message_ListWidget;

	//总布局
	QVBoxLayout* Layout;

	//计时器
	QDateTime BeginTime;
	QDateTime EndTime;
	int Interval = 60 * 5; //5min间隔

	//消息队列
	QQueue<Chat_MessageWidget*> MessageQueue;

	//当前消息的个数
	int MessageNum = 0;

	friend class MainWidget;
};
