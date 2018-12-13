#pragma once

//1������ - ��ҳ��

#include "Qt.h"
#include "GroupChatWidget.h"
#include <QTabWidget>

class MainWidget : public QWidget
{
	Q_OBJECT

public:
	MainWidget(QWidget *parent = Q_NULLPTR);
	~MainWidget();

private:

	//Tab����
	QTabWidget* mTabWidget;

	//ҳ��
	GroupChatWidget* mGroupChat; //Ⱥ��ҳ��

	//����
	QVBoxLayout* Layout;
};
