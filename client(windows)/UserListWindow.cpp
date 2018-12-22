
#include "UserListWindow.h"
#include <QPainter>

UserListWindow::UserListWindow(QString _username, QWidget *parent)
	: username(_username),QDialog(parent)
{
	//Qt �ؼ�
	{
		//��ͷ
		ulUserListHead = new UserListHeadWidget(_username);

		ulUserListListWidget = new QListWidget;
	
		//ulUserListListWidget->setStyleSheet("background:transparent;");

		ulUserListGroupBox = new QGroupBox("��ϵ��");
		//ulUserListGroupBox->setStyleSheet("background:transparent;");
		ulUserListGroupBoxLayout = new QVBoxLayout(ulUserListGroupBox);
		ulUserListGroupBoxLayout->addWidget(ulUserListListWidget);

		Layout = new QVBoxLayout(this);
		Layout->addWidget(ulUserListHead);
		Layout->addWidget(ulUserListGroupBox);

		this->setFixedWidth(420);

		this->setAutoFillBackground(true);
		QPalette palette = this->palette();
		palette.setBrush(QPalette::Window, QBrush(QPixmap("Resources//9-bg.jpg").scaled(/*this->size()*/QSize(420, 800), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));// ʹ��ƽ�������ŷ�ʽ
		this->setPalette(palette);// ��widget���ϱ���ͼ

	}

}

UserListWindow::~UserListWindow()
{
}

void UserListWindow::AddOneUser(QString _user)
{
	QWidget* item = new QWidget;
	QHBoxLayout* itemlayout = new QHBoxLayout(item);

	//Բ��ͷ�����ʾ
	QPixmap pixmapa(HeadImageAddr);
	QPixmap pixmap(ImageSize, ImageSize);
	pixmap.fill(Qt::transparent);
	QPainter painter(&pixmap);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	QPainterPath path;
	path.addEllipse(1, 1, ImageSize, ImageSize);
	painter.setClipPath(path);
	painter.drawPixmap(1, 1, ImageSize, ImageSize, pixmapa);

	QLabel* IconLabel = new QLabel;
	IconLabel->setPixmap(pixmap);

	QChar flag = _user[_user.length() - 1];
	QString _username = _user.left(_user.length()-1);

	QLabel* name = new QLabel(_username);
	QFont SignatureFont("����", SignatureFontSize, 50);
	name->setFont(SignatureFont);


	QLabel* OnOffLabel = new QLabel;
	if (flag == '1') OnOffLabel->setText("(online)");
	else OnOffLabel->setText("(offline)");

	itemlayout->addWidget(IconLabel,1);
	itemlayout->addWidget(name,4);
	itemlayout->addWidget(OnOffLabel,1);
	itemlayout->setAlignment(Qt::AlignLeft);


	item->setFixedHeight(60);
	QListWidgetItem* listitem = new QListWidgetItem();
	listitem->setSizeHint(QSize(100,60));
	ulUserListListWidget->addItem(listitem);
	ulUserListListWidget->setItemWidget(listitem, item);
	
}


//#define TEST_USERLISTWINDOW
#ifdef TEST_USERLISTWINDOW

#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	UserListWindow test("��С��");
	
	test.AddOneUser("������1");
	
	test.exec();

	return a.exec();
}


#endif // TEST_USERLISTWINDOW
