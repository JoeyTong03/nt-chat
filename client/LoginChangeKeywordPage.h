#pragma once

#pragma execution_character_set("utf-8")

#include <QLabel>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

class LoginChangeKeywordPage : public QWidget
{
	Q_OBJECT

public:
	LoginChangeKeywordPage(QWidget *parent = Q_NULLPTR);
	~LoginChangeKeywordPage();

private:

	/* QT部件 */

	//标题
	QLabel* TitleLabel;


	//旧密码
	QLabel* oKeywordLabel;
	QLineEdit* oKeywordLineEdit;
	QHBoxLayout* oKeywordLayout;

	//原密码
	QLabel* nKeywordLabel;
	QLineEdit* nKeywordLineEdit;
	QHBoxLayout* nKeywordLayout;

	QLineEdit::EchoMode KeywordMode = QLineEdit::Password;
	// PasswordEchoOnEdit;-输入时正常显示字符，输入完毕为隐藏字符

	//提示
	QLabel* HintLabel;
	QHBoxLayout* HintLayout;


	//确认按钮
	QPushButton* CertifyPushButton;

	//总布局
	QVBoxLayout* Layout;

	/* 参数 */
	const int UsernameMaxSize = 16; //用户名最长16字节
	const int KeywordMaxSize = 12;	//密码最长12字节

private slots:
	void CertifyPushButton_Slots();

signals:
	//发送新的密码
	void Signal_NewKeyword(QString _keyword);

};

