#pragma once

#include <QQueue>
#include <QWidget>
#include <QDateTime>
#include <QListWidget>
#include <QVBoxLayout>
#include "GroupChat_MessageWidget.h"

class GroupChat_ShowMessageWidget : public QWidget
{
	Q_OBJECT

public:
	GroupChat_ShowMessageWidget(QWidget *parent = Q_NULLPTR);
	~GroupChat_ShowMessageWidget();

	//����һ����Ϣ
	void Add_Message(QString _username,QString _text,GroupChat_MessageWidget::MessageType _mt);

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
	QQueue<GroupChat_MessageWidget*> MessageQueue;

	//��ǰ��Ϣ�ĸ���
	int MessageNum = 0;


};
