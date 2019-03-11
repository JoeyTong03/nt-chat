
#include "UserListHeadWidget.h"
#include <QPainter>
#include <QFont>

UserListHeadWidget::UserListHeadWidget(QString _username, QWidget *parent)
	: QWidget(parent)
{
	//Qt布局
	{
		//圆形头像的显示
		QPixmap pixmapa(HeadImageAddr);
		QPixmap pixmap(ImageSize, ImageSize);
		pixmap.fill(Qt::transparent);
		QPainter painter(&pixmap);
		painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
		QPainterPath path;
		path.addEllipse(1, 1, ImageSize, ImageSize);
		painter.setClipPath(path);
		painter.drawPixmap(1, 1, ImageSize, ImageSize, pixmapa);

		HeadImageLabel = new QLabel;
		HeadImageLabel->setPixmap(pixmap);

		//用户名
		Username = new QLabel(_username);
		QFont UsernameFont("Microsoft YaHei", UsernameFontSize, 75);
		Username->setStyleSheet("background:transparent;");
		Username->setFont(UsernameFont);

		//个性签名
		Signature = new QLabel("经不住似水流年，逃不过此间少年");
		QFont SignatureFont("黑体", SignatureFontSize, 50);
		Signature->setStyleSheet("background:transparent;");
		Signature->setFont(SignatureFont);

		UserInfoLayout = new QVBoxLayout;
		UserInfoLayout->addWidget(Username);
		UserInfoLayout->addWidget(Signature);
		UserInfoLayout->setAlignment(Qt::AlignTop|Qt::AlignLeft);
		UserInfoLayout->setContentsMargins(20,0,0,0);

		//总布局
		Layout = new QHBoxLayout(this);
		Layout->addWidget(HeadImageLabel);
		Layout->addLayout(UserInfoLayout);

		//this->setStyleSheet("background:transparent");

	}


}


UserListHeadWidget::~UserListHeadWidget()
{

}


//#define TEST_USERLISTHEADWIDGET
#ifdef TEST_USERLISTHEADWIDGET

#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	UserListHeadWidget test("杨小宇");
	test.show();

	return a.exec();
}


#endif // TEST_USERLISTHEADWIDGET


