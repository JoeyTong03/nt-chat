
#include "MainWidget.h"
#include <QString>
#include <QByteArray>
#include <QMessageBox>

MainWidget::MainWidget(QWidget *parent)
	: QWidget(parent)
{

	/* ��ʼ��Socket�������� */
	socket = new ClientSocket;

	//Qt�ؼ�����
	{
		/* ��ʼ����½���� */
		{
			mLogin = new LoginWidget;
			this->setFixedSize(300, 180);
		}

		/* ���ܽ��� */
		{
			mChangeKeyword = new ChangeKeywordWidget;
		}

		/* ������� */
		{
			//���tab����
			mTabWidget = new QTabWidget;

			//Ⱥ�Ľ���
			mGroupChat = new GroupChatWidget;
			mTabWidget->addTab(mGroupChat, tr("����"));
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


	//Qt �źŲ�
	{
		/* ������յ���Ϣ -  */
		QObject::connect(socket->socket, &QTcpSocket::readyRead, this, &MainWidget::GetInfoSlots);

		/* Login��½����-��½��ť */
		QObject::connect(mLogin->LoginPushButton, &QPushButton::clicked, this, &MainWidget::LoginSlots);

	}

}

MainWidget::~MainWidget()
{
}

//��½ - �ۺ���
void MainWidget::LoginSlots()
{
	//���ӷ�����
	if (socket->connectServer() == false)
	{
		//����ʧ��
		mLogin->ShowErrorMessage("���粻�ɴ�");

		return;
	};

	//���͵�½֡
	socket->SendRegisterFrame(mLogin->GetUsername(), mLogin->GetKeyword());
}


//�õ���Ϣ - �ۺ���
void MainWidget::GetInfoSlots()
{
	//�õ�����˷�������Ϣ
	QByteArray Qba = socket->socket->readAll();
	qDebug << Qba;

	//�õ��˷���˷�������Ϣ
	QString ServerMessage = QVariant(Qba).toString();
	const char* tmpbuf = ServerMessage.toStdString().c_str();

	//�ж�֡ͷ����
	if ((int)tmpbuf[0] == 0x71)
	{
		//����֡
		switch ((int)tmpbuf[1])
		{
		case 0://��½�ɹ�
		{
			//�л����� - ������Ⱥ�Ľ���
			mStackedWidget->setCurrentIndex(1);
			this->setFixedSize(600, 400);
			break;
		}
		case 1://�������
		{
			//��ʾ������Ϣ
			mLogin->ShowErrorMessage("�������");
			break;
		}
		case 2://�û���������
		{
			//��ʾ������Ϣ
			mLogin->ShowErrorMessage("�û�������");
			break;
		}
		case 3://��Ҫ����
		{
			//�л�����
			//�л����� - ������Ⱥ�Ľ���
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
		//�ı���Ϣ֡
		switch ((int)tmpbuf[1])
		{
		case 0x00://ת���ɹ�
			//
			break;
		case 0xFF://��Ϣ�﷨����
			//
			break;
		case 0xFE://�������Ʋ�����
			//
			break;
		case 0xFD://����δ����
			//
			break;
		default:
			break;
		}
	}
	else if ((int)tmpbuf[0] == 0x73)
	{
		//������λ֡

	}
	else if ((int)tmpbuf[0] == 0x75)
	{
		////���ߣ�������ʾ֡
		//QString name = AcceptOnOffLineName(buf);//���߻������û���
		//if ((int)buf[1] == 0)
		//{
		//	//����
		//}
		//else
		//{
		//	//����
		//}

	}
	else if ((int)tmpbuf[0] == 0x76)
	{
		////���ѳ�ʼ��֡

		//QVector<QString>namelist = AcceptInitFri(buf);
		////namelistΪ�����б�(vector)
		////
	}
	else if ((int)tmpbuf[0] == 0x77)
	{
		//�ı���Ϣ֡
		QString FromName;//��Ϣ��Դ
		//QString Text = AcceptText(buf, FromName);//�ı���Ϣ
		//�����ı���Ϣ
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
