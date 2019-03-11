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

	QVector<QPair<QString, bool>>chatwindow;
	QVector<QPair<QString, QLabel*>>OnOfflineLabe;

	//�Ի�����
	QVector<ChatWidget*>dialog;

	//֮ǰ���ӵ��׽���
	ClientSocket* socket;


signals:
	void userquit();
};
