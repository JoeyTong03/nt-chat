#pragma once

#include "Qt.h"
#include "Image_TextEdit.h"
#include "ClientSocket.h"

#include "FileOp.h"

class Chat_SendMessageWidget : public QWidget
{
	Q_OBJECT

public:
	Chat_SendMessageWidget(QString _dest,ClientSocket* _socket, QWidget *parent = Q_NULLPTR);
	~Chat_SendMessageWidget();

	QString GetMessagecs();

	void SendFileHead();
	void SendFileBody();

	void DownLoad();

private:

	//����ͼ���ļ�
	const QString ExpressionIcon_FileAddr = "Resources//1-expression.ico";
	const QString FileIcon_FileAddr = "Resources//2-file.ico";

	/* QT������ - ���飬�ļ����� */
	QPushButton* tExpression_PushButton;	//���ͱ��鰴ť
	QPushButton* tFile_PushButton;			//�����ļ���ť
	QPushButton* tDownload;//���ذ�ť
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

	QString dest;

	//�׽���
	ClientSocket* socket;

	MyFile file;

	QString dnuser;
	QString dnfile;

	void GetDownLoad(QString _user,QString _file);

	friend class ChatWidget;
};
