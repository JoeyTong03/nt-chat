#include "LoginWindow.h"

LoginWindow::LoginWindow(QWidget *parent)
	: QDialog(parent)
{
	//Qt 布局
	{
		lLoginInputPage = new LoginInputPage; //登陆输入框
		lLoginWaitPage = new LoginWaitPage;	  //登陆等待框
		lLoginChangeKeywordPage = new LoginChangeKeywordPage; //修改密码框

		lStackWidget = new QStackedWidget;
		lStackWidget->addWidget(lLoginInputPage);
		lStackWidget->addWidget(lLoginWaitPage);
		lStackWidget->addWidget(lLoginChangeKeywordPage);
		lStackWidget->setCurrentIndex(pnLOGININPUT);

		Layout = new QVBoxLayout(this);
		Layout->addWidget(lStackWidget);

		this->setFixedSize(400, 300);
	}
}

LoginWindow::~LoginWindow()
{
}

//设置当前窗口的页面
void LoginWindow::SetCurrnetPage(PAGENUM _page)
{
	lStackWidget->setCurrentIndex(_page);
}

//设置登陆错误的错误信息
void LoginWindow::SetErrorMessage(QString _msg)
{
	lLoginInputPage->ShowErrorMessage(_msg);
}

//#define TEST_LOGINWAITPAGE
#ifdef TEST_LOGINWAITPAGE

#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	LoginWindow test;

	test.show();

	return a.exec();
}


#endif // TEST_LOGINWAITPAGE


