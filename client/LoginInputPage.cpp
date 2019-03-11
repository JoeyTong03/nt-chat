
#include "LoginInputPage.h"


LoginInputPage::LoginInputPage(QWidget *parent)
	: QWidget(parent)
{
	//QT布局
	{
		//用户名
		UsernameLabel = new QLabel(tr("用户名"));
		UsernameLineEdit = new QLineEdit;
		UsernameLineEdit->setMaxLength(UsernameMaxSize);
		UsernameLayout = new QHBoxLayout;
		UsernameLayout->addWidget(UsernameLabel, 1);
		UsernameLayout->addWidget(UsernameLineEdit, 3);

		//密码
		KeywordLabel = new QLabel(tr("密码"));
		KeywordLineEdit = new QLineEdit;
		KeywordLineEdit->setMaxLength(KeywordMaxSize);
		KeywordLineEdit->setEchoMode(KeywordMode);
		KeywordLayout = new QHBoxLayout;
		KeywordLayout->addWidget(KeywordLabel, 1);
		KeywordLayout->addWidget(KeywordLineEdit, 3);

		//登录按钮
		LoginPushButton = new QPushButton(tr("登陆"));

		//错误提示 - 不可见
		HintLabel = new QLabel;
		HintLabel->setVisible(false);
		HintLayout = new QHBoxLayout;
		HintLayout->addWidget(HintLabel);

		Layout = new QVBoxLayout(this);
		Layout->addLayout(UsernameLayout);
		Layout->addLayout(KeywordLayout);
		Layout->addWidget(LoginPushButton);
		Layout->addLayout(HintLayout);
		Layout->setAlignment(Qt::AlignTop);

		//		this->setFixedSize(400, 150);
	}

	//Qt 信号槽
	{
		//登陆按钮的信号处理
		QObject::connect(
			LoginPushButton,
			&QPushButton::clicked, 
			this, 
			&LoginInputPage::LoginPushButton_Slots
		);
	}
}

LoginInputPage::~LoginInputPage()
{

}

void LoginInputPage::ShowErrorMessage(QString _message)
{
	HintLabel->setText(_message);
	HintLabel->setStyleSheet("color:red;");
	HintLabel->setVisible(true);
}

void LoginInputPage::LoginPushButton_Slots()
{
	//将用户名与密码以信号的方式发送出去
	QString username = UsernameLineEdit->text();
	QString keyword = KeywordLineEdit->text();
	emit Signal_Login(username, keyword);
}

//#define TEST_LOGININPUTPAGE
#ifdef TEST_LOGININPUTPAGE

#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	LoginInputPage test;

	test.ShowErrorMessage("密码错误");

	test.show();

	return a.exec();
}


#endif // TEST_LOGININPUTPAGE


