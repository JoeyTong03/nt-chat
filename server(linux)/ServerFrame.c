#include"ServerFrame.h"

void Str2int2(char*buf, int length)
{
	int i;
	for (i = 0; i<length; i++)
	{
		if ((buf[i] >= 'a'&&buf[i] <= 'z') || (buf[i] >= 'A' && buf[i] <= 'Z') || (buf[i] >= '0'&&buf[i] <= '9')||(buf[i]=='@'))
			printf("%c ", buf[i]);
		else
			printf("%x ", (int)buf[i]);
		// if(i!=0&&i%4==0)
		//     printf(" ");
	}
	printf("%c", '\n');
}

/***************************************************************************
函数名称：getType
功    能：根据帧头，返回帧的功能对应枚举值
输入参数：帧
返 回 值：帧头枚举值
说    明：
***************************************************************************/
int getType(char buf[])
{
	return (int)buf[0];
}
/***************************************************************************
函数名称：analysisSfhRegister
功    能：解析报道帧
输入参数：报道帧，client对应用户名,密码指针
返 回 值
说    明：使用函数时，帧以字符串形式输入，其余两个参数均已指针形式输入（在函数赋予空间）
***************************************************************************/
int analysisSfhRegister(char buf[], char username[], char secret[])
{
	//username = (char*)malloc(16 * sizeof(char));
	//secret = (char*)malloc(12 * sizeof(char));
	memset(username, 0, 16);
	memset(secret, 0, 12);
	strcpy(username, buf + 4);
	strcpy(secret, buf + 20);
	return 1;
}
/***************************************************************************
函数名称：analysisSfhChangeSecret
功    能：解析改密帧
输入参数：改密帧，新密码指针
返 回 值：
说    明：
***************************************************************************/
int analysisSfhChangeSecret(char buf[], char newSecret[])
{
	//newSecret = (char*)malloc(12 * sizeof(char));
	memset(newSecret, 0, 12);
	strcpy(newSecret, buf + 4);
	return 1;
}
/***************************************************************************
函数名称：analysisSfhText
功    能：解析文本信息帧
输入参数：帧，来源client用户名，文本内容二级指针
返 回 值：
说    明：在函数外将定义文本信息的指针，并用一个二级指针指向该指针，作为输入的参数
***************************************************************************/
int analysisSfhText(char buf[], char name[], char**Text)
{
	//name = (char*)malloc(16 * sizeof(char));
	memset(name, 0, 16);
	strcpy(name, buf + 5);
	uint16_t TxtLength;
	memcpy(&TxtLength, buf + 2, 2);
	TxtLength -= (strlen(name) + 6);
	*Text = (char*)malloc(TxtLength * sizeof(char));
	memset(*Text, 0, TxtLength);
	strcpy(*Text, buf + 6 + strlen(name));
	return 1;
}

