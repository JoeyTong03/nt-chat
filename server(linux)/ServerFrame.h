#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
//����Ӧ��֡��Ӧ������
enum IdentifyResultType
{
	Right = 0,			//��ȷ
	SecretErr,			//�������
	NameNoExist,		//�û�������
	NeedUpdateSecret	//�Ƿ���Ҫ����
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
	SfhRegister = 0x11,		//����֡
	SfhText = 0x12,			//�ı���Ϣ֡
	SfhChangeSecret = 0x15,	//����֡
	SfhOffLine = 0x16		//����֡
};
//S->C֡����
enum ReceiveFrameHeadType
{
	RfhRegister = 0x71,		//����֡
	RfhTextReply = 0x72,	//�ı�Ӧ��֡
	RfhText = 0x77,			//�ı���Ϣ֡
	RfhBeKicked = 0x73,		//������λ֡
	RfhOnOffLine = 0x75,	//��/����֡
	RfhInit = 0x76,			//���ѳ�ʼ��֡
	OnOffLine = 0x77		//����֡����Ҳ��֪����������Բ��ԣ�
};
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
