#pragma once

#include <QWidget>
#include <QTcpSocket>

class ClientSocket : public QWidget
{
	Q_OBJECT

public:
	ClientSocket(QWidget *parent = Q_NULLPTR);
	~ClientSocket();

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

	//Socket连接函数
	bool connectServer();

	//Socket断开连接
	void closeConnect();

	//发送帧函数
	void SendRegisterFrame(const QString _username, const QString _secret)const;
	void SendTextFrame(const QString _str, const QString _username)const;
	void SendChangeSecretFrame(const QString _secret)const;
	void SendOffLineFrame()const;

	//文本帧解析
	std::pair<QString, QString> AcceptText(char* TextFrame);
	QString AcceptOnOffLineName(char* OnOffLineFrame);
	QVector<QString> AcceptInitFri(char* InitFriFrame);
		
private slots:

	void onSocketDisconnected();	//信号槽函数，socket连接
	void onSocketReadyRead();		//信号槽函数，socket有数据可读

signals:

	//报道应答帧返回信号 1-成功 2-密码错误 3-用户名不存在 4-需要改密 default:信息错误
	void Signal_RegisterACK(char _type);

	//文本信息应答帧返回信号 0x00 - 发送成功 0xFF - 信息语法有误 0xFE - 好友名称不存在 0xFD - 好友未上线
	void Signal_SendTextACK(char _type);

	//文本信息帧信号
	void Signal_AcceptText(QString _text, QString _from);

	//下线退位帧信号
	void Signal_Abdication();

	//好友上线帧信号
	void Signal_FriendOnline(QString _user);

	//好友下线帧信号
	void Signal_FriendOffline(QString _user);

	//好友初始化信号
	void Signal_InitFriend(QVector<QString> _userlist);
};

