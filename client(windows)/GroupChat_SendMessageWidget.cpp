
#include "GroupChat_SendMessageWidget.h"

GroupChat_SendMessageWidget::GroupChat_SendMessageWidget(QWidget *parent)
	: QWidget(parent)
{
	//QT ���ͳ�ʼ��	
	{
		/* QT������ - ���飬�ļ����� */
		tExpression_PushButton = new QPushButton;	//���ͱ��鰴ť
		QIcon tExpression_Icon;
		tExpression_Icon.addFile(ExpressionIcon_FileAddr);
		tExpression_PushButton->setIcon(tExpression_Icon);

		tFile_PushButton = new QPushButton;			//�����ļ���ť
		QIcon tFile_Icon;
		tFile_Icon.addFile(FileIcon_FileAddr);
		tFile_PushButton->setIcon(tFile_Icon);
		
		tToolBar_HLayout = new QHBoxLayout;			//����������
		tToolBar_HLayout->addWidget(tExpression_PushButton);
		tToolBar_HLayout->addWidget(tFile_PushButton);
		tToolBar_HLayout->setAlignment(Qt::AlignLeft);
		
		/* �������ֿ� */
		//SendText_TextEdit = new QTextEdit;			//���뷢�����ֿ�
		SendText_TextEdit = new Image_TextEdit;

		/* ȷ�ϰ�ť/ȡ�� */
		cCancel_PushButton = new QPushButton(tr("ȡ��"));	//ȡ����ť
		cCertify_PushButton = new QPushButton(tr("����"));	//ȷ�ϰ�ť
		cState_HLayout = new QHBoxLayout;					//ȷ��������
		cState_HLayout->addWidget(cCertify_PushButton);
		cState_HLayout->addWidget(cCancel_PushButton);
		cState_HLayout->setAlignment(Qt::AlignRight);

		//�ܲ���
		Layout = new QVBoxLayout(this);
		Layout->addLayout(tToolBar_HLayout, 1);
		Layout->addWidget(SendText_TextEdit, 3);
		Layout->addLayout(cState_HLayout, 1);
	}

}

GroupChat_SendMessageWidget::~GroupChat_SendMessageWidget()
{
}


#define TEST_GROUPCHAT_SENDMESSAGE
#ifdef TEST_GROUPCHAT_SENDMESSAGE

#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	GroupChat_SendMessageWidget test;
	test.show();

	return a.exec();
}

#endif // TEST_GROUPCHAT_SENDMESSAGE
