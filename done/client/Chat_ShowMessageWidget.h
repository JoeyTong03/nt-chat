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

	//����һ����Ϣ
	void Add_Message(QString _username,QString _text,Chat_MessageWidget::MessageType _mt);

private:

	//��Ϣ����
	QListWidget* Message_ListWidget;

	//�ܲ���
	QVBoxLayout* Layout;

	//��ʱ��
	QDateTime BeginTime;
	QDateTime EndTime;
	int Interval = 60 * 5; //5min���

	//��Ϣ����
	QQueue<Chat_MessageWidget*> MessageQueue;

	//��ǰ��Ϣ�ĸ���
	int MessageNum = 0;

	friend class MainWidget;
};
