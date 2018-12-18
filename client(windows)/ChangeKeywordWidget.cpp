#include "ChangeKeywordWidget.h"

ChangeKeywordWidget::ChangeKeywordWidget(QWidget *parent)
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

		//旧密码
		oKeywordLabel = new QLabel(tr("旧密码"));
		oKeywordLineEdit = new QLineEdit;
		oKeywordLineEdit->setMaxLength(KeywordMaxSize);
		oKeywordLineEdit->setEchoMode(KeywordMode);
		oKeywordLayout = new QHBoxLayout;
		oKeywordLayout->addWidget(oKeywordLabel, 1);
		oKeywordLayout->addWidget(oKeywordLineEdit, 3);

		//新密码
		oKeywordLabel = new QLabel(tr("新密码"));
		oKeywordLineEdit = new QLineEdit;
		oKeywordLineEdit->setMaxLength(KeywordMaxSize);
		oKeywordLineEdit->setEchoMode(KeywordMode);
		oKeywordLayout = new QHBoxLayout;
		oKeywordLayout->addWidget(oKeywordLabel, 1);
		oKeywordLayout->addWidget(oKeywordLineEdit, 3);


		//登录按钮
		LoginPushButton = new QPushButton(tr("确认修改"));

		//错误提示 - 不可见
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
