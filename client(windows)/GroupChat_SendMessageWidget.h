#pragma once

#include "Qt.h"
#include "Image_TextEdit.h"


class GroupChat_SendMessageWidget : public QWidget
{
	Q_OBJECT

public:
	GroupChat_SendMessageWidget(QWidget *parent = Q_NULLPTR);
	~GroupChat_SendMessageWidget();

	QString GetMessage();

private:

	//表情图标文件
	const QString ExpressionIcon_FileAddr = "Resources//1-expression.ico";
	const QString FileIcon_FileAddr = "Resources//2-file.ico";

	/* QT工具栏 - 表情，文件…… */
	QPushButton* tExpression_PushButton;	//发送表情按钮
	QPushButton* tFile_PushButton;			//传输文件按钮
	QHBoxLayout* tToolBar_HLayout;			//工具栏布局

	/* 发送文字框 */
	//QTextEdit* SendText_TextEdit;		//输入发送文字框
	Image_TextEdit* SendText_TextEdit;	//输入发送文字框

	/* 确认按钮/取消 */
	QPushButton* cCertify_PushButton;	//确认按钮
	QPushButton* cCancel_PushButton;	//取消按钮
	QHBoxLayout* cState_HLayout;		//确认栏布局

	//总布局
	QVBoxLayout* Layout;

	friend class GroupChatWidget;
};
