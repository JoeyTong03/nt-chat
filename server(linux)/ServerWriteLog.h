#pragma once
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "ServerFrame.h"
// //�ı�Ӧ��֡��Ӧ������
// enum IdentifyTextType
// {
// 	SendSucc = 0,		//�ɹ�ת��
// 	GramWrong = 0xFF,	//�﷨����
// 	FriNotExist = 0xFE,	//�������Ʋ�����
// 	FriOffLine = 0xFD	//����������
// };
// //����Ӧ��֡��Ӧ������
// enum IdentifyResultType
// {
// 	Right = 0,			//��ȷ
// 	SecretErr,			//�������
// 	NameNoExist,		//�û�������
// 	NeedUpdateSecret	//��Ҫ����
// };
//����½��Ϣд����־
int WriteReglog(char *name,int RightorErrorType);
//��������Ϣд����־
int WriteOfflinelog(char *name);
//��������Ϣд����־
int WriteChgPswdlog(char *name);
//��������Ϣ�¼�д����־
int WriteSendText(char *SendName, char *RecvName, int RightorErrorType);
//������Ϣ��־
int WriteAllLog(char*name);