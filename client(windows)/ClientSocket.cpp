#include "ClientSocket.h"
#include<iomanip>
#include<iostream>
using namespace std;
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
	if (!socket->waitForConnected(3000)) //正式连接等待3s
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

/***************************************************************************
函数名称：Str2int
功    能：显示帧
输入参数：buf:帧,length:帧长度
返 回 值：
说    明：
***************************************************************************/
void Str2int(char*buf, int length)
{
	int i;
	for (i = 0; i<length; i++)
	{
		if ((buf[i] >= 'a'&&buf[i] <= 'z') || (buf[i] >= 'A' && buf[i] <= 'Z') || (buf[i] >= '0'&&buf[i] <= '9'))
			printf("%c ", buf[i]);
		else
			printf("%x ", (int)buf[i]);
		// if(i!=0&&i%4==0)
		//     printf(" ");
	}
	printf("%c", '\n');
}
/*************************************
 * 函数名称：SendRegisterFrame
 * 功    能：发送报道帧
 * 参    数：_username - 用户名 secret - 密码
 * 返    回：
 * 说    明：报道帧 - 32字节：4字节帧头+16字节用户名+12字节密码
 *			 帧头 [0-7] 0x11 [8-15] 0x0 [16-31] 0x0020(报文长度-32byte)
*************************************/
void ClientSocket::SendRegisterFrame(const QString _username, const QString _secret) const
{
	//32字节报文
	char buf[32] = { 0 };

	//帧头[0-7]
	memcpy(&(buf[0]), &SendFrameHead[SfhRegister], sizeof(char));

	//报文长度[16-31]??
	//char FrameLength = 0x20;
	//memcpy(&(buf[3]), &FrameLength, sizeof(char));
	uint16_t FrameLength = 32;
	memcpy(buf+2,&FrameLength,2);


	//用户名 - 16字节
	memcpy(&(buf[4]), _username.toStdString().c_str(), _username.length());

	//密码 - 12字节
	memcpy(&(buf[20]), _secret.toStdString().c_str(), _secret.length());

	//主机序转网络序


	socket->write(buf, sizeof(buf));

	//Str2int(buf, 32);

}
/***************************************************************************
函数名称：SendTextFrame
功    能：发送文本信息帧
输入参数：_str：文本信息
		  DestUsername：目的用户名
返 回 值：发送成功返回1，失败返回0
说    明：4字节帧头+@目的用户名(用户名或all)+'\0'+文本信息
***************************************************************************/
void ClientSocket::SendTextFrame(const QString _str,QString DestUsername)const
{
	uint16_t FrameLength = 4 + 1 + DestUsername.length() + _str.length() + 2;
	char* buf = new char[FrameLength];
	if (FrameLength > 1024)//文本信息长度限制
	{
		qDebug() << "Length Limit" << endl;
		return;
	}
	memset(buf, '\0', FrameLength);
	//帧头
	memcpy(buf, SendFrameHead + SfhText, sizeof(char));
	//报文长度
	memcpy(buf + 2, &FrameLength, sizeof(uint16_t));
	//加入目标客户端
	char*name = new char[2 + DestUsername.length()];
	strcpy(name + 1, DestUsername.toStdString().data());
	name[0] = '@';
	memcpy(buf + 4, name, 1 + DestUsername.length());
	//文本信息
	memcpy(buf + 6 + DestUsername.length(), _str.toStdString().c_str(), _str.length());

	socket->write(buf, FrameLength);

}
/***************************************************************************
函数名称：SendChangeSecretFrame
功    能：发送改密帧
输入参数：新的密码值
返 回 值：
说    明：16字节（4字节帧头+12字节修改后密码）
***************************************************************************/
void ClientSocket::SendChangeSecretFrame(const QString _secret)const
{
	char* buf = new char[16];
	uint16_t FrameLength = 16;
	memset(buf, '\0', FrameLength);
	//帧头
	memcpy(buf, SendFrameHead + SfhChangeSecret, sizeof(char));
	//报文长度
	memcpy(buf + 2, &FrameLength, sizeof(uint16_t));
	//加入新密码
	memcpy(buf + 4, _secret.toStdString().c_str(), _secret.length());

	socket->write(buf, FrameLength);

}
/***************************************************************************
函数名称：SendOffLineFrame
功    能：client端向server端发送下线帧
输入参数：
返 回 值：
说    明：
***************************************************************************/
void ClientSocket::SendOffLineFrame()const
{
	uint16_t FrameLength = 4;
	char*buf = new char[FrameLength];
	memset(buf, '\0', FrameLength);
	//帧头
	memcpy(buf, SendFrameHead + SfhOffLine, sizeof(char));
	//报文长度
	memcpy(buf + 2, &FrameLength, sizeof(uint16_t));

	socket->write(buf, FrameLength);

}
/***************************************************************************
函数名称：AnlsFrame
功    能：解析帧头
输入参数：来自server的帧
返 回 值：
说    明：@杨小宇要修改的部分，在所有case后面有注释的地方加入每种情况client需要作出的处理，
		帧解析后的有效内容在每个case内部，具体内容看注释
***************************************************************************/
void ClientSocket::AnlsFrame(char*buf)
{
	switch ((int)buf[0])
	{
	case 0x71://报道帧
		switch ((int)buf[1])
		{
		case 0://登陆成功
			//
			break;
		case 1://密码错误
			//
			break;
		case 2://用户名不存在
			//
			break;
		case 3://是否需要改密
			//
			break;
		default:
			break;
		}
		break;
	case 0x72://文本信息应答帧
		switch ((int)buf[1])
		{
		case 0x00://转发成功
			//
			break;
		case 0xFF://信息语法有误
			//
			break;
		case 0xFE://好友名称不存在
			//
			break;
		case 0xFD://好友未上线
			//
			break;
		default:
			break;
		}
		break;
	case 0x77://文本信息帧（从server端转发而来的）
	{
		QString FromName;//信息来源
		QString Text = AcceptText(buf, FromName);//文本信息
		//处理文本信息
		break;
	}
	case 0x73://下线退位帧
		//下线退位
		break;
	case 0x75://下线/上线帧
	{
		QString name = AcceptOnOffLineName(buf);//上线或下线用户名
		if ((int)buf[1] == 0)
		{
			//上线
		}
		else
		{
			//下线
		}
		break;
	}
	case 0x76://好友初始化
	{
		QVector<QString>namelist = AcceptInitFri(buf);
		//namelist为好友列表(vector)
		//
		break;
	}
	default:
		break;
	}
}
/***************************************************************************
函数名称：AcceptText
功    能：解析文本信息帧
输入参数：文本信息帧，信息来源用户名
返 回 值：解析文本
说    明：
***************************************************************************/
QString ClientSocket::AcceptText(char* TextFrame,QString&FromName)
{
	QString Text;
	Text.clear();
	FromName.clear();
	uint16_t TextLength;
	memcpy(&TextLength, TextFrame + 2, 2);
	FromName = QString(QLatin1String(TextFrame + 5));
	Text = QString(QLatin1String(TextFrame + 6 + FromName.size()));
	return Text;
}
/***************************************************************************
函数名称：AcceptOnOffLineName
功    能：解析上/下线帧
输入参数：上下线帧,
返 回 值：上线或下线用户名，上线或下线(0x0上线，0x1下线)
说    明：
***************************************************************************/
QString ClientSocket::AcceptOnOffLineName(char* OnOffLineFrame)
{
	QString name = QString(QLatin1String(OnOffLineFrame + 4));
	//OnOffLine = (int)OnOffLineFrame[1];
	return name;
}
/***************************************************************************
函数名称：AcceptInitFri
功    能：解析好友初始化帧
输入参数：好友初始化帧，在线用户名列表
返 回 值：
说    明：
***************************************************************************/
QVector<QString> ClientSocket::AcceptInitFri(char* InitFriFrame)
{
	QVector<QString> NameList;
	uint8_t FriNum;
	memcpy(&FriNum, InitFriFrame + 1, 1);
	int startloc = 5;
	for (int i = 0; i < FriNum; i++)
	{
		QString name = QString(QLatin1String(InitFriFrame + startloc));
		NameList.push_back(name);
		InitFriFrame += name.size() + 2;
	}
	return NameList;
}
//目前问题
//ntohs