


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

	accept(socket.socket, "����Ӧ��֡");

	socket.SendChangeSecretFrame(QString("123456"));

	accept(socket.socket, "���ѳ�ʼ��֡");

	accept(socket.socket, "tongjiayan ��������֡");

	accept(socket.socket, "tongjiayan �ı���Ϣ֡");

	accept(socket.socket, "zhengxuanci ��������֡");

	accept(socket.socket, "zhengxuanci �ı���Ϣ֡");
}

void test2()
{
	ClientSocket socket;

	socket.connectServer();

	socket.SendRegisterFrame(QString("MiaoMiaoYang"), QString("123456"));

	accept(socket.socket, "����Ӧ��֡");

	socket.SendChangeSecretFrame(QString("654321"));

	accept(socket.socket, "���ѳ�ʼ��֡");

	accept(socket.socket, "tongjiayan ��������֡");

	accept(socket.socket, "tongjiayan �ı���Ϣ֡");
}

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	test3();

	return a.exec();

}
