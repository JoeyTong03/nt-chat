
#include "LoginInputPage.h"


LoginInputPage::LoginInputPage(QWidget *parent)
	: QWidget(parent)
{
	//QT����
	{
		//�û���
		UsernameLabel = new QLabel(tr("�û���"));
		UsernameLineEdit = new QLineEdit;
		UsernameLineEdit->setMaxLength(UsernameMaxSize);
		UsernameLayout = new QHBoxLayout;
		UsernameLayout->addWidget(UsernameLabel, 1);
		UsernameLayout->addWidget(UsernameLineEdit, 3);

		//����
		KeywordLabel = new QLabel(tr("����"));
		KeywordLineEdit = new QLineEdit;
		KeywordLineEdit->setMaxLength(KeywordMaxSize);
		KeywordLineEdit->setEchoMode(KeywordMode);
		KeywordLayout = new QHBoxLayout;
		KeywordLayout->addWidget(KeywordLabel, 1);
		KeywordLayout->addWidget(KeywordLineEdit, 3);

		//��¼��ť
		LoginPushButton = new QPushButton(tr("��½"));

		//������ʾ - ���ɼ�
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

	//Qt �źŲ�
	{
		//��½��ť���źŴ���
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
	//���û������������źŵķ�ʽ���ͳ�ȥ
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

	test.ShowErrorMessage("�������");

	test.show();

	return a.exec();
}


#endif // TEST_LOGININPUTPAGE


