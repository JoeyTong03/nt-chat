#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
//S->C2的文本信息帧中判断群发或向个人发送标记
//C收到来自S的文本信息帧，观察标志位[8-15]
enum SglOrAll
{
	Single = 0x00,
	All = 0x01
};
//解析报道帧时的结果类型
enum IdentifyResultType
{
	Right=1,
	SecretErr,
	NameNoExist,
	NeedUpdateSecret,
	TextFrameRecvFail
};
//文本应答帧的应答类型
enum IdentifyTextType
{
	SendSucc = 0,		//成功转发
	GramWrong = 0xFF,	//语法错误
	FriNotExist = 0xFE,	//好友名称不存在
	FriOffLine = 0xFD	//好友已下线
};
//C->S帧类型
enum SendFrameHeadType
{
	SfhRegister = 0x11,			//报到帧
	SfhText = 0x12,				//文本信息帧
	SfhChangeSecret = 0x15,		//改密帧
	SfhOffLine = 0x16,			//下线帧
	SfhOnLine = 0x18,			//上线帧
	SfhFileHead = 0x20,			//文件信息帧1(传送文件名等文件头信息)
	SfhFileBody = 0x21,			//文件信息帧2(传送文件内容)
	SfhFileDownload = 0x22,		//下载通知帧(客户端请求下载)
	SfhFileHeadReply = 0x23,	//文件信息帧1应答帧
	SfhFileBodyReply = 0x24		//文本信息帧2应答帧
};
//S->C帧类型
enum ReceiveFrameHeadType
{
	RfhRegister = 0x71,			//报到应答帧
	RfhTextReply = 0x72,		//文本应答帧
	RfhText = 0x77,				//文本信息帧
	RfhBeKicked = 0x73,			//下线退位帧
	RfhOnOffLine = 0x75,		//上/下线帧
	RfhInit = 0x76,				//好友初始化帧
	RfhFileHeadReply = 0x80,	//文件信息帧1应答帧
	RfhFileBodyReply = 0x81,	//文本信息帧2应答帧
	RfhFileInform = 0x82,		//传输文件消息帧
	RfhFileHead = 0x83,			//文件信息帧1(传送文件名等文件头信息)
	RfhFileBody = 0x84			//文件信息帧2(传送文件内容)
};
void Str2int2(char *buf, int length);


/********************C->S的帧解析部分*******************/
//根据收到的帧，返回属于哪个帧类型：
//返回值为统一的枚举值
int getType(char buf[]);

//解析报道帧
int analysisSfhRegister(char buf[], char username[], char secret[]);

//解析改密帧
int analysisSfhChangeSecret(char buf[], char newSecret[]);

//解析文本信息帧
int analysisSfhText(char buf[], char name[], char**Text);

//解析上线帧
/********************S->C的帧生成部分*******************/
//根据replyType生成应答帧replyFrame
int initReplyFrame(int replyType, char**replyFrame);

//生成文本应答帧文本（应答类型）
int CrtTextReplyFrame(int replyType, char**TextReplyFrame);

//生成文本信息帧（转发者用户名，文本信息）
int CrtTextFrame(char *name, char *text, char **TextFrame, uint8_t SingleOrAll);

//生成下线退位帧
int CrtOffLineFrame(char**OffLineFrame);

//生成上/下线帧(上线或下线用户名，上线/下线状态)
int CrtOnOffFrame(char*name,int OnorOff,char**OnOffFrame);

//生成好友初始化帧
int CrtFriInit(char* NameList, char**FriInitFrame);

/********************父子进程传帧*******************/

//生成上线帧，将用户名包裹进去
int CrtOnLineFrame(char username[], char**frOnline);

//解析上线帧,获得上线的用户名
int analysisSfhOnOffLine(char buf[], char username[]);
