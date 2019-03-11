

#include "LoginChangeKeywordPage.h"

LoginChangeKeywordPage::LoginChangeKeywordPage(QWidget *parent)
	: QWidget(parent)
{
	//QT����
	{
		//��ʾ
		TitleLabel = new QLabel("�û��״ε�½����Ҫ����");

		//������
		oKeywordLabel = new QLabel(tr("ԭ����"));
		oKeywordLineEdit = new QLineEdit;
		oKeywordLineEdit->setMaxLength(KeywordMaxSize);
		oKeywordLineEdit->setEchoMode(KeywordMode);
		oKeywordLayout = new QHBoxLayout;
		oKeywordLayout->addWidget(oKeywordLabel, 1);
		oKeywordLayout->addWidget(oKeywordLineEdit, 3);

		//������
		nKeywordLabel = new QLabel(tr("������"));
		nKeywordLineEdit = new QLineEdit;
		nKeywordLineEdit->setMaxLength(KeywordMaxSize);
		nKeywordLineEdit->setEchoMode(KeywordMode);
		nKeywordLayout = new QHBoxLayout;
		nKeywordLayout->addWidget(nKeywordLabel, 1);
		nKeywordLayout->addWidget(nKeywordLineEdit, 3);


		//��¼��ť
		CertifyPushButton = new QPushButton(tr("��½"));

		//������ʾ - ���ɼ�
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

	//Qt �źŲ�
	{
		//ȷ�ϰ�ť���źŴ���
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
		HintLabel->setText("��ԭ������ͬ����������������");
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
