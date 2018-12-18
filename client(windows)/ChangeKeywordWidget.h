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

	/* QT���� */

	//�û���
	QLabel* UsernameLabel;
	QLineEdit* UsernameLineEdit;
	QHBoxLayout* UsernameLayout;

	//������
	QLabel* oKeywordLabel;
	QLineEdit* oKeywordLineEdit;
	QHBoxLayout* oKeywordLayout;

	//������
	QLabel* nKeywordLabel;
	QLineEdit* nKeywordLineEdit;
	QHBoxLayout* nKeywordLayout;

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

	friend class MainWidget;

};
