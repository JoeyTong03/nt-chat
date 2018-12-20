#pragma once
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
// //文本应答帧的应答类型
// enum IdentifyTextType
// {
// 	SendSucc = 0,		//成功转发
// 	GramWrong = 0xFF,	//语法错误
// 	FriNotExist = 0xFE,	//好友名称不存在
// 	FriOffLine = 0xFD	//好友已下线
// };
// //报道应答帧的应答类型
// enum IdentifyResultType
// {
// 	Right = 0,			//正确
// 	SecretErr,			//密码错误
// 	NameNoExist,		//用户不存在
// 	NeedUpdateSecret	//需要改密
// };
//将登陆信息写入日志
int WriteReglog(char *name,int RightorErrorType);
//将下线信息写入日志
int WriteOfflinelog(char *name);
//将改密信息写入日志
int WriteChgPswdlog(char *name);
//将发送信息事件写入日志
int WriteSendText(char *SendName, char *RecvName, int RightorErrorType);
//公共信息日志
int WriteAllLog(char*name);