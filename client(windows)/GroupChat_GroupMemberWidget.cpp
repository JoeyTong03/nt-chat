
#include "GroupChat_GroupMemberWidget.h"

#include <QHeaderView>

GroupChat_GroupMemberWidget::GroupChat_GroupMemberWidget(QWidget *parent)
	: QWidget(parent)
{
	// Qt ���ͳ�ʼ��
	{
		gGroupMember_GroupBox = new QGroupBox(tr("����Ⱥ��Ա"));
		gGroupMember_VLayout = new QVBoxLayout;
		
		//����ʾ��ǰ����Ⱥ��Ա - ͷ��/�û���/ip
		gGroupMember_TableWidget = new QTableWidget(0, 2);
		gGroupMember_TableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //����ֹ�༭
		QStringList header;
		header << "�û���"<<"ip";
		gGroupMember_TableWidget->setHorizontalHeaderLabels(header);
		gGroupMember_TableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);	//�������Ӧ
		//gGroupMember_TableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);//�Ե�0�е������ù̶����
		//gGroupMember_TableWidget->setColumnWidth(0, 45);											//ͷ�������ù̶����
		gGroupMember_TableWidget->verticalHeader()->setDefaultSectionSize(20);						//Ĭ���и�

		gGroupMember_VLayout->addWidget(gGroupMember_TableWidget);
		gGroupMember_GroupBox->setLayout(gGroupMember_VLayout);

		//�ܲ���
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


