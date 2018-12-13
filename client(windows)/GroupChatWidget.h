#pragma once

//2级窗口 - 聊天窗口

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

	/* 三个窗口 */
	GroupChat_GroupMemberWidget* gGroupMember; //当前群成员
	GroupChat_SendMessageWidget* gSendMessage; //发送消息窗口
	GroupChat_ShowMessageWidget* gShowMessgae; //消息显示窗口

	/* Qt布局 */
	QVBoxLayout* gLeftLayout;

	QHBoxLayout* Layout;

};
