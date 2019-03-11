
#include "UserListHeadWidget.h"
#include <QPainter>
#include <QFont>

UserListHeadWidget::UserListHeadWidget(QString _username, QWidget *parent)
	: QWidget(parent)
{
	//Qt����
	{
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

		HeadImageLabel = new QLabel;
		HeadImageLabel->setPixmap(pixmap);

		//�û���
		Username = new QLabel(_username);
		QFont UsernameFont("Microsoft YaHei", UsernameFontSize, 75);
		Username->setStyleSheet("background:transparent;");
		Username->setFont(UsernameFont);

		//����ǩ��
		Signature = new QLabel("����ס��ˮ���꣬�Ӳ����˼�����");
		QFont SignatureFont("����", SignatureFontSize, 50);
		Signature->setStyleSheet("background:transparent;");
		Signature->setFont(SignatureFont);

		UserInfoLayout = new QVBoxLayout;
		UserInfoLayout->addWidget(Username);
		UserInfoLayout->addWidget(Signature);
		UserInfoLayout->setAlignment(Qt::AlignTop|Qt::AlignLeft);
		UserInfoLayout->setContentsMargins(20,0,0,0);

		//�ܲ���
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

	UserListHeadWidget test("��С��");
	test.show();

	return a.exec();
}


#endif // TEST_USERLISTHEADWIDGET


