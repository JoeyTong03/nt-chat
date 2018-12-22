#pragma once

#include "UserListHeadWidget.h"
#include <QGroupBox>
#include <QListWidget>
#include <QDialog>

class UserListWindow : public QDialog
{
	Q_OBJECT

public:
	UserListWindow(QString _username,QWidget *parent = Q_NULLPTR);
	~UserListWindow();

	void AddOneUser(QString _user);


private:

	/* Qt �ؼ� */

	//�û��б��ͷ
	UserListHeadWidget* ulUserListHead;

	//�û��б�
	QGroupBox* ulUserListGroupBox;
	QVBoxLayout* ulUserListGroupBoxLayout;

	//�û���
	QListWidget* ulUserListListWidget;

	//Qt ����
	QVBoxLayout* Layout;

	QString username;

	const int ImageSize = 30;
	const char* HeadImageAddr = "Resources//3-other.png";

	const int UsernameFontSize = 20;
	const int SignatureFontSize = 10;
};
