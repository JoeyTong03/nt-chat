#pragma once

#include <QMovie>
#include <QLabel>
#include <QWidget>
#include <QHBoxLayout>
#include "Image_TextEdit.h"

class Icon_Widget : public QWidget
{
	Q_OBJECT

public:
	Icon_Widget(
		int _LEFT_MARGIN,	//left
		int _TOP_MARGIN,	//top
		int _ICON_SIZE,		//icon size
		QString _ico_addr,	//头像文件地址
		QWidget *parent = Q_NULLPTR
	);

	~Icon_Widget();

private:
	void paintEvent(QPaintEvent* event);

	int LEFT_MARGIN;	//left
	int TOP_MARGIN;		//top
	int ICON_SIZE;		//Width

	QString ico_addr;	//头像文件地址

};

class Chat_MessageWidget : public QWidget
{
	Q_OBJECT

public:

	//消息类型
	enum MessageType
	{
		MTSystem,	//系统用户
		MTMe,		//我自己
		MTOthers,	//其他用户
		MTTime		//时间戳
	};

	Chat_MessageWidget(
		MessageType _mt,
		QString _username = "",
		QString _text = "",
		QWidget *parent = Q_NULLPTR
	);
	~Chat_MessageWidget();

	QSize GetSize();

	void MessageArrive();


private:

	MessageType mt;	//这个消息的类型

	//头像框
	Icon_Widget* IconWidget;

	//用户名
	QLabel* UsernameLabel;

	//等待消息框
	QLabel* WaitImageLabel;

	//文本框
	QLabel* TextEdit;

	//文本框
	QVBoxLayout* TextLayout;

	//总布局
	QHBoxLayout* Layout;
	
	//时间戳
	QLabel* TimeLabel;

	int Height;
	int Width;

	//参数常量
	const int LEFT_MARGIN = 20;		//消息与左右边框的间距
	const int TOP_MARGIN = 10;		//消息与上下文之间的间距
	const int ICON_SIZE = 40;		//头像的大小(正方形 - 宽、高)
	const int ICONMESSAGE_GIP = 10;	//头像与消息之间的间隔
	const int USERNAME_HEIGHT = 10;	//用户名的高度
	const int USERNAME_FONTSIZE = 8;//用户名字体

	//表情图标文件
	const QString WaitGif_Addr = "Resources//5-loading.gif";
	const QString MyIcon_Addr = "Resources//4-me.png";
	const QString OtherIcon_Addr = "Resources//3-other.png";

	//消息
	QString text = "这是一个消息";
	QString username = "yxy";

	//字体
	int Text_FontSize = 12;					//字体大小
	QString Text_FontFamily = "微软雅黑";	//字体

};
