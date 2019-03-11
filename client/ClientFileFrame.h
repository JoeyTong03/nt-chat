#pragma once
#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>

const int OK = 1;
const int ERROR =0;
const int ACKNULL = 0x00;			//Ӧ����ϢΪ��
const int UserNameLength = 16;		//�û�����������
const int FileNameLength = 36;		//�ļ�����������
const int FrameFileLength =1000;	//�ļ������ļ���Ϣ����

/*׼���ϴ��ļ���Ӧ������*/
enum RplyAllowUpload
{
	UploadOk = 0x00,	//�����ϴ�
	UploadRefuse = 0x01	//�ܾ��ϴ�
};
/*׼������ļ���Ӧ������*/
enum RplyAllowAccept
{
	AcceptOk = 0x00,	//�������
	AcceptRefuse = 0x01	//�ܾ�����
};
/***************************************************************************
�ṹ�����ƣ�FrameHead
��      �ܣ�֡ͷ��ʽ
��Ա��������type:֡����,ack:Ӧ�����ͻ�0x00��������;,length:֡����
˵      ����
***************************************************************************/
typedef struct
{
	uint8_t type;
	uint8_t ack;
	uint16_t length;
}FrameHead;
/*****************�ļ�������S->C������֡����*******************/
/***************************************************************************
�ṹ�����ƣ�S2CFileHeadRply
��      �ܣ��ļ���Ϣ֡1Ӧ��֡
��Ա��������FH:֡ͷ
˵      ����C1��S�˷��͵��ı���Ϣ֡1��Ӧ��֡
C1��S��ʾ�ϴ��ļ�������ʱ��S�Ե�ǰ�Ƿ����ϴ��ļ�������Ӧ��
***************************************************************************/
typedef struct
{
	FrameHead FH;
}FrameS2CFileHeadRply;
/***************************************************************************
�ṹ�����ƣ�FrameS2CFileBodyRply
��      �ܣ��ļ���Ϣ֡2Ӧ��֡
��Ա��������FH:֡ͷ,FrameOrder:֡�����
˵      ����C1��S�˷��͵��ļ���Ϣ֡2��S��C1���͵�Ӧ��֡�Ա�ʾȷ���յ�
***************************************************************************/
typedef struct
{
	FrameHead FH;
	uint32_t FrameOrder;
}FrameS2CFileBodyRply;
/***************************************************************************
�ṹ�����ƣ�FrameS2CFileInform
��      �ܣ������ļ���Ϣ֡
��Ա��������FH:֡ͷ,SendUserName:�������û���,FileSize:�ļ���С,PackNum:�ļ���������,FileName:�ļ���
˵      ����C1��S�ļ��ϴ��ɹ���S��C2���͵�֡������֪ͨC2����˭���������ļ����ļ�����ʲô���ļ���С����
***************************************************************************/
typedef struct
{
	FrameHead FH;
	char SendUserName[UserNameLength];
	uint32_t FileSize;
	uint32_t PackNum;
	char FileName[FileNameLength];
}FrameS2CFileInform;
/***************************************************************************
�ṹ�����ƣ�FrameS2CFileHead
��      �ܣ��ļ���Ϣ֡1(�����ļ������ļ�ͷ��Ϣ)
��Ա��������FH:֡ͷ,SendUserName:�������û���,FileSize:�ļ���С,PackNum:�ļ���������,FileName:�ļ���
˵      ����C2��S��������ָ���S��C2���͵���Ϣͷ
***************************************************************************/
typedef struct
{
	FrameHead FH;
	char SendUserName[UserNameLength];
	uint32_t FileSize;
	uint32_t PackNum;
	char FileName[FileNameLength];
}FrameS2CFileHead;
/***************************************************************************
�ṹ�����ƣ�FrameS2CFileBody
��      �ܣ��ļ���Ϣ֡2(�����ļ�����)
��Ա��������FH:֡ͷ,SendUserName:�������û���,OrderNum:�ļ������к�,FileCont:�ļ�����
˵      �������ļ����γɶ���ļ����ݰ�
***************************************************************************/
typedef struct
{
	FrameHead FH;
	char SendUserName[UserNameLength];
	uint32_t OrderNum;
	char FileCont[FrameFileLength];
}FrameS2CFileBody;

/*****************�ļ�������C->S������֡����*******************/
/***************************************************************************
�ṹ�����ƣ�FrameC2SFileHead
��      �ܣ��ļ���Ϣ֡1(�����ļ������ļ�ͷ��Ϣ)
��Ա��������FH:֡ͷ,AcceptUserName:�������û���,FileSize:�ļ���С,PackNum:�ļ�������,FileName:�ļ���
˵      ����C1��S���͵��ļ���������
***************************************************************************/
typedef struct
{
	FrameHead FH;
	char AcceptUserName[UserNameLength];
	uint32_t FileSize;
	uint32_t PackNum;
	char FileName[FileNameLength];
}FrameC2SFileHead;

/***************************************************************************
�ṹ�����ƣ�FrameC2SFileBody
��      �ܣ��ļ���Ϣ֡2(�����ļ�����)
��Ա��������FH:֡ͷ,AcceptUserName:�������û���,OrderNum:�ļ������к�,FileCont:�ļ�����
˵      ����C1��S�����ļ�����
***************************************************************************/
typedef struct
{
	FrameHead FH;
	char AcceptUserName[UserNameLength];
	uint32_t OrderNum;
	char FileCont[FrameFileLength];
}FrameC2SFileBody;
/***************************************************************************
�ṹ�����ƣ�FrameC2SFileDownload
��      �ܣ�����֪ͨ֡(�ͻ�����������)
��Ա��������FH:֡ͷ,�������û������ļ���
˵      ����C2��S���͵������ļ�����֡
***************************************************************************/
typedef struct
{
	FrameHead FH;
	char SendUserName[UserNameLength];
	char FileName[FileNameLength];
}FrameC2SFileDownload;
/***************************************************************************
�ṹ�����ƣ�FrameC2SFileHeadReply
��      �ܣ��ļ���Ϣ֡1Ӧ��֡
��Ա��������FH:֡ͷ
˵      ����S��C2�����ļ�ͷ��Ϣ��C2���͵�Ӧ��֡
***************************************************************************/
typedef struct
{
	FrameHead FH;
}FrameC2SFileHeadReply;
/***************************************************************************
�ṹ�����ƣ�FrameC2SFileBodyReply
��      �ܣ��ļ���Ϣ֡2Ӧ��֡
��Ա��������FH:֡ͷ,FrameOrder:�ļ������
˵      ����C2�����ļ���ȱ���������Ϣͷ
***************************************************************************/
typedef struct
{
	FrameHead FH;
	uint32_t FrameOrder;
}FrameC2SFileBodyReply;