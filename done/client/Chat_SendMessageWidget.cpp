

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
	//QT 类型初始化	
	{
		/* QT工具栏 - 表情，文件…… */
		tExpression_PushButton = new QPushButton;	//发送表情按钮
		QIcon tExpression_Icon;
		tExpression_Icon.addFile(ExpressionIcon_FileAddr);
		tExpression_PushButton->setIcon(tExpression_Icon);

		tFile_PushButton = new QPushButton;			//传输文件按钮
		QIcon tFile_Icon;
		tFile_Icon.addFile(FileIcon_FileAddr);
		tFile_PushButton->setIcon(tFile_Icon);
		
		tDownload = new QPushButton("下载");

		tToolBar_HLayout = new QHBoxLayout;			//工具栏布局
		tToolBar_HLayout->addWidget(tExpression_PushButton);
		tToolBar_HLayout->addWidget(tFile_PushButton);
		tToolBar_HLayout->addWidget(tDownload);
		tToolBar_HLayout->setAlignment(Qt::AlignLeft);
		
		/* 发送文字框 */
		//SendText_TextEdit = new QTextEdit;			//输入发送文字框
		SendText_TextEdit = new Image_TextEdit;

		/* 确认按钮/取消 */
		cCancel_PushButton = new QPushButton(tr("取消"));	//取消按钮
		cCertify_PushButton = new QPushButton(tr("发送"));	//确认按钮

		cState_HLayout = new QHBoxLayout;					//确认栏布局
		cState_HLayout->addWidget(cCertify_PushButton);
		cState_HLayout->addWidget(cCancel_PushButton);
		cState_HLayout->setAlignment(Qt::AlignRight);

		//总布局
		Layout = new QVBoxLayout(this);
		Layout->addLayout(tToolBar_HLayout, 1);
		Layout->addWidget(SendText_TextEdit, 3);
		Layout->addLayout(cState_HLayout, 1);
	}

	//Qt事件
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

//得到了文本框的内容
QString Chat_SendMessageWidget::GetMessagecs()
{
	QString tmp = SendText_TextEdit->toPlainText();
	return tmp;
}

void Chat_SendMessageWidget::SendFileHead()
{
	//读取文件名字
	QString FileName = QFileDialog::getOpenFileName(
		this,
		"打开文件",
		".//",
		"*.*");

	//得到的文件
	int filelen=readFileIn(FileName.toStdString().c_str(), &file);

	string name = FileName.toStdString();
	name = name.substr(name.find_last_of("/")+1);

	socket->SendFileHead(dest,file.fileLength,name.c_str());

	Sleep(2);

	SendFileBody();
}

void Chat_SendMessageWidget::SendFileBody()
{

	QMessageBox::about(NULL, "Hint", "正在上传");

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
	QMessageBox::about(NULL, "Hint", "正在下载");
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
	accept(b.socket, "报道帧");
	accept(b.socket, "好友初始化帧");


	Chat_SendMessageWidget test("all",&b);
	test.show();

	return a.exec();
}

#endif // TEST_GROUPCHAT_SENDMESSAGE
