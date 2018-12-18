#include "LoginWidget.h"

LoginWidget::LoginWidget(QWidget *parent)
	: QWidget(parent)
{
	//QT布局
	{
		//用户名
		UsernameLabel = new QLabel(tr("用户名"));
		UsernameLineEdit = new QLineEdit;
		UsernameLineEdit->setMaxLength(UsernameMaxSize);
		UsernameLayout = new QHBoxLayout;
		UsernameLayout->addWidget(UsernameLabel,1);
		UsernameLayout->addWidget(UsernameLineEdit,3);

		//密码
		KeywordLabel = new QLabel(tr("密码"));
		KeywordLineEdit = new QLineEdit;
		KeywordLineEdit->setMaxLength(KeywordMaxSize);
		KeywordLineEdit->setEchoMode(KeywordMode);
		KeywordLayout = new QHBoxLayout;
		KeywordLayout->addWidget(KeywordLabel,1);
		KeywordLayout->addWidget(KeywordLineEdit,3);

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
}

LoginWidget::~LoginWidget()
{

}

void LoginWidget::ShowErrorMessage(QString _message)
{
	HintLabel->setText(_message);
	HintLabel->setStyleSheet("color:red;");
	HintLabel->setVisible(true);
}

//#define TEST_LOGINWIDGET
#ifdef TEST_LOGINWIDGET

#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	LoginWidget test;
	test.show();

	return a.exec();
}


#endif // TEST_LOGINWIDGET
