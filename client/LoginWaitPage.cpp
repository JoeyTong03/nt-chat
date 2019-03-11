#include "LoginWaitPage.h"

LoginWaitPage::LoginWaitPage(QWidget *parent)
	: QWidget(parent)
{
	//QT ²¼¾Ö
	{
		WaitImageLabel = new QLabel;
		QMovie* gif = new QMovie(ImageAddr);
		WaitImageLabel->setMovie(gif);
		gif->start();

		Layout = new QVBoxLayout(this);
		Layout->addWidget(WaitImageLabel);

		this->setStyleSheet("background-color: rgb(235,235, 235)");
	}
}

LoginWaitPage::~LoginWaitPage()
{

}

//#define TEST_LOGINWAITPAGE
#ifdef TEST_LOGINWAITPAGE

#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	LoginWaitPage test;

	test.show();

	return a.exec();
}


#endif // TEST_LOGINWAITPAGE