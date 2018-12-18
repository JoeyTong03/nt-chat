#pragma once

//1������ - ��ҳ��

#include "ChangeKeywordWidget.h"
#include "GroupChatWidget.h"
#include "ClientSocket.h"
#include "LoginWidget.h"
#include "Qt.h"


#include <QTabWidget>
#include <QStackedWidget>


class MainWidget : public QWidget
{
	Q_OBJECT

public:
	MainWidget(QWidget *parent = Q_NULLPTR);
	~MainWidget();

private:

	//���ҳ��
	QStackedWidget* mStackedWidget;

	//Tab����
	QTabWidget* mTabWidget;

	//ҳ��
	LoginWidget* mLogin;				//��½����
	ChangeKeywordWidget* mChangeKeyword;//���ܽ���
	GroupChatWidget* mGroupChat;		//Ⱥ��ҳ��

	//����
	QVBoxLayout* Layout;

	//Socket
	ClientSocket* socket;


//�źŲۺ���
//private slots:
	void LoginSlots();		//��½����
	void GetInfoSlots();	//�õ����������

};
