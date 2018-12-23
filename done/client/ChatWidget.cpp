
#include "ChatWidget.h"
#include "QMessageBox"

ChatWidget::ChatWidget(QString _destuser, QString _me, ClientSocket* _socket, QWidget *parent)
	: dest_user(_destuser),MyUser(_me), socket(_socket), QDialog(parent)
{
	//QT空间及布局
	{
		gSendMessage = new Chat_SendMessageWidget(_destuser,socket);
		gShowMessgae = new Chat_ShowMessageWidget;

		gLeftLayout = new QVBoxLayout;
		gLeftLayout->addWidget(gShowMessgae, 2);
		gLeftLayout->addWidget(gSendMessage, 1);

		Layout = new QHBoxLayout(this);
		Layout->addLayout(gLeftLayout, 5);

		this->setWindowTitle(_destuser);
		this->resize(800, 600);
	}

	//Qt动作
	{
		//确认按钮发送，发送消息
		QObject::connect(
			gSendMessage->cCertify_PushButton,
			&QPushButton::clicked,
			this,
			&ChatWidget::SendMessage);

		//接受到别人的消息
		QObject::connect(
		socket,
		&ClientSocket::Signal_AcceptText,
		this,
		&ChatWidget::RecvMessage
		);

	}

	this->setAutoFillBackground(true);
	QPalette palette = this->palette();
	palette.setBrush(QPalette::Window, QBrush(QPixmap("Resources//10-bg.jpg").scaled(this->size()/*QSize(420, 800)*/, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));// 使用平滑的缩放方式
	this->setPalette(palette);// 给widget加上背景图

	this->setObjectName(_destuser);
}

void ChatWidget::SendMessage()
{
	//发送消息，客户端首先显示发送出去的话
	QString text = gSendMessage->GetMessagecs();
	gShowMessgae->Add_Message(MyUser, text, Chat_MessageWidget::MTMe);

	socket->SendTextFrame(text, dest_user);
}

void ChatWidget::RecvMessage(QString _text, QString _from)
{
	gShowMessgae->Add_Message(_from, _text, Chat_MessageWidget::MTOthers);
}

ChatWidget::~ChatWidget()
{

}

void ChatWidget::closeEvent(QCloseEvent * event)
{
	emit CloseSignal(dest_user);
}

//#define TEST_GROUPCHAT
#ifdef TEST_GROUPCHAT
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	ChatWidget test("MiaoMiaoYang");
	test.show();

	return a.exec();
}

#endif // TEST_GROUPCHAT
