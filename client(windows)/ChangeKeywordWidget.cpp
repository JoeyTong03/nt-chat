#include "ChangeKeywordWidget.h"

ChangeKeywordWidget::ChangeKeywordWidget(QWidget *parent)
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

		//������
		oKeywordLabel = new QLabel(tr("������"));
		oKeywordLineEdit = new QLineEdit;
		oKeywordLineEdit->setMaxLength(KeywordMaxSize);
		oKeywordLineEdit->setEchoMode(KeywordMode);
		oKeywordLayout = new QHBoxLayout;
		oKeywordLayout->addWidget(oKeywordLabel, 1);
		oKeywordLayout->addWidget(oKeywordLineEdit, 3);

		//������
		oKeywordLabel = new QLabel(tr("������"));
		oKeywordLineEdit = new QLineEdit;
		oKeywordLineEdit->setMaxLength(KeywordMaxSize);
		oKeywordLineEdit->setEchoMode(KeywordMode);
		oKeywordLayout = new QHBoxLayout;
		oKeywordLayout->addWidget(oKeywordLabel, 1);
		oKeywordLayout->addWidget(oKeywordLineEdit, 3);


		//��¼��ť
		LoginPushButton = new QPushButton(tr("ȷ���޸�"));

		//������ʾ - ���ɼ�
		HintLabel = new QLabel;
		HintLabel->setVisible(false);
		HintLayout = new QHBoxLayout;
		HintLayout->addWidget(HintLabel);

		Layout = new QVBoxLayout(this);
		Layout->addLayout(UsernameLayout);
		Layout->addLayout(oKeywordLayout);
		Layout->addLayout(nKeywordLayout);
		Layout->addWidget(LoginPushButton);
		Layout->addLayout(HintLayout);
		Layout->setAlignment(Qt::AlignTop);

		//		this->setFixedSize(400, 150);
	}

}

ChangeKeywordWidget::~ChangeKeywordWidget()
{
}