/********************S->C的帧生成部分*******************/
/***************************************************************************
函数名称：initReplyFrame
功    能：根据ReplyType生成应答帧replyFrame
输入参数：应答类型，应答帧
返 回 值：帧的长度
说    明：
***************************************************************************/
int initReplyFrame(int ReplyType, char**ReplyFrame)
{
	*ReplyFrame = (char*)malloc(4 * sizeof(char));
	memset(*ReplyFrame, 0, 4);
	(*ReplyFrame)[0] = (char)0x71;
	(*ReplyFrame)[1] = (char)ReplyType;
	uint16_t length = 4;
	memcpy((*ReplyFrame) + 2, &length, 2);
	return 4;
}
/***************************************************************************
函数名称：CrtTextReplyFrame
功    能：生成文本应答帧文本（应答类型）
输入参数：应答类型，应答帧
返 回 值：帧的长度
说    明：
***************************************************************************/
int CrtTextReplyFrame(int ReplyType, char**TextReplyFrame)
{
	*TextReplyFrame = (char*)malloc(4 * sizeof(char));
	memset(*TextReplyFrame, 0, 4);
	(*TextReplyFrame)[0] = (char)0x72;
	(*TextReplyFrame)[1] = (char)ReplyType;
	uint16_t length = 4;
	memcpy((*TextReplyFrame) + 2, &length, 2);
	return 4;
}
/***************************************************************************
函数名称：CrtTextFrame
功    能：生成文本信息帧（转发者用户名，文本信息）
输入参数：文本信息源的用户名，文本信息，文本信息帧
返 回 值：帧的长度
说    明：
***************************************************************************/
int CrtTextFrame(char*name, char*text, char**TextFrame)
{
	uint16_t FrameLength = (uint16_t)strlen(name) + strlen(text) + 7;
	*TextFrame = (char*)malloc(FrameLength * sizeof(char));
	memset(*TextFrame, 0, FrameLength);
	(*TextFrame)[0] = (char)0x77;
	memcpy((*TextFrame) + 2, &FrameLength, 2);
	(*TextFrame)[4] = '@';
	memcpy((*TextFrame) + 5, name, strlen(name) + 1);
	memcpy((*TextFrame) + 6 + strlen(name), text, strlen(text) + 1);
	return (int)FrameLength;
}
/***************************************************************************
函数名称：CrtOffLineFrame
功    能：生成下线退位帧
输入参数：帧
返 回 值：帧的长度
说    明：
***************************************************************************/
int CrtOffLineFrame(char**OffLineFrame)
{
	*OffLineFrame = (char*)malloc(4 * sizeof(char));
	memset(*OffLineFrame, 0, 4);
	*OffLineFrame[0] = (char)0x73;
	uint16_t length = 4;
	memcpy((*OffLineFrame) + 2, &length, 2);
	return 4;
}
/***************************************************************************
函数名称：CrtOnOffFrame
功    能：生成上/下线帧(上线或下线用户名，上线/下线状态)
输入参数：上线或下线的用户名，上线/下线标记，帧
返 回 值：帧的长度
说    明：
***************************************************************************/
int CrtOnOffFrame(char*name, int OnorOff, char**OnOffFrame)
{
	uint16_t length = 32;
	*OnOffFrame = (char*)malloc(length * sizeof(char));
	memset(*OnOffFrame, 0, 32);
	(*OnOffFrame)[0] = (char)0x75;
	memcpy((*OnOffFrame) + 2, &length, 2);
	memcpy((*OnOffFrame) + 4, name, strlen(name));
	return 32;
}
/***************************************************************************
函数名称：CrtFriInit
功    能：生成好友初始化帧
输入参数：用户名文本，帧
返 回 值：帧的长度
说    明：
***************************************************************************/
int CrtFriInit(char* NameList, char**FriInitFrame)
{
	uint8_t FriNum = 0;
	uint16_t length = 0;
	for (; NameList[length] == '@';)
	{
		length++;
		for (; NameList[length] != '\0'; length++)
			;
		length++;
		FriNum++;
	}
	length += 5;
	*FriInitFrame = (char*)malloc(length * sizeof(char));
	memset(*FriInitFrame, 0, length);
	(*FriInitFrame)[0] = (char)0x76;
	memcpy((*FriInitFrame) + 1, &FriNum, 1);
	memcpy((*FriInitFrame) + 2, &length, 2);
	memcpy((*FriInitFrame) + 4, NameList, length - 4);
	return (int)(length);
}
/***************************************************************************
函数名称：CrtOnLineFrame
功    能：生成上线帧，将用户名包裹进去
输入参数：用户名,帧
返 回 值：帧的长度
说    明：
***************************************************************************/
int CrtOnLineFrame(char username[], char**frOnline)
{
	uint16_t length = 32;
	*frOnline = (char*)malloc(length * sizeof(char));
	memset(*frOnline, 0, length);
	(*frOnline)[0] = (char)0x77;
	memcpy((*frOnline) + 2, &length, 2);
	memcpy((*frOnline) + 4, username, 16);
	return 32;
}
/***************************************************************************
函数名称：analysisSfhOnLine
功    能：解析上线帧,获得上线的用户名
输入参数：帧，用户名
返 回 值：
说    明：
***************************************************************************/
int analysisSfhOnOffLine(char buf[], char username[])
{
	memset(username, 0, 16);
	memcpy(username, buf + 4, 16);
	return 1;
}