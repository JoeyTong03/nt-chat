
#include "GroupChatWidget.h"


GroupChatWidget::GroupChatWidget(QWidget *parent)
	: QWidget(parent)
{
	//QT空间及布局
	{
		gGroupMember = new GroupChat_GroupMemberWidget;
		gSendMessage = new GroupChat_SendMessageWidget;
		gShowMessgae = new GroupChat_ShowMessageWidget;

		gLeftLayout = new QVBoxLayout;
		gLeftLayout->addWidget(gShowMessgae,2);
		gLeftLayout->addWidget(gSendMessage,1);

		Layout = new QHBoxLayout(this);
		Layout->addLayout(gLeftLayout,5);
		Layout->addWidget(gGroupMember,2);
	}
}

GroupChatWidget::~GroupChatWidget()
{

}

//#define TEST_GROUPCHAT
#ifdef TEST_GROUPCHAT
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	GroupChatWidget test;
	test.show();

	return a.exec();
}

#endif // TEST_GROUPCHAT
