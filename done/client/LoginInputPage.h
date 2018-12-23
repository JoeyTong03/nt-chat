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

	//�����û���
	QString GetUsername() { return UsernameLineEdit->text(); };
	
	//���ؿ���
	QString GetKeyword() { return KeywordLineEdit->text(); };
	
	//��ʾ������ʾ
	void ShowErrorMessage(QString _message);

private:

	/* QT���� */

	//�û���
	QLabel* UsernameLabel;
	QLineEdit* UsernameLineEdit;
	QHBoxLayout* UsernameLayout;

	//����
	QLabel* KeywordLabel;
	QLineEdit* KeywordLineEdit;
	QHBoxLayout* KeywordLayout;
	QLineEdit::EchoMode KeywordMode = QLineEdit::Password;
	// PasswordEchoOnEdit;-����ʱ������ʾ�ַ����������Ϊ�����ַ�

	//��½��ť
	QPushButton* LoginPushButton;

	//��ʾ
	QLabel* HintLabel;
	QHBoxLayout* HintLayout;

	//�ܲ���
	QVBoxLayout* Layout;

	/* ���� */
	const int UsernameMaxSize = 16; //�û����16�ֽ�
	const int KeywordMaxSize = 12;	//�����12�ֽ�

private slots:
	void LoginPushButton_Slots(); //��¼��ť�����ۺ���

signals:
	void Signal_Login(QString _username, QString _keyword);

};
