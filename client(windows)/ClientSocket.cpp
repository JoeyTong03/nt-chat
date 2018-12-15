#include "ClientSocket.h"



ClientSocket::ClientSocket()
{

}

ClientSocket::~ClientSocket()
{

}

/*************************************
 * 函数名称：connectServer
 * 功    能：客户端连接服务器
 * 参    数：
 * 返    回：bool - 是否正确连接
 * 说    明：ip/port均在类中固定为常量
*************************************/
bool ClientSocket::connectServer()
{
	//创建新的Socket套接字
	socket = new QTcpSocket;
	socket->abort();

	//连接主机
	socket->connectToHost(ip, port);
	if (!socket->waitForConnected(30000)) //正式连接等待3s
	{
		//等待连接成功
		qDebug() << "connect failed";
		return false;
	}

	return true;
}

/*************************************
 * 函数名称：closeServer
 * 功    能：客户端断开连接
 * 参    数：
 * 返    回：
 * 说    明：
*************************************/
void ClientSocket::closeConnect()
{
	socket->close();

}

/*************************************
 * 函数名称：SendRegisterFrame
 * 功    能：发送报道帧
 * 参    数：_username - 用户名 secret - 密码
 * 返    回：
 * 说    明：报道帧 - 32字节：4字节帧头+16字节用户名+12字节密码
 *			 帧头 [0-7] 0x11 [8-15] 0x0 [16-31] 0x0020(报文长度-32byte)
*************************************/
char* ClientSocket::SendRegisterFrame(const QString _username, const QString _secret) const
{
	//32字节报文
	char buf[32] = { 0 };

	//帧头[0-7]
	memcpy(&(buf[0]), &SendFrameHead[SfhRegister], sizeof(char));

	//报文长度[16-31]
	char FrameLength = 0x20;
	memcpy(&(buf[3]), &FrameLength, sizeof(char));

	//用户名 - 16字节
	memcpy(&(buf[4]), _username.toStdString().c_str(), _username.length());

	//密码 - 12字节
	memcpy(&(buf[19]), _secret.toStdString().c_str(), _secret.length());

	return buf;

//	socket->write(buf, sizeof(buf));


}

