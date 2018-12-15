#include "ClientSocket.h"



ClientSocket::ClientSocket()
{

}

ClientSocket::~ClientSocket()
{

}

/*************************************
 * �������ƣ�connectServer
 * ��    �ܣ��ͻ������ӷ�����
 * ��    ����
 * ��    �أ�bool - �Ƿ���ȷ����
 * ˵    ����ip/port�������й̶�Ϊ����
*************************************/
bool ClientSocket::connectServer()
{
	//�����µ�Socket�׽���
	socket = new QTcpSocket;
	socket->abort();

	//��������
	socket->connectToHost(ip, port);
	if (!socket->waitForConnected(30000)) //��ʽ���ӵȴ�3s
	{
		//�ȴ����ӳɹ�
		qDebug() << "connect failed";
		return false;
	}

	return true;
}

/*************************************
 * �������ƣ�closeServer
 * ��    �ܣ��ͻ��˶Ͽ�����
 * ��    ����
 * ��    �أ�
 * ˵    ����
*************************************/
void ClientSocket::closeConnect()
{
	socket->close();

}

/*************************************
 * �������ƣ�SendRegisterFrame
 * ��    �ܣ����ͱ���֡
 * ��    ����_username - �û��� secret - ����
 * ��    �أ�
 * ˵    ��������֡ - 32�ֽڣ�4�ֽ�֡ͷ+16�ֽ��û���+12�ֽ�����
 *			 ֡ͷ [0-7] 0x11 [8-15] 0x0 [16-31] 0x0020(���ĳ���-32byte)
*************************************/
char* ClientSocket::SendRegisterFrame(const QString _username, const QString _secret) const
{
	//32�ֽڱ���
	char buf[32] = { 0 };

	//֡ͷ[0-7]
	memcpy(&(buf[0]), &SendFrameHead[SfhRegister], sizeof(char));

	//���ĳ���[16-31]
	char FrameLength = 0x20;
	memcpy(&(buf[3]), &FrameLength, sizeof(char));

	//�û��� - 16�ֽ�
	memcpy(&(buf[4]), _username.toStdString().c_str(), _username.length());

	//���� - 12�ֽ�
	memcpy(&(buf[19]), _secret.toStdString().c_str(), _secret.length());

	return buf;

//	socket->write(buf, sizeof(buf));


}

