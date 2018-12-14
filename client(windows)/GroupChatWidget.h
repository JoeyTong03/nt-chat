#pragma once

//2������ - ���촰��

#include "Qt.h"

#include "GroupChat_ShowMessageWidget.h"
#include "GroupChat_SendMessageWidget.h"
#include "GroupChat_GroupMemberWidget.h"


class GroupChatWidget : public QWidget
{
	Q_OBJECT

public:
	GroupChatWidget(QWidget *parent = Q_NULLPTR);
	~GroupChatWidget();

private:

	/* QT ���� */
	void SendMessage();

	/* �������� */
	GroupChat_GroupMemberWidget* gGroupMember; //��ǰȺ��Ա
	GroupChat_SendMessageWidget* gSendMessage; //������Ϣ����
	GroupChat_ShowMessageWidget* gShowMessgae; //��Ϣ��ʾ����

	/* Qt���� */
	QVBoxLayout* gLeftLayout;

	QHBoxLayout* Layout;

	/* ���� */
	QString username = "MiaoMiaoYang";

};
