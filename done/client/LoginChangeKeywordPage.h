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

	/* QT���� */

	//����
	QLabel* TitleLabel;


	//������
	QLabel* oKeywordLabel;
	QLineEdit* oKeywordLineEdit;
	QHBoxLayout* oKeywordLayout;

	//ԭ����
	QLabel* nKeywordLabel;
	QLineEdit* nKeywordLineEdit;
	QHBoxLayout* nKeywordLayout;

	QLineEdit::EchoMode KeywordMode = QLineEdit::Password;
	// PasswordEchoOnEdit;-����ʱ������ʾ�ַ����������Ϊ�����ַ�

	//��ʾ
	QLabel* HintLabel;
	QHBoxLayout* HintLayout;


	//ȷ�ϰ�ť
	QPushButton* CertifyPushButton;

	//�ܲ���
	QVBoxLayout* Layout;

	/* ���� */
	const int UsernameMaxSize = 16; //�û����16�ֽ�
	const int KeywordMaxSize = 12;	//�����12�ֽ�

private slots:
	void CertifyPushButton_Slots();

signals:
	//�����µ�����
	void Signal_NewKeyword(QString _keyword);

};

