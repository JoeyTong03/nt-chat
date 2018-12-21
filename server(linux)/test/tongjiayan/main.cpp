


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

	socket.SendRegisterFrame(QString("tongjiayan"), QString("123456"));

	accept(socket.socket, "����Ӧ��֡");

	socket.SendChangeSecretFrame(QString("123456"));

	accept(socket.socket, "���ѳ�ʼ��֡");

	socket.SendTextFrame(QString("This is message from tongjiayan"), QString("MiaoMiaoYang"));

	accept(socket.socket, "zhengxuanci ��������֡");

	accept(socket.socket, "zhengxuanci �ı���Ϣ֡");

}

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);


	ClientSocket socket;

	socket.connectServer();

	socket.SendRegisterFrame(QString("tongjiayan"), QString("123456"));

	accept(socket.socket, "����Ӧ��֡");

	socket.SendChangeSecretFrame(QString("123456"));

	accept(socket.socket, "���ѳ�ʼ��֡");

	socket.SendTextFrame(QString("This is message from tongjiayan"), QString("MiaoMiaoYang"));

	accept(socket.socket, "��ϢӦ��֡");


	return a.exec();

}
