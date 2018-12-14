#include "GroupChat_ShowMessageWidget.h"

GroupChat_ShowMessageWidget::GroupChat_ShowMessageWidget(QWidget *parent)
	: QWidget(parent)
{

	Message_ListWidget = new QListWidget;

	Layout = new QVBoxLayout(this);
	Layout->addWidget(Message_ListWidget);

	MessageNum = 0;
}

GroupChat_ShowMessageWidget::~GroupChat_ShowMessageWidget()
{

}

void GroupChat_ShowMessageWidget::Add_Message(QString _username, QString _text, GroupChat_MessageWidget::MessageType _mt)
{
	if (MessageNum == 0)
	{
		//会话框初始化
		GroupChat_MessageWidget* TimeMessage = new GroupChat_MessageWidget(GroupChat_MessageWidget::MTTime);
		QListWidgetItem* TimeMessageItem = new QListWidgetItem;
		TimeMessageItem->setSizeHint(TimeMessage->GetSize());
		Message_ListWidget->addItem(TimeMessageItem);
		Message_ListWidget->setItemWidget(TimeMessageItem, TimeMessage);

		MessageQueue.push_back(TimeMessage);

		//开启计时器
		BeginTime = QDateTime::currentDateTime();

		GroupChat_MessageWidget* DateMessage = new GroupChat_MessageWidget(_mt, _username, _text);
		QListWidgetItem* MessageItem = new QListWidgetItem;
		MessageItem->setSizeHint(DateMessage->GetSize());
		Message_ListWidget->addItem(MessageItem);
		Message_ListWidget->setItemWidget(MessageItem, DateMessage);

	}
	else
	{
		//观察时间戳是否超时
		EndTime = QDateTime::currentDateTime();
		int IntervalSec = BeginTime.secsTo(EndTime);

		//与上条消息超过5min - 显示时间戳
		if (IntervalSec > Interval)
		{
			GroupChat_MessageWidget* TimeMessage = new GroupChat_MessageWidget(GroupChat_MessageWidget::MTTime);
			QListWidgetItem* TimeMessageItem = new QListWidgetItem;
			TimeMessageItem->setSizeHint(TimeMessage->GetSize());
			Message_ListWidget->addItem(TimeMessageItem);
			Message_ListWidget->setItemWidget(TimeMessageItem, TimeMessage);

			MessageQueue.push_back(TimeMessage);
		}

		//更新开始时间
		BeginTime = QDateTime::currentDateTime();

		GroupChat_MessageWidget* DateMessage = new GroupChat_MessageWidget(_mt, _username, _text);
		QListWidgetItem* MessageItem = new QListWidgetItem;
		MessageItem->setSizeHint(DateMessage->GetSize());
		Message_ListWidget->addItem(MessageItem);
		Message_ListWidget->setItemWidget(MessageItem, DateMessage);

		MessageQueue.push_back(DateMessage);
	}

	MessageNum++;
}