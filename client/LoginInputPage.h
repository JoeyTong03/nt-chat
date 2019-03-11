#pragma once

#pragma execution_character_set("utf-8")

#include <QLabel>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

class LoginInputPage : public QWidget
{
	Q_OBJECT

public:
	LoginInputPage(QWidget *parent = Q_NULLPTR);
	~LoginInputPage();

	//返回用户名
	QString GetUsername() { return UsernameLineEdit->text(); };
	
	//返回口令
	QString GetKeyword() { return KeywordLineEdit->text(); };
	
	//显示错误提示
	void ShowErrorMessage(QString _message);

private:

	/* QT部件 */

	//用户名
	QLabel* UsernameLabel;
	QLineEdit* UsernameLineEdit;
	QHBoxLayout* UsernameLayout;

	//密码
	QLabel* KeywordLabel;
	QLineEdit* KeywordLineEdit;
	QHBoxLayout* KeywordLayout;
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

private slots:
	void LoginPushButton_Slots(); //登录按钮触发槽函数

signals:
	void Signal_Login(QString _username, QString _keyword);

};
