
#pragma execution_character_set("utf-8")

#include "GroupChat_MessageWidget.h"
#include <QFont>
#include <QPainter>
#include <QDateTime>
#include <QTextEdit>

Icon_Widget::Icon_Widget(
	int _LEFT_MARGIN,	//left
	int _TOP_MARGIN,	//top
	int _ICON_SIZE,		//icon size
	QString _ico_addr,	//ͷ���ļ���ַ
	QWidget *parent
) :
	LEFT_MARGIN(_LEFT_MARGIN), TOP_MARGIN(_TOP_MARGIN),
	ICON_SIZE(_ICON_SIZE), ico_addr(_ico_addr), QWidget(parent)
{

};

Icon_Widget::~Icon_Widget()
{

}

void Icon_Widget::paintEvent(QPaintEvent * event)
{
	Q_UNUSED(event);
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);//�����
	painter.setPen(Qt::NoPen);
	painter.setBrush(QBrush(Qt::gray));

	/* ͷ�� QRect(left,top,width,top) */
	QRect IconRect(
		LEFT_MARGIN,	//left
		TOP_MARGIN,		//top
		ICON_SIZE,		//Width
		ICON_SIZE		//Height
	);
	QPixmap IconPhoto(ico_addr);
	painter.drawPixmap(IconRect, IconPhoto);
}



GroupChat_MessageWidget::GroupChat_MessageWidget(
	MessageType _mt,
	QString _username,
	QString _text,
	QWidget *parent
)
	:mt(_mt), text(_text), username(_username), QWidget(parent)
{
	if (mt == MTMe || mt == MTOthers)
	{
		//�û�

		IconWidget = new Icon_Widget(
			LEFT_MARGIN,
			TOP_MARGIN,
			ICON_SIZE,
			(mt == MTMe) ? MyIcon_Addr : OtherIcon_Addr
		);

		IconWidget->setFixedWidth(LEFT_MARGIN + ICON_SIZE + ICONMESSAGE_GIP);

		QFont usernamefont;
		usernamefont.setPointSize(USERNAME_FONTSIZE);
		UsernameLabel = new QLabel(username);
		UsernameLabel->setFont(usernamefont);

		QFont textfont;
		textfont.setPointSize(Text_FontSize);
		textfont.setFamily(Text_FontFamily);
		TextEdit = new QLabel(text);
		TextEdit->adjustSize();
		TextEdit->setWordWrap(true);
		TextEdit->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px;background-color: rgb(234,234,234);color: rgb(0,0,0);");
		TextEdit->setFont(textfont);

		//TextEdit->setAlignment(Qt::AlignTop | Qt::AlignLeft);

		TextLayout = new QVBoxLayout;
		TextLayout->addWidget(UsernameLabel);
		TextLayout->addWidget(TextEdit);

		Layout = new QHBoxLayout(this);

		if (mt == MTOthers)
		{
			UsernameLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
			TextEdit->setAlignment(Qt::AlignTop | Qt::AlignLeft);

			TextLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
			Layout->addWidget(IconWidget);
			Layout->addLayout(TextLayout);
		}
		else
		{
			UsernameLabel->setAlignment(Qt::AlignTop | Qt::AlignRight);
			TextEdit->setAlignment(Qt::AlignTop | Qt::AlignRight);

			TextLayout->setAlignment(Qt::AlignTop | Qt::AlignRight);
			Layout->addLayout(TextLayout);
			Layout->addWidget(IconWidget);
		}

		//Height = TOP_MARGIN + TextEdit->height() + TOP_MARGIN;

	}
	else if (mt == MTTime)
	{
		//ʱ���
		QDateTime current_time = QDateTime::currentDateTime();
		//��ʾʱ�䣬��ʽΪ����-��-�� ʱ���֣��� �ܼ�
		QString StrCurrentTime = current_time.toString("yyyy-MM-dd hh:mm:ss");
		TimeLabel = new QLabel(StrCurrentTime);
		TimeLabel->adjustSize();
		TimeLabel->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
		Layout = new QHBoxLayout(this);
		Layout->addWidget(TimeLabel);
		Layout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
	}

	this->adjustSize();
	Height = this->height();
	Width = this->width();
}

GroupChat_MessageWidget::~GroupChat_MessageWidget()
{

}

QSize GroupChat_MessageWidget::GetSize()
{
	return QSize(Width, Height);
}


//#define TEST_GROUPCHAT_MESSAGE
#ifdef  TEST_GROUPCHAT_MESSAGE
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	GroupChat_MessageWidget test(GroupChat_MessageWidget::MTMe,"��С��","�����ҷ���һ����Ϣ");
	test.show();

	return a.exec();
}

#endif // TEST_GROUPCHAT_MESSAGE
