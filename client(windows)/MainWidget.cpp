
#include "MainWidget.h"
#include <QString>
#include <QByteArray>
#include <QMessageBox>

MainWidget::MainWidget(QWidget *parent)
	: QWidget(parent)
{

	/* 初始化Socket网络连接 */
	socket = new ClientSocket;

	//Qt控件控制
	{
		/* 初始化登陆界面 */
		{
			mLogin = new LoginWidget;
			this->setFixedSize(300, 180);
		}

		/* 改密界面 */
		{
			mChangeKeyword = new ChangeKeywordWidget;
		}

		/* 聊天界面 */
		{
			//多个tab界面
			mTabWidget = new QTabWidget;

			//群聊界面
			mGroupChat = new GroupChatWidget;
			mTabWidget->addTab(mGroupChat, tr("聊天"));
			mTabWidget->setTabPosition(QTabWidget::North);
		}


		/* StackWidget */
		mStackedWidget = new QStackedWidget;
		mStackedWidget->addWidget(mLogin);
		mStackedWidget->addWidget(mTabWidget);
		mStackedWidget->addWidget(mChangeKeyword);
		mStackedWidget->setCurrentIndex(0);

		Layout = new QVBoxLayout(this);
		Layout->addWidget(mStackedWidget);

	}


	//Qt 信号槽
	{
		/* 网络接收到信息 -  */
		QObject::connect(socket->socket, &QTcpSocket::readyRead, this, &MainWidget::GetInfoSlots);

		/* Login登陆界面-登陆按钮 */
		QObject::connect(mLogin->LoginPushButton, &QPushButton::clicked, this, &MainWidget::LoginSlots);

	}

}

MainWidget::~MainWidget()
{
}

//登陆 - 槽函数
void MainWidget::LoginSlots()
{
	//连接服务器
	if (socket->connectServer() == false)
	{
		//连接失败
		mLogin->ShowErrorMessage("网络不可达");

		return;
	};

	//发送登陆帧
	socket->SendRegisterFrame(mLogin->GetUsername(), mLogin->GetKeyword());
}


//得到信息 - 槽函数
void MainWidget::GetInfoSlots()
{
	//得到服务端发来的信息
	QByteArray Qba = socket->socket->readAll();
	qDebug << Qba;

	//得到了服务端发来的信息
	QString ServerMessage = QVariant(Qba).toString();
	const char* tmpbuf = ServerMessage.toStdString().c_str();

	//判断帧头类型
	if ((int)tmpbuf[0] == 0x71)
	{
		//报道帧
		switch ((int)tmpbuf[1])
		{
		case 0://登陆成功
		{
			//切换界面 - 换到了群聊界面
			mStackedWidget->setCurrentIndex(1);
			this->setFixedSize(600, 400);
			break;
		}
		case 1://密码错误
		{
			//显示错误信息
			mLogin->ShowErrorMessage("密码错误");
			break;
		}
		case 2://用户名不存在
		{
			//显示错误信息
			mLogin->ShowErrorMessage("用户不存在");
			break;
		}
		case 3://需要改密
		{
			//切换界面
			//切换界面 - 换到了群聊界面
			mStackedWidget->setCurrentIndex(2);
			this->setFixedSize(600, 400);
			break;
		}
		default:
			QMessageBox::about(NULL, "Warning!!!", "Client has accepted a error frame in register frame");
			break;
		}
	}
	else if ((int)tmpbuf[0] == 0x72)
	{
		//文本信息帧
		switch ((int)tmpbuf[1])
		{
		case 0x00://转发成功
			//
			break;
		case 0xFF://信息语法有误
			//
			break;
		case 0xFE://好友名称不存在
			//
			break;
		case 0xFD://好友未上线
			//
			break;
		default:
			break;
		}
	}
	else if ((int)tmpbuf[0] == 0x73)
	{
		//下线退位帧

	}
	else if ((int)tmpbuf[0] == 0x75)
	{
		////下线，上线提示帧
		//QString name = AcceptOnOffLineName(buf);//上线或下线用户名
		//if ((int)buf[1] == 0)
		//{
		//	//上线
		//}
		//else
		//{
		//	//下线
		//}

	}
	else if ((int)tmpbuf[0] == 0x76)
	{
		////好友初始化帧

		//QVector<QString>namelist = AcceptInitFri(buf);
		////namelist为好友列表(vector)
		////
	}
	else if ((int)tmpbuf[0] == 0x77)
	{
		//文本信息帧
		QString FromName;//信息来源
		//QString Text = AcceptText(buf, FromName);//文本信息
		//处理文本信息
	}
	else
	{
		QMessageBox::about(NULL, "Warning!!!", "Client has accepted a unknow frame");

	}

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
