#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
//S->C2���ı���Ϣ֡���ж�Ⱥ��������˷��ͱ��
//C�յ�����S���ı���Ϣ֡���۲��־λ[8-15]
enum SglOrAll
{
	Single = 0x00,
	All = 0x01
};
//��������֡ʱ�Ľ������
enum IdentifyResultType
{
	Right=1,
	SecretErr,
	NameNoExist,
	NeedUpdateSecret,
	TextFrameRecvFail
};
//�ı�Ӧ��֡��Ӧ������
enum IdentifyTextType
{
	SendSucc = 0,		//�ɹ�ת��
	GramWrong = 0xFF,	//�﷨����
	FriNotExist = 0xFE,	//�������Ʋ�����
	FriOffLine = 0xFD	//����������
};
//C->S֡����
enum SendFrameHeadType
{
	SfhRegister = 0x11,			//����֡
	SfhText = 0x12,				//�ı���Ϣ֡
	SfhChangeSecret = 0x15,		//����֡
	SfhOffLine = 0x16,			//����֡
	SfhOnLine = 0x18,			//����֡
	SfhFileHead = 0x20,			//�ļ���Ϣ֡1(�����ļ������ļ�ͷ��Ϣ)
	SfhFileBody = 0x21,			//�ļ���Ϣ֡2(�����ļ�����)
	SfhFileDownload = 0x22,		//����֪ͨ֡(�ͻ�����������)
	SfhFileHeadReply = 0x23,	//�ļ���Ϣ֡1Ӧ��֡
	SfhFileBodyReply = 0x24		//�ı���Ϣ֡2Ӧ��֡
};
//S->C֡����
enum ReceiveFrameHeadType
{
	RfhRegister = 0x71,			//����Ӧ��֡
	RfhTextReply = 0x72,		//�ı�Ӧ��֡
	RfhText = 0x77,				//�ı���Ϣ֡
	RfhBeKicked = 0x73,			//������λ֡
	RfhOnOffLine = 0x75,		//��/����֡
	RfhInit = 0x76,				//���ѳ�ʼ��֡
	RfhFileHeadReply = 0x80,	//�ļ���Ϣ֡1Ӧ��֡
	RfhFileBodyReply = 0x81,	//�ı���Ϣ֡2Ӧ��֡
	RfhFileInform = 0x82,		//�����ļ���Ϣ֡
	RfhFileHead = 0x83,			//�ļ���Ϣ֡1(�����ļ������ļ�ͷ��Ϣ)
	RfhFileBody = 0x84			//�ļ���Ϣ֡2(�����ļ�����)
};
void Str2int2(char *buf, int length);


/********************C->S��֡��������*******************/
//�����յ���֡�����������ĸ�֡���ͣ�
//����ֵΪͳһ��ö��ֵ
int getType(char buf[]);

//��������֡
int analysisSfhRegister(char buf[], char username[], char secret[]);

//��������֡
int analysisSfhChangeSecret(char buf[], char newSecret[]);

//�����ı���Ϣ֡
int analysisSfhText(char buf[], char name[], char**Text);

//��������֡
/********************S->C��֡���ɲ���*******************/
//����replyType����Ӧ��֡replyFrame
int initReplyFrame(int replyType, char**replyFrame);

//�����ı�Ӧ��֡�ı���Ӧ�����ͣ�
int CrtTextReplyFrame(int replyType, char**TextReplyFrame);

//�����ı���Ϣ֡��ת�����û������ı���Ϣ��
int CrtTextFrame(char*name, char*text, char**TextFrame);

//����������λ֡
int CrtOffLineFrame(char**OffLineFrame);

//������/����֡(���߻������û���������/����״̬)
int CrtOnOffFrame(char*name,int OnorOff,char**OnOffFrame);

//���ɺ��ѳ�ʼ��֡
int CrtFriInit(char* NameList, char**FriInitFrame);

/********************���ӽ��̴�֡*******************/

//��������֡�����û���������ȥ
int CrtOnLineFrame(char username[], char**frOnline);

//��������֡,������ߵ��û���
int analysisSfhOnOffLine(char buf[], char username[]);
