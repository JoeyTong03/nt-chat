
#include "TopWidget.h"

#include <QMessageBox>

TopWidget::TopWidget()
{
	this->setVisible(false);


	//Qt 控件
	{
		//创建QTcpSocket对象
		{
			//初始化连接
			tClientSocket = new ClientSocket(this);

			//报到应答帧的回复
			QObject::connect(tClientSocket, &ClientSocket::Signal_RegisterACK, this, &TopWidget::LoginACK_Slots);

		}


		//登陆窗口
		{
			//初始化登陆窗口
			tLoginWindow = new LoginWindow;

			//Qt登陆按钮
			QObject::connect(
				tLoginWindow->lLoginInputPage,
				&LoginInputPage::Signal_Login,
				this,
				&TopWidget::Login_Slots
			);

			//Qt发送更新密码
			QObject::connect(
				tLoginWindow->lLoginChangeKeywordPage,
				&LoginChangeKeywordPage::Signal_NewKeyword,
				this,
				&TopWidget::ChangeKeyword_Slots
			);

			tLoginWindow->exec();

		}

	}

}

TopWidget::~TopWidget()
{

}

/*************************************
 * 函数名称：Login_Slots
 * 功    能：客户端向服务器发起连接，进行登陆
 * 参    数：
 * 返    回：
 * 说    明：LoginInputPage的登陆被按下，使用信号将用户名与密码传入这个槽函数
 *			 这个槽函数首先将LoginWindow进行页面的切换,建立Socket连接
 *			 如果网络通畅，向服务器发送用户名和密码的报道帧，结束
 *			 如果网络错误，LoginWindow进行页面切换到登录界面，显示错误信息
*************************************/
void TopWidget::Login_Slots(QString _username, QString _keyword)
{
	//登陆窗口进入等待界面
	tLoginWindow->SetCurrnetPage(LoginWindow::pnLOGINWAIT);

	//建立Socket的连接
	if (!tClientSocket->connectServer())
	{

		//网络不可达，Socket连接服务器失败
		tLoginWindow->SetCurrnetPage(LoginWindow::pnLOGININPUT);
		tLoginWindow->SetErrorMessage("网络不可达：连接服务器失败");

		return;
	};

	//发送报道帧
	tClientSocket->SendRegisterFrame(_username, _keyword);
	ClientUser = _username;

}

/*************************************
 * 函数名称：LoginACK_Slots
 * 功    能：服务端向客户端返回了一个报道应答帧，对报道应答进行处理
 * 参    数：
 * 返    回：
 * 说    明：信号携带参数进入该信号槽函数 0-成功 1-密码错误 2-用户名不存在 3-需要改密 default:信息错误
 *			 如果成功，生成用户列表窗口，将当前的登陆窗口进行退出
 *			 密码错误，跳转为登陆界面，显示密码错误;同理，用户名不存在
 *			 如果首次登陆，跳转为改密界面
*************************************/
void TopWidget::LoginACK_Slots(char _type)
{
	switch (_type)
	{
	case 1:
		//登陆成功，通过验证，不是首次登陆
	{
		//登陆界面关闭
		tLoginWindow->close();

		//创建用户列表
		CreateUserList();
		break;
	}

	case 2:
		//登陆失败，密码错误
	{
		tLoginWindow->SetCurrnetPage(LoginWindow::pnLOGININPUT);
		tLoginWindow->SetErrorMessage("登录失败：密码错误");
		break;
	}

	case 3:
		//登陆失败，用户名不存在
	{
		tLoginWindow->SetCurrnetPage(LoginWindow::pnLOGININPUT);
		tLoginWindow->SetErrorMessage("登录失败：用户名不存在");
		break;
	}

	case 4:
		//登陆成功，需要改密
	{
		tLoginWindow->SetCurrnetPage(LoginWindow::pnLOGINCHANGEKEYWORD);
		break;
	}

	default:
		QMessageBox::about(NULL, "Warning", "报道应答帧收到非法响应类型");
	}
}

/*************************************
 * 函数名称：ChangeKeyword_Slots
 * 功    能：客户端向服务器发改密帧
 * 参    数：
 * 返    回：
 * 说    明：
*************************************/
void TopWidget::ChangeKeyword_Slots(QString _keyword)
{
	//发送改密帧
	tClientSocket->SendChangeSecretFrame(_keyword);

	//登陆界面关闭
	tLoginWindow->close();

	//创建用户列表
	CreateUserList();
}

void TopWidget::CreateUserList()
{
	//用户列表开启
	tUserListWindow = new UserListWindow(ClientUser);
	
	//Qt 信号槽
	{
		QObject::connect(
			tClientSocket, &ClientSocket::Signal_InitFriend,
			this,&TopWidget::FriendInit
		);
	}
	
	tUserListWindow->exec();

}

void TopWidget::FriendInit(QVector<QString> _userlist)
{
	for (int i = 0; i < (int)_userlist.size(); i++)
		tUserListWindow->AddOneUser(_userlist[i]);

}

#define TEST_TOPWIDGET
#ifdef TEST_TOPWIDGET

#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	TopWidget test;
	test.show();

	return a.exec();
}


#endif // TEST_TOPWIDGET
