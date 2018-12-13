
#include "GroupChat_GroupMemberWidget.h"

#include <QHeaderView>

GroupChat_GroupMemberWidget::GroupChat_GroupMemberWidget(QWidget *parent)
	: QWidget(parent)
{
	// Qt 类型初始化
	{
		gGroupMember_GroupBox = new QGroupBox(tr("在线群成员"));
		gGroupMember_VLayout = new QVBoxLayout;
		
		//表格表示当前在线群成员 - 头像/用户名/ip
		gGroupMember_TableWidget = new QTableWidget(0, 2);
		gGroupMember_TableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //表格禁止编辑
		QStringList header;
		header << "用户名"<<"ip";
		gGroupMember_TableWidget->setHorizontalHeaderLabels(header);
		gGroupMember_TableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);	//宽度自适应
		//gGroupMember_TableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);//对第0列单独设置固定宽度
		//gGroupMember_TableWidget->setColumnWidth(0, 45);											//头像列设置固定宽度
		gGroupMember_TableWidget->verticalHeader()->setDefaultSectionSize(20);						//默认行高

		gGroupMember_VLayout->addWidget(gGroupMember_TableWidget);
		gGroupMember_GroupBox->setLayout(gGroupMember_VLayout);

		//总布局
		Layout = new QVBoxLayout(this);
		Layout->addWidget(gGroupMember_GroupBox);
	}
}

GroupChat_GroupMemberWidget::~GroupChat_GroupMemberWidget()
{

}

//#define TEST_GROUPCHAT_SENDMESSAGE
#ifdef TEST_GROUPCHAT_SENDMESSAGE

#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	GroupChat_GroupMemberWidget test;
	test.show();

	return a.exec();
}


#endif // TEST_GROUPCHAT_SENDMESSAGE


