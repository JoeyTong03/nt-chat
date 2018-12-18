#pragma once

#pragma execution_character_set("utf-8")


#include <QLabel>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

class ChangeKeywordWidget : public QWidget
{
	Q_OBJECT

public:
	ChangeKeywordWidget(QWidget *parent = Q_NULLPTR);
	~ChangeKeywordWidget();

private:

	/* QT部件 */

	//用户名
	QLabel* UsernameLabel;
	QLineEdit* UsernameLineEdit;
	QHBoxLayout* UsernameLayout;

	//旧密码
	QLabel* oKeywordLabel;
	QLineEdit* oKeywordLineEdit;
	QHBoxLayout* oKeywordLayout;

	//新密码
	QLabel* nKeywordLabel;
	QLineEdit* nKeywordLineEdit;
	QHBoxLayout* nKeywordLayout;

	QLineEdit::EchoMode KeywordMode = QLineEdit::Password;
	// PasswordEchoOnEdit;-输入时正常显示字符，输入完毕为隐藏字符

	//登陆按钮
	QPushButton* LoginPushButton;

	//提示
	QLabel* HintLabel;
	QHBoxLayout* HintLayout;

	//总布局
	QVBoxLayout* Layout;

	/* 参数 */
	const int UsernameMaxSize = 16; //用户名最长16字节
	const int KeywordMaxSize = 12;	//密码最长12字节

	friend class MainWidget;

};
