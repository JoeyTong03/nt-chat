

#include "Chat_SendMessageWidget.h"
#include <QFileDialog>

#include <QFile>

#include <string>

#include <QMessageBox>
#include <Windows.h>

using namespace std;

Chat_SendMessageWidget::Chat_SendMessageWidget(QString _dest,ClientSocket* _socket, QWidget *parent)
	: dest(_dest), socket(_socket), QWidget(parent)
{
	//QT ���ͳ�ʼ��	
	{
		/* QT������ - ���飬�ļ����� */
		tExpression_PushButton = new QPushButton;	//���ͱ��鰴ť
		QIcon tExpression_Icon;
		tExpression_Icon.addFile(ExpressionIcon_FileAddr);
		tExpression_PushButton->setIcon(tExpression_Icon);

		tFile_PushButton = new QPushButton;			//�����ļ���ť
		QIcon tFile_Icon;
		tFile_Icon.addFile(FileIcon_FileAddr);
		tFile_PushButton->setIcon(tFile_Icon);
		
		tDownload = new QPushButton("����");

		tToolBar_HLayout = new QHBoxLayout;			//����������
		tToolBar_HLayout->addWidget(tExpression_PushButton);
		tToolBar_HLayout->addWidget(tFile_PushButton);
		tToolBar_HLayout->addWidget(tDownload);
		tToolBar_HLayout->setAlignment(Qt::AlignLeft);
		
		/* �������ֿ� */
		//SendText_TextEdit = new QTextEdit;			//���뷢�����ֿ�
		SendText_TextEdit = new Image_TextEdit;

		/* ȷ�ϰ�ť/ȡ�� */
		cCancel_PushButton = new QPushButton(tr("ȡ��"));	//ȡ����ť
		cCertify_PushButton = new QPushButton(tr("����"));	//ȷ�ϰ�ť

		cState_HLayout = new QHBoxLayout;					//ȷ��������
		cState_HLayout->addWidget(cCertify_PushButton);
		cState_HLayout->addWidget(cCancel_PushButton);
		cState_HLayout->setAlignment(Qt::AlignRight);

		//�ܲ���
		Layout = new QVBoxLayout(this);
		Layout->addLayout(tToolBar_HLayout, 1);
		Layout->addWidget(SendText_TextEdit, 3);
		Layout->addLayout(cState_HLayout, 1);
	}

	//Qt�¼�
	{
		QObject::connect(
			tFile_PushButton,
			&QPushButton::clicked, 
			this,
			&Chat_SendMessageWidget::SendFileHead
		);

		QObject::connect(
			tDownload,
			&QPushButton::clicked,
			this,
			&Chat_SendMessageWidget::DownLoad
		);

		//QObject::connect(
		//	socket,
		//	&ClientSocket::Signal_DownLoadInfo,
		//	this,
		//	&Chat_SendMessageWidget::GetDownLoad
		//);


	}


	this->setStyleSheet("background-color:transparent;");
//	cCertify_PushButton->setStyleSheet("background-color:rgb(255,249,143);border:2px groove gray;border-radius:10px;padding:2px 4px;");
	cCertify_PushButton->setStyleSheet("background-color:rgb(255,225,228);border:2px groove gray;border-radius:10px;padding:4px 12px;");
	cCancel_PushButton->setStyleSheet("background-color:rgb(255,225,228);border:2px groove gray;border-radius:10px;padding:4px 12px;");


}

Chat_SendMessageWidget::~Chat_SendMessageWidget()
{

}

//�õ����ı��������
QString Chat_SendMessageWidget::GetMessagecs()
{
	QString tmp = SendText_TextEdit->toPlainText();
	return tmp;
}

void Chat_SendMessageWidget::SendFileHead()
{
	//��ȡ�ļ�����
	QString FileName = QFileDialog::getOpenFileName(
		this,
		"���ļ�",
		".//",
		"*.*");

	//�õ����ļ�
	int filelen=readFileIn(FileName.toStdString().c_str(), &file);

	string name = FileName.toStdString();
	name = name.substr(name.find_last_of("/")+1);

	socket->SendFileHead(dest,file.fileLength,name.c_str());

	Sleep(2);

	SendFileBody();
}

void Chat_SendMessageWidget::SendFileBody()
{

	QMessageBox::about(NULL, "Hint", "�����ϴ�");

	int packnum = file.fileLength / 1000 + 1;
	for (int i = 0; i < packnum; i++)
	{
		int tmplen = file.fileLength-i*1000;
		if (tmplen > 1000)
			tmplen = 1000;
		
		socket->SendFileBody(dest, i+1,&file.textHead[i*1000],tmplen);
		
		Sleep(2);
	}
}

void Chat_SendMessageWidget::DownLoad()
{
	QMessageBox::about(NULL,socket->_destu, socket->_file);

	socket->SendFileDownload(socket->_destu, socket->_file);
	QMessageBox::about(NULL, "Hint", "��������");
//	socket->SendFileDownload("test1","test.txt");
}

void Chat_SendMessageWidget::GetDownLoad(QString _user, QString _file)
{
	QMessageBox::about(NULL, _user, _file);

	dnuser = _user;
	dnfile = _file;
}

//#define TEST_GROUPCHAT_SENDMESSAGE
#ifdef TEST_GROUPCHAT_SENDMESSAGE

#include <QtWidgets/QApplication>

void accept(QTcpSocket* socket, string msg)
{
	cout << endl << "----------------" << endl;

	cout << msg << endl;

	if (!socket->waitForReadyRead(120000))
	{
		printf("waitForReadyRead error\n");
		qDebug() << "error=" << socket->errorString();
		return;
	}

	QByteArray date = socket->readAll();
	

}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QWidget c;

	ClientSocket b(&c);
	b.connectServer();

	b.SendRegisterFrame("test1", "123456");
	accept(b.socket, "����֡");
	accept(b.socket, "���ѳ�ʼ��֡");


	Chat_SendMessageWidget test("all",&b);
	test.show();

	return a.exec();
}

#endif // TEST_GROUPCHAT_SENDMESSAGE
