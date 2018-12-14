
#include "MainWidget.h"

MainWidget::MainWidget(QWidget *parent)
	: QWidget(parent)
{
	//Qt�ؼ�����
	{
		mTabWidget = new QTabWidget;

		mGroupChat = new GroupChatWidget;
		mTabWidget->addTab(mGroupChat, tr("����"));
		mTabWidget->setTabPosition(QTabWidget::North);

		Layout = new QVBoxLayout(this);
		Layout->addWidget(mTabWidget);
	}
}

MainWidget::~MainWidget()
{
}

#define TEST_MAINWIDGET
#ifdef TEST_MAINWIDGET

#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	MainWidget test;
	test.show();

	return a.exec();
}


#endif // TEST_MAINWIDGET
