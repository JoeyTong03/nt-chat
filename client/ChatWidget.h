#pragma once

//2������ - ���촰��

#include "Qt.h"
#include <QDialog>
#include "Chat_ShowMessageWidget.h"
#include "Chat_SendMessageWidget.h"
#include "ClientSocket.h"


class ChatWidget : public QDialog
{
	Q_OBJECT

public:
	ChatWidget(QString _destuser, QString _me, ClientSocket* _socket,QWidget *parent = Q_NULLPTR);
	~ChatWidget();

	void closeEvent(QCloseEvent* event);

	void RecvMessage(QString _text, QString _from);

private:

	/* QT ���� */
	void SendMessage();

	/* �������� */
	Chat_SendMessageWidget* gSendMessage; //������Ϣ����
	Chat_ShowMessageWidget* gShowMessgae; //��Ϣ��ʾ����

	/* Qt���� */
	QVBoxLayout* gLeftLayout;

	QHBoxLayout* Layout;

	/* ���� */
	QString MyUser;

	/* ������������ */
	QString dest_user;

	//�׽���
	ClientSocket* socket;

signals:
	//������Ϣ�ź�
	void CloseSignal(QString _user);
};
