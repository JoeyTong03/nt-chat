
#include "GroupChat_SendMessageWidget.h"

GroupChat_SendMessageWidget::GroupChat_SendMessageWidget(QWidget *parent)
	: QWidget(parent)
{
	//QT 类型初始化	
	{
		/* QT工具栏 - 表情，文件…… */
		tExpression_PushButton = new QPushButton;	//发送表情按钮
		QIcon tExpression_Icon;
		tExpression_Icon.addFile(ExpressionIcon_FileAddr);
		tExpression_PushButton->setIcon(tExpression_Icon);

		tFile_PushButton = new QPushButton;			//传输文件按钮
		QIcon tFile_Icon;
		tFile_Icon.addFile(FileIcon_FileAddr);
		tFile_PushButton->setIcon(tFile_Icon);
		
		tToolBar_HLayout = new QHBoxLayout;			//工具栏布局
		tToolBar_HLayout->addWidget(tExpression_PushButton);
		tToolBar_HLayout->addWidget(tFile_PushButton);
		tToolBar_HLayout->setAlignment(Qt::AlignLeft);
		
		/* 发送文字框 */
		//SendText_TextEdit = new QTextEdit;			//输入发送文字框
		SendText_TextEdit = new Image_TextEdit;

		/* 确认按钮/取消 */
		cCancel_PushButton = new QPushButton(tr("取消"));	//取消按钮
		cCertify_PushButton = new QPushButton(tr("发送"));	//确认按钮
		cState_HLayout = new QHBoxLayout;					//确认栏布局
		cState_HLayout->addWidget(cCertify_PushButton);
		cState_HLayout->addWidget(cCancel_PushButton);
		cState_HLayout->setAlignment(Qt::AlignRight);

		//总布局
		Layout = new QVBoxLayout(this);
		Layout->addLayout(tToolBar_HLayout, 1);
		Layout->addWidget(SendText_TextEdit, 3);
		Layout->addLayout(cState_HLayout, 1);
	}

}

GroupChat_SendMessageWidget::~GroupChat_SendMessageWidget()
{

}

//得到了文本框的内容
QString GroupChat_SendMessageWidget::GetMessage()
{
	QString tmp = SendText_TextEdit->toHtml();
	SendText_TextEdit->clear();
	return tmp;
}


//#define TEST_GROUPCHAT_SENDMESSAGE
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
