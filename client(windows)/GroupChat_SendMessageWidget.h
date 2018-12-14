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

	//����ͼ���ļ�
	const QString ExpressionIcon_FileAddr = "Resources//1-expression.ico";
	const QString FileIcon_FileAddr = "Resources//2-file.ico";

	/* QT������ - ���飬�ļ����� */
	QPushButton* tExpression_PushButton;	//���ͱ��鰴ť
	QPushButton* tFile_PushButton;			//�����ļ���ť
	QHBoxLayout* tToolBar_HLayout;			//����������

	/* �������ֿ� */
	//QTextEdit* SendText_TextEdit;		//���뷢�����ֿ�
	Image_TextEdit* SendText_TextEdit;	//���뷢�����ֿ�

	/* ȷ�ϰ�ť/ȡ�� */
	QPushButton* cCertify_PushButton;	//ȷ�ϰ�ť
	QPushButton* cCancel_PushButton;	//ȡ����ť
	QHBoxLayout* cState_HLayout;		//ȷ��������

	//�ܲ���
	QVBoxLayout* Layout;

	friend class GroupChatWidget;
};
