#pragma once

#include <QTcpSocket>
#include <QtEndian>
#include <QVector>

class ClientSocket
{
private:

	//发送帧类型
	enum SendFrameHeadType
	{
		SfhRegister = 0,	//报到帧
		SfhText,			//文本信息帧
		SfhChangeSecret,	//改密帧
		SfhOffLine			//下线帧
	};

	//发送帧头
	const char SendFrameHead[4] = {
		0x11,	//报到帧
		0x12,	//文本信息帧
		0x15,	//改密帧
		0x16	//下线帧
	};

	//接受帧类型
	enum ReceiveFrameHeadType
	{
		RfhRegister = 0,	//报到帧
		RfhTextReply,		//文本应答帧
		RfhText,			//文本信息帧
		RfhBeKicked,		//下线退位帧
		RfhOnOffLine,		//上/下线帧
		RfhInit				//好友初始化帧
	};

	//接受帧头
	const char ReceiveFrameHead[6] = {
		0x71,	//报到帧
		0x72,	//文本应答帧
		0x77,   //文本信息帧
		0x73,	//下线退位帧
		0x75,	//上/下线帧
		0x76	//好友初始化帧
	};

	//长度限制
	const int USERNAME_MAXSIZE = 16;	//用户名最长为16字节
	const int SECRET_MAXSIZE = 12;		//密码最长为12字节

	//网络相关配置
	const QString ip = "192.168.80.230";
	const int port = 4000;

	//QTcpSocket
	QTcpSocket* socket;

public:
	ClientSocket();
	~ClientSocket();

	//Socket连接函数
	bool connectServer();

	//Socket断开连接
	void closeConnect();

	//发送帧函数
	void SendRegisterFrame(const QString _username, const QString _secret)const;
	void SendTextFrame(const QString _str, const QString _username)const;
	void SendChangeSecretFrame(const QString _secret)const;
	void SendOffLineFrame()const;

	//解析帧头函数
	void AnlsFrame(char*buf);

	//文本帧解析
	QString AcceptText(char* TextFrame, QString&FromName);
	QString AcceptOnOffLineName(char* OnOffLineFrame);
	QVector<QString> AcceptInitFri(char* InitFriFrame);

	friend class MainWidget;
};

