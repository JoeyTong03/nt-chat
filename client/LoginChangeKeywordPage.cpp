

#include "LoginChangeKeywordPage.h"

LoginChangeKeywordPage::LoginChangeKeywordPage(QWidget *parent)
	: QWidget(parent)
{
	//QT布局
	{
		//提示
		TitleLabel = new QLabel("用户首次登陆，需要改密");

		//旧密码
		oKeywordLabel = new QLabel(tr("原密码"));
		oKeywordLineEdit = new QLineEdit;
		oKeywordLineEdit->setMaxLength(KeywordMaxSize);
		oKeywordLineEdit->setEchoMode(KeywordMode);
		oKeywordLayout = new QHBoxLayout;
		oKeywordLayout->addWidget(oKeywordLabel, 1);
		oKeywordLayout->addWidget(oKeywordLineEdit, 3);

		//新密码
		nKeywordLabel = new QLabel(tr("新密码"));
		nKeywordLineEdit = new QLineEdit;
		nKeywordLineEdit->setMaxLength(KeywordMaxSize);
		nKeywordLineEdit->setEchoMode(KeywordMode);
		nKeywordLayout = new QHBoxLayout;
		nKeywordLayout->addWidget(nKeywordLabel, 1);
		nKeywordLayout->addWidget(nKeywordLineEdit, 3);


		//登录按钮
		CertifyPushButton = new QPushButton(tr("登陆"));

		//错误提示 - 不可见
		HintLabel = new QLabel;
		HintLabel->setVisible(false);
		HintLayout = new QHBoxLayout;
		HintLayout->addWidget(HintLabel);

		Layout = new QVBoxLayout(this);
		Layout->addWidget(TitleLabel);
		Layout->addLayout(oKeywordLayout);
		Layout->addLayout(nKeywordLayout);
		Layout->addWidget(CertifyPushButton);
		Layout->addLayout(HintLayout);
		Layout->setAlignment(Qt::AlignTop);

		//this->setFixedSize(400, 150);
	}

	//Qt 信号槽
	{
		//确认按钮的信号处理
		QObject::connect(
			CertifyPushButton,
			&QPushButton::clicked,
			this,
			&LoginChangeKeywordPage::CertifyPushButton_Slots
		);
	}
}

LoginChangeKeywordPage::~LoginChangeKeywordPage()
{
}

void LoginChangeKeywordPage::CertifyPushButton_Slots()
{
	QString okeyword = oKeywordLineEdit->text();
	QString nkeyword = nKeywordLineEdit->text();

	if (okeyword == nkeyword)
	{
		HintLabel->setText("与原密码相同，请重新输入密码");
		HintLabel->setStyleSheet("color:red;");
		HintLabel->setVisible(true);
		return;
	}

	emit Signal_NewKeyword(nkeyword);
}

//#define TEST_LOGINCHANGEKEYWORD
#ifdef TEST_LOGINCHANGEKEYWORD

#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	LoginChangeKeywordPage test;
	test.show();

	return a.exec();
}


#endif // TEST_LOGINCHANGEKEYWORD
