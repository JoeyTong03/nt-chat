#pragma once

#pragma execution_character_set("utf-8")


#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QLabel>

class UserListHeadWidget : public QWidget
{
	Q_OBJECT

public:
	UserListHeadWidget(QString _username,QWidget *parent = Q_NULLPTR);
	~UserListHeadWidget();

private:

	/* Qt �ؼ� */
	QLabel* HeadImageLabel;

	QLabel* Username;	//�û���
	QLabel* Signature;	//����ǩ��
	QVBoxLayout* UserInfoLayout;

	QHBoxLayout* Layout;
	
	const int ImageSize = 80;
	const char* HeadImageAddr = "Resources//7-headimage.jpg";
	
	const int UsernameFontSize = 20;
	const int SignatureFontSize = 10;

};
