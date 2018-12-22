
#include "TopWidget.h"

#include <QMessageBox>

TopWidget::TopWidget()
{
	this->setVisible(false);


	//Qt �ؼ�
	{
		//����QTcpSocket����
		{
			//��ʼ������
			tClientSocket = new ClientSocket(this);

			//����Ӧ��֡�Ļظ�
			QObject::connect(tClientSocket, &ClientSocket::Signal_RegisterACK, this, &TopWidget::LoginACK_Slots);

		}


		//��½����
		{
			//��ʼ����½����
			tLoginWindow = new LoginWindow;

			//Qt��½��ť
			QObject::connect(
				tLoginWindow->lLoginInputPage,
				&LoginInputPage::Signal_Login,
				this,
				&TopWidget::Login_Slots
			);

			//Qt���͸�������
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
 * �������ƣ�Login_Slots
 * ��    �ܣ��ͻ�����������������ӣ����е�½
 * ��    ����
 * ��    �أ�
 * ˵    ����LoginInputPage�ĵ�½�����£�ʹ���źŽ��û��������봫������ۺ���
 *			 ����ۺ������Ƚ�LoginWindow����ҳ����л�,����Socket����
 *			 �������ͨ����������������û���������ı���֡������
 *			 ����������LoginWindow����ҳ���л�����¼���棬��ʾ������Ϣ
*************************************/
void TopWidget::Login_Slots(QString _username, QString _keyword)
{
	//��½���ڽ���ȴ�����
	tLoginWindow->SetCurrnetPage(LoginWindow::pnLOGINWAIT);

	//����Socket������
	if (!tClientSocket->connectServer())
	{

		//���粻�ɴSocket���ӷ�����ʧ��
		tLoginWindow->SetCurrnetPage(LoginWindow::pnLOGININPUT);
		tLoginWindow->SetErrorMessage("���粻�ɴ���ӷ�����ʧ��");

		return;
	};

	//���ͱ���֡
	tClientSocket->SendRegisterFrame(_username, _keyword);
	ClientUser = _username;

}

/*************************************
 * �������ƣ�LoginACK_Slots
 * ��    �ܣ��������ͻ��˷�����һ������Ӧ��֡���Ա���Ӧ����д���
 * ��    ����
 * ��    �أ�
 * ˵    �����ź�Я������������źŲۺ��� 0-�ɹ� 1-������� 2-�û��������� 3-��Ҫ���� default:��Ϣ����
 *			 ����ɹ��������û��б��ڣ�����ǰ�ĵ�½���ڽ����˳�
 *			 ���������תΪ��½���棬��ʾ�������;ͬ���û���������
 *			 ����״ε�½����תΪ���ܽ���
*************************************/
void TopWidget::LoginACK_Slots(char _type)
{
	switch (_type)
	{
	case 1:
		//��½�ɹ���ͨ����֤�������״ε�½
	{
		//��½����ر�
		tLoginWindow->close();

		//�����û��б�
		CreateUserList();
		break;
	}

	case 2:
		//��½ʧ�ܣ��������
	{
		tLoginWindow->SetCurrnetPage(LoginWindow::pnLOGININPUT);
		tLoginWindow->SetErrorMessage("��¼ʧ�ܣ��������");
		break;
	}

	case 3:
		//��½ʧ�ܣ��û���������
	{
		tLoginWindow->SetCurrnetPage(LoginWindow::pnLOGININPUT);
		tLoginWindow->SetErrorMessage("��¼ʧ�ܣ��û���������");
		break;
	}

	case 4:
		//��½�ɹ�����Ҫ����
	{
		tLoginWindow->SetCurrnetPage(LoginWindow::pnLOGINCHANGEKEYWORD);
		break;
	}

	default:
		QMessageBox::about(NULL, "Warning", "����Ӧ��֡�յ��Ƿ���Ӧ����");
	}
}

/*************************************
 * �������ƣ�ChangeKeyword_Slots
 * ��    �ܣ��ͻ����������������֡
 * ��    ����
 * ��    �أ�
 * ˵    ����
*************************************/
void TopWidget::ChangeKeyword_Slots(QString _keyword)
{
	//���͸���֡
	tClientSocket->SendChangeSecretFrame(_keyword);

	//��½����ر�
	tLoginWindow->close();

	//�����û��б�
	CreateUserList();
}

void TopWidget::CreateUserList()
{
	//�û��б���
	tUserListWindow = new UserListWindow(ClientUser);
	
	//Qt �źŲ�
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
