
#include "ClientSocket.h"

#include <QMessageBox>
#include <QString>
#include <string>
#include "UTF8_GBK.hpp"
#include <QFile>

ClientSocket::ClientSocket(QWidget *parent)
	: QDialog(parent)
{

}

ClientSocket::~ClientSocket()
{
	closeConnect();
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
	socket = new QTcpSocket(this);
	socket->abort();

	//连接主机
	socket->connectToHost(ip, port);
	if (!socket->waitForConnected(10000)) //正式连接等待30s
	{
		//等待连接成功
		qDebug() << "connect failed";
		return false;
	}

	//Qt Socket信号槽函数
	{
		connect(socket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnected()));
		connect(socket, SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()));
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
	memcpy(buf + 2, &FrameLength, 2);


	//用户名 - 16字节
	memcpy(&(buf[4]), _username.toStdString().c_str(), _username.length());

	//密码 - 12字节
	memcpy(&(buf[20]), _secret.toStdString().c_str(), _secret.length());

	//主机序转网络序


	socket->write(buf, sizeof(buf));
	socket->waitForBytesWritten();


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
void ClientSocket::SendTextFrame(const QString _str, QString DestUsername)const
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

	socket->waitForBytesWritten();

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
	socket->waitForBytesWritten();

}

QString Str2int2(char*buf, int length)
{
	char result[200];
	sprintf(result, "%x ", (int)buf[0]);

	int i;
	for (i = 1; i < length; i++)
	{
		if ((buf[i] >= 'a'&&buf[i] <= 'z') || (buf[i] >= 'A' && buf[i] <= 'Z') || (buf[i] >= '0'&&buf[i] <= '9') || (buf[i] == '@'))
			sprintf(result, "%s%c ", result, buf[i]);
		else
			sprintf(result, "%s%x ", result, (int)buf[i]);
		// if(i!=0&&i%4==0)
		//     printf(" ");
	}
	sprintf(result, "%s%c ", result, '\n');
	return QString(result);
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
	socket->waitForBytesWritten();

}

/***************************************************************************
函数名称：AcceptText
功    能：解析文本信息帧
输入参数：文本信息帧，信息来源用户名
返 回 值：解析文本，来源用户
说    明：
***************************************************************************/
std::pair<QString, QString> ClientSocket::AcceptText(char* TextFrame)
{
	QString Text, FromName;
	Text.clear();
	FromName.clear();

	uint16_t TextLength;
	memcpy(&TextLength, TextFrame + 2, 2);
	FromName = QString(QLatin1String(TextFrame + 5));
	Text = QString::fromStdString(GBKToUTF8(TextFrame + 6 + FromName.size()));

	return std::pair<QString, QString>(Text, FromName);
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
QVector<QString> ClientSocket::AcceptInitFri(QByteArray InitFriFrame)
{
	QVector<QString> NameList;

	//好友的数量
	uint8_t FriNum = 0;
	memcpy(&FriNum, InitFriFrame.data() + 1, 1);

	//帧长
	int len = InitFriFrame.length();

	QByteArray tmp = InitFriFrame;
	tmp.remove(0, 4);

	std::string str = tmp.toStdString();
	QString name;
	for (int i = 1; i < str.length(); i++)
	{
		if (str[i] == '@' || str[i] == '#')
		{
			NameList.push_back(name);
			name.clear();

			if (str[i] == '@')
				continue;
			else
				break;
		}

		name += str[i];

	}

	NameList.push_back(name);

	return NameList;
}

/***************************************************************************
函数名称：onSocketDisconnect
功    能：当服务端抛弃这个连接的情况下，将这个socket断开
输入参数：
返 回 值：
说    明：
***************************************************************************/
void ClientSocket::onSocketDisconnected()
{
	socket->abort();
}

/***************************************************************************
函数名称：onSocketDisconnect
功    能：当服务端抛弃这个连接的情况下，将这个socket断开
输入参数：
返 回 值：
说    明：
***************************************************************************/
void ClientSocket::onSocketReadyRead()
{
	QByteArray data = socket->readAll();

	int length = data.length();
	char* str = new char[length];
	memcpy(str, data.data(), length);

	//解析不同帧
	{
		unsigned char head = str[0];

		switch (head) 
		{

			//发来的是报道帧的处理
		case 0x71:
		{
			char type = str[1];
			emit Signal_RegisterACK(type);

			break;
		}

		//文本信息应答帧的处理
		case 0x72:
		{

			char type = str[1];
			emit Signal_SendTextACK(type);

			break;
		}

		//文本信息帧的处理
		case 0x77:
		{

			//text,from_user
			std::pair<QString, QString> Text = AcceptText(str);//文本信息
			emit Signal_AcceptText(Text.first, Text.second);

			break;
		}

		//下线退位帧的处理
		case 0x73:
		{
			emit Signal_Abdication();
			break;
		}

		//好友下线/上线帧的处理
		case 0x75:
		{
			QString name = AcceptOnOffLineName(str);//上线或下线用户名
			if ((int)str[1] == 0)
			{
				//上线
				Signal_FriendOnline(name);
			}
			else
			{
				//下线
				Signal_FriendOffline(name);
			}

			break;
		}

		//好友初始化帧
		case 0x76:
		{
			QVector<QString>namelist = AcceptInitFri(data);
			emit Signal_InitFriend(namelist);
			break;
		}

		case 0x80:
		{
			emit Signal_InitFile();
			break;
		}

		case 0x82:
		{
			uint8_t acceptType;
			QString _username;
			uint32_t Filelen;
			uint32_t Packnum;
			QString _filename;
			AcceptFileInform((FrameS2CFileInform*)data.data(), acceptType, _username, Filelen, Packnum, _filename);
			
			_destu = _username;
			_file = _filename;

			break;
		}

		case 0x83:
		{
			QString _user;
			uint32_t len;
			uint32_t num; //包的数量
			QString _file;
			AcceptFileHead((FrameS2CFileHead*)data.data(), _user, len, num, _file);

			SendFileHeadReply(AcceptOk);

			break;
		}
		

		case 0x84:
		{

			QString name;
			uint32_t packnum = 0;
			QString Filecont;
			uint32_t len=0;
			AcceptFileBody((FrameS2CFileBody*)data.data(), name, packnum, Filecont, len);

			SendFileBodyReply(packnum);

			QFile file(name);
			if (!file.open(QIODevice::ReadWrite | QIODevice::Text| QIODevice::Append))
			{
				QMessageBox::warning(this, "file write", "can't open", QMessageBox::Yes);
			}
			QTextStream in(&file);
			in << Filecont;

			file.close();

			break;
		}

		default:
			//			QMessageBox::about(NULL, "Warning!!!", "接收到非法消息");
			break;
		}
	}


	delete str;

}
