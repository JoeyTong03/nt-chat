#include "Chat_ShowMessageWidget.h"

Chat_ShowMessageWidget::Chat_ShowMessageWidget(QWidget *parent)
	: QWidget(parent)
{

	Message_ListWidget = new QListWidget;

	Layout = new QVBoxLayout(this);
	Layout->addWidget(Message_ListWidget);

	MessageNum = 0;

	this->setStyleSheet("background-color:transparent;");

}

Chat_ShowMessageWidget::~Chat_ShowMessageWidget()
{

}

void Chat_ShowMessageWidget::Add_Message(QString _username, QString _text, Chat_MessageWidget::MessageType _mt)
{
	//�Ƿ�Ϊ������Ϣ
	if (MessageNum == 0)
	{
		//�Ự���ʼ��
		Chat_MessageWidget* TimeMessage = new Chat_MessageWidget(Chat_MessageWidget::MTTime);
		QListWidgetItem* TimeMessageItem = new QListWidgetItem;
		TimeMessageItem->setSizeHint(TimeMessage->GetSize());
		Message_ListWidget->addItem(TimeMessageItem);
		Message_ListWidget->setItemWidget(TimeMessageItem, TimeMessage);

		MessageQueue.push_back(TimeMessage);

		//������ʱ��
		BeginTime = QDateTime::currentDateTime();

		Chat_MessageWidget* DateMessage = new Chat_MessageWidget(_mt, _username, _text);
		QListWidgetItem* MessageItem = new QListWidgetItem;
		MessageItem->setSizeHint(DateMessage->GetSize());
		Message_ListWidget->addItem(MessageItem);
		Message_ListWidget->setItemWidget(MessageItem, DateMessage);

	}
	else
	{
		//�۲�ʱ����Ƿ�ʱ
		EndTime = QDateTime::currentDateTime();
		int IntervalSec = BeginTime.secsTo(EndTime);

		//��������Ϣ����5min - ��ʾʱ���
		if (IntervalSec > Interval)
		{
			Chat_MessageWidget* TimeMessage = new Chat_MessageWidget(Chat_MessageWidget::MTTime);
			QListWidgetItem* TimeMessageItem = new QListWidgetItem;
			TimeMessageItem->setSizeHint(TimeMessage->GetSize());
			Message_ListWidget->addItem(TimeMessageItem);
			Message_ListWidget->setItemWidget(TimeMessageItem, TimeMessage);

			MessageQueue.push_back(TimeMessage);
		}

		//���¿�ʼʱ��
		BeginTime = QDateTime::currentDateTime();

		Chat_MessageWidget* DateMessage = new Chat_MessageWidget(_mt, _username, _text);
		QListWidgetItem* MessageItem = new QListWidgetItem;
		MessageItem->setSizeHint(DateMessage->GetSize());
		Message_ListWidget->addItem(MessageItem);
		Message_ListWidget->setItemWidget(MessageItem, DateMessage);

		MessageQueue.push_back(DateMessage);
	}

	MessageNum++;
}