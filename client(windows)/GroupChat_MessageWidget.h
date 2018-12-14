#pragma once

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
		QString _ico_addr,	//ͷ���ļ���ַ
		QWidget *parent = Q_NULLPTR
	);

	~Icon_Widget();

private:
	void paintEvent(QPaintEvent* event);

	int LEFT_MARGIN;	//left
	int TOP_MARGIN;		//top
	int ICON_SIZE;		//Width

	QString ico_addr;	//ͷ���ļ���ַ

};

class GroupChat_MessageWidget : public QWidget
{
	Q_OBJECT

public:

	//��Ϣ����
	enum MessageType
	{
		MTSystem,	//ϵͳ�û�
		MTMe,		//���Լ�
		MTOthers,	//�����û�
		MTTime		//ʱ���
	};

	GroupChat_MessageWidget(
		MessageType _mt,
		QString _username = "",
		QString _text = "",
		QWidget *parent = Q_NULLPTR
	);
	~GroupChat_MessageWidget();

	QSize GetSize();

private:

	MessageType mt;	//�����Ϣ������

	//ͷ���
	Icon_Widget* IconWidget;

	//�û���
	QLabel* UsernameLabel;

	//�ı���
	QLabel* TextEdit;

	//�ı���
	QVBoxLayout* TextLayout;

	//�ܲ���
	QHBoxLayout* Layout;
	
	//ʱ���
	QLabel* TimeLabel;

	int Height;
	int Width;

	//��������
	const int LEFT_MARGIN = 20;		//��Ϣ�����ұ߿�ļ��
	const int TOP_MARGIN = 10;		//��Ϣ��������֮��ļ��
	const int ICON_SIZE = 40;		//ͷ��Ĵ�С(������ - ����)
	const int ICONMESSAGE_GIP = 10;	//ͷ������Ϣ֮��ļ��
	const int USERNAME_HEIGHT = 10;	//�û����ĸ߶�
	const int USERNAME_FONTSIZE = 8;//�û�������

	//����ͼ���ļ�
	const QString MyIcon_Addr = "Resources//4-me.png";
	const QString OtherIcon_Addr = "Resources//3-other.png";

	//��Ϣ
	QString text = "����һ����Ϣ";
	QString username = "yxy";

	//����
	int Text_FontSize = 12;					//�����С
	QString Text_FontFamily = "΢���ź�";	//����

};
