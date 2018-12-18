#include"ServerFrame.h"
/***************************************************************************
�������ƣ�getType
��    �ܣ�����֡ͷ������֡�Ĺ��ܶ�Ӧö��ֵ
���������֡
�� �� ֵ��֡ͷö��ֵ
˵    ����
***************************************************************************/
int getType(char buf[])
{
	return (int)buf[0];
}
/***************************************************************************
�������ƣ�analysisSfhRegister
��    �ܣ���������֡
�������������֡��client��Ӧ�û���,����ָ��
�� �� ֵ��
˵    ����ʹ�ú���ʱ��֡���ַ�����ʽ���룬����������������ָ����ʽ���루�ں�������ռ䣩
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
�������ƣ�analysisSfhChangeSecret
��    �ܣ���������֡
�������������֡��������ָ��
�� �� ֵ��
˵    ����
***************************************************************************/
int analysisSfhChangeSecret(char buf[], char newSecret[])
{
	//newSecret = (char*)malloc(12 * sizeof(char));
	memset(newSecret, 0, 12);
	strcpy(newSecret, buf + 4);
	return 1;
}
/***************************************************************************
�������ƣ�analysisSfhText
��    �ܣ������ı���Ϣ֡
���������֡����Դclient�û������ı����ݶ���ָ��
�� �� ֵ��
˵    �����ں����⽫�����ı���Ϣ��ָ�룬����һ������ָ��ָ���ָ�룬��Ϊ����Ĳ���
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

/********************S->C��֡���ɲ���*******************/
/***************************************************************************
�������ƣ�initReplyFrame
��    �ܣ�����ReplyType����Ӧ��֡replyFrame
���������Ӧ�����ͣ�Ӧ��֡
�� �� ֵ��
˵    ����
***************************************************************************/
int initReplyFrame(int ReplyType, char**ReplyFrame)
{
	*ReplyFrame = (char*)malloc(4 * sizeof(char));
	memset(*ReplyFrame, 0, 4);
	*ReplyFrame[0] = (char)0x71;
	*ReplyFrame[1] = (char)ReplyType;
	uint16_t length = 4;
	memcpy(*ReplyFrame + 2, &length, 2);
	return 1;
}
/***************************************************************************
�������ƣ�CrtTextReplyFrame
��    �ܣ������ı�Ӧ��֡�ı���Ӧ�����ͣ�
���������Ӧ�����ͣ�Ӧ��֡
�� �� ֵ��
˵    ����
***************************************************************************/
int CrtTextReplyFrame(int ReplyType, char**TextReplyFrame)
{
	*TextReplyFrame = (char*)malloc(4 * sizeof(char));
	memset(*TextReplyFrame, 0, 4);
	*TextReplyFrame[0] = (char)0x72;
	*TextReplyFrame[1] = (char)ReplyType;
	uint16_t length = 4;
	memcpy(*TextReplyFrame + 2, &length, 2);
	return 1;
}
/***************************************************************************
�������ƣ�CrtTextFrame
��    �ܣ������ı���Ϣ֡��ת�����û������ı���Ϣ��
����������ı���ϢԴ���û������ı���Ϣ���ı���Ϣ֡
�� �� ֵ��
˵    ����
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
	return 1;
}
/***************************************************************************
�������ƣ�CrtOffLineFrame
��    �ܣ�����������λ֡
���������֡
�� �� ֵ��
˵    ����
***************************************************************************/
int CrtOffLineFrame(char**OffLineFrame)
{
	*OffLineFrame = (char*)malloc(4 * sizeof(char));
	memset(*OffLineFrame, 0, 4);
	*OffLineFrame[0] = (char)0x73;
	uint16_t length = 4;
	memcpy((*OffLineFrame) + 2, &length, 2);
	return 1;
}
/***************************************************************************
�������ƣ�CrtOnOffFrame
��    �ܣ�������/����֡(���߻������û���������/����״̬)
������������߻����ߵ��û���������/���߱�ǣ�֡
�� �� ֵ��
˵    ����
***************************************************************************/
int CrtOnOffFrame(char*name, int OnorOff, char**OnOffFrame)
{
	uint16_t length = 32;
	*OnOffFrame = (char*)malloc(length * sizeof(char));
	memset(*OnOffFrame, 0, 32);
	(*OnOffFrame)[0] = (char)0x75;
	memcpy((*OnOffFrame) + 2, &length, 2);
	memcpy((*OnOffFrame) + 4, name, strlen(name));
	return 1;
}
/***************************************************************************
�������ƣ�CrtFriInit
��    �ܣ����ɺ��ѳ�ʼ��֡
����������û����ı���֡
�� �� ֵ��
˵    ����
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
	return 1;
}
/***************************************************************************
�������ƣ�CrtOnLineFrame
��    �ܣ���������֡�����û���������ȥ
����������û���,֡
�� �� ֵ��
˵    ����
***************************************************************************/
int CrtOnLineFrame(char username[], char**frOnline)
{
	uint16_t length = 32;
	*frOnline = (char*)malloc(length * sizeof(char));
	memset(*frOnline, 0, length);
	(*frOnline)[0] = (char)0x77;
	memcpy((*frOnline) + 2, &length, 2);
	memcpy((*frOnline) + 4, username, 16);
	return 1;
}
/***************************************************************************
�������ƣ�analysisSfhOnLine
��    �ܣ���������֡,������ߵ��û���
���������֡���û���
�� �� ֵ��
˵    ����
***************************************************************************/
int analysisSfhOnOffLine(char buf[], char username[])
{
	memset(username, 0, 16);
	memcpy(username, buf + 4, 16);
	return 1;
}