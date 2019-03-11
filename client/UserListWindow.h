#pragma once

#include "ChatWidget.h"
#include "UserListHeadWidget.h"
#include <QGroupBox>
#include <QListWidget>
#include <QDialog>
#pragma execution_character_set("utf-8")

class UserListWindow : public QDialog
{
	Q_OBJECT

public:
	UserListWindow(QString _username, ClientSocket* _socket, QWidget *parent = Q_NULLPTR);
	~UserListWindow();

	void AddOneUser(QString _user);
	void ItemDoubleClicked(QListWidgetItem* item);

	ChatWidget* AddOneDialog(QString itemname);
	void DelOneDialog(QString _user);

	void FriendOnline(QString _user);
	void FriendOffline(QString _user);

	void GetMessageWithoutDialog(QString _text, QString _from);

	void closeEvent(QCloseEvent* event);


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

	QVector<QPair<QString, bool>>chatwindow;
	QVector<QPair<QString, QLabel*>>OnOfflineLabe;

	//对话窗口
	QVector<ChatWidget*>dialog;

	//之前连接的套接字
	ClientSocket* socket;


signals:
	void userquit();
};
