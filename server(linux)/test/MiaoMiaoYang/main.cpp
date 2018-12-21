


#include <QtCore/QCoreApplication>

#include <QTcpSocket>
#include <iostream>
#include <string>

#include "ClientSocket.h"

using namespace std;

void accept(QTcpSocket* socket,string msg)
{
	cout << endl << "----------------" << endl;

	cout <<msg<< endl;

	if (!socket->waitForReadyRead()) 
	{
		printf("waitForReadyRead error\n");
		qDebug() << "error=" << socket->errorString();
		return;
	}

	QByteArray date = socket->readAll();
	Str2int2(date.data(), date.length());

}

void test3()
{
	ClientSocket socket;

	socket.connectServer();

	socket.SendRegisterFrame(QString("MiaoMiaoYang"), QString("123456"));

	accept(socket.socket, "报道应答帧");

	socket.SendChangeSecretFrame(QString("123456"));

	accept(socket.socket, "好友初始化帧");

	accept(socket.socket, "tongjiayan 好友上线帧");

	accept(socket.socket, "tongjiayan 文本消息帧");

	accept(socket.socket, "zhengxuanci 好友上线帧");

	accept(socket.socket, "zhengxuanci 文本消息帧");
}

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	ClientSocket socket;

	socket.connectServer();

	socket.SendRegisterFrame(QString("MiaoMiaoYang"), QString("123456"));

	accept(socket.socket, "报道应答帧");

	socket.SendChangeSecretFrame(QString("123456"));

	accept(socket.socket, "好友初始化帧");

	accept(socket.socket, "tongjiayan 好友上线帧");

	accept(socket.socket, "tongjiayan 文本消息帧");



	return a.exec();

}
