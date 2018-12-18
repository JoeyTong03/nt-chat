#include "LoginWidget.h"

LoginWidget::LoginWidget(QWidget *parent)
	: QWidget(parent)
{
	//QT����
	{
		//�û���
		UsernameLabel = new QLabel(tr("�û���"));
		UsernameLineEdit = new QLineEdit;
		UsernameLineEdit->setMaxLength(UsernameMaxSize);
		UsernameLayout = new QHBoxLayout;
		UsernameLayout->addWidget(UsernameLabel,1);
		UsernameLayout->addWidget(UsernameLineEdit,3);

		//����
		KeywordLabel = new QLabel(tr("����"));
		KeywordLineEdit = new QLineEdit;
		KeywordLineEdit->setMaxLength(KeywordMaxSize);
		KeywordLineEdit->setEchoMode(KeywordMode);
		KeywordLayout = new QHBoxLayout;
		KeywordLayout->addWidget(KeywordLabel,1);
		KeywordLayout->addWidget(KeywordLineEdit,3);

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
