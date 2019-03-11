#include "LoginWindow.h"

LoginWindow::LoginWindow(QWidget *parent)
	: QDialog(parent)
{
	//Qt ����
	{
		lLoginInputPage = new LoginInputPage; //��½�����
		lLoginWaitPage = new LoginWaitPage;	  //��½�ȴ���
		lLoginChangeKeywordPage = new LoginChangeKeywordPage; //�޸������

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

//���õ�ǰ���ڵ�ҳ��
void LoginWindow::SetCurrnetPage(PAGENUM _page)
{
	lStackWidget->setCurrentIndex(_page);
}

//���õ�½����Ĵ�����Ϣ
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


