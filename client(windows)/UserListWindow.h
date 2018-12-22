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

	/* Qt 控件 */

	//用户列表表头
	UserListHeadWidget* ulUserListHead;

	//用户列表
	QGroupBox* ulUserListGroupBox;
	QVBoxLayout* ulUserListGroupBoxLayout;

	//用户项
	QListWidget* ulUserListListWidget;

	//Qt 布局
	QVBoxLayout* Layout;

	QString username;

	const int ImageSize = 30;
	const char* HeadImageAddr = "Resources//3-other.png";

	const int UsernameFontSize = 20;
	const int SignatureFontSize = 10;
};
