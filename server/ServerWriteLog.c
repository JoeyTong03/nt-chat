
#include "ServerWriteLog.h"
#include <sys/file.h> //flock,lockf
#include <sys/prctl.h>
#include <fstream>
#include<iostream>

using namespace std;

/**********************
* �������ƣ�lock_set
* ��    �ܣ�Ϊ�ļ�������fd��Ӧ���ļ�����
* ��    ����fd--�ļ���������type--��������
* ��    �أ�
* ˵    ������������
	(1)F_RDLCK:����
	(2)F_WRLCK:д��
	(3)F_UNLCK:����
***********************/
int lock_set(int fd, int type)
{
    struct flock old_lock, lock;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_type = type;
    lock.l_pid = -1;

    // �ж��ļ��Ƿ��������
    fcntl(fd, F_GETLK, &lock);

    if (lock.l_type != F_UNLCK)
    {
        //�ж��ļ�����������ԭ��
        if (lock.l_type == F_RDLCK)
            printf("Read lock already set by %d\n", lock.l_pid);
        else if (lock.l_type == F_WRLCK)
            printf("Write lock already set by %d\n", lock.l_pid);
    }

    lock.l_type = type;
    printf("The type of lock is %d.\n", type);
    //���ݲ�ͬ��typeֵ��������ʽ���������
    if ((fcntl(fd, F_SETLKW, &lock)) < 0)
    {
        printf("Lock failed:type = %d\n", lock.l_type);
        return -1;
    }

    switch (lock.l_type)
    {
    case F_RDLCK:
        printf("Read lock set by %d\n", getpid());
        break;

    case F_WRLCK:
        printf("Write lock set by %d\n", getpid());
        break;

    case F_UNLCK:
        printf("Release lock by %d\n", getpid());
        break;
    default:
        break;
    }
    return 1;
}

/***************************************************************************
  �������ƣ�WriteFile
  ��    �ܣ�д���ļ�
  ����������ļ�����������������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int WriteFile(char *FileName, char *buf, int buf_len)
{
    // mkdir("log", 0777);

    // string path = ".//log//";
    // path += string(FileName);

    // ofstream outfile;
    // outfile.open(path.c_str(), std::fstream::out | std::fstream::app);

    // if (!outfile.is_open()) //����ļ��Ƿ�������//�������ڼ���ļ��Ƿ����
    // {
    //     printf("log error");
    //     outfile.close();
    // }
    // else
    // {
    //     outfile << buf << endl;
    //     outfile.close();
    // }











    // char loc[10];
    // sprintf(loc, "log/%s", FileName);
    // int fd = open(loc, O_WRONLY | O_CREAT | O_APPEND);

    // lock_set(fd, F_WRLCK);

    // write(fd, buf, buf_len);

    // lock_set(fd, F_UNLCK);

    // close(fd);
    return 1;
}

/***************************************************************************
  �������ƣ�StrTime
  ��    �ܣ�����ǰʱ��ת��Ϊ�ַ�����
  ���������
  �� �� ֵ����ǰʱ���ַ���ָ��
  ˵    ����
***************************************************************************/
int StrTime(char *TimeStr)
{
    struct timespec time;
    clock_gettime(CLOCK_REALTIME, &time); //��ȡ�����1970�����ڵ�����
    struct tm nowTime;
    localtime_r(&time.tv_sec, &nowTime);
    sprintf(TimeStr, "[%04d/%02d/%02d %02d:%02d:%02d] ", nowTime.tm_year + 1900, nowTime.tm_mon + 1, nowTime.tm_mday, nowTime.tm_hour, nowTime.tm_min, nowTime.tm_sec);
    return 1;
}

/***************************************************************************
  �������ƣ�WriteReglog
  ��    �ܣ�����½��Ϣд����־
  ����������û�������½�ɹ���ʧ��ԭ��
  �� �� ֵ��
  ˵    ������־�ļ�λ��log�ļ����£�ÿ����־�ļ����ļ������û���ͬ��
***************************************************************************/
int WriteReglog(char *name, int RightorErrorType)
{
    char time[30];
    char event[50];
    char message[100];
    StrTime(time);
    if (RightorErrorType == Right || RightorErrorType == NeedUpdateSecret)
    {
        sprintf(event, "%s has logged in successfully.\n", name);
        sprintf(message, "%s%s", time, event);
        WriteFile(name, message, strlen(message));
    }
    else
    {
        sprintf(event, "%s failed to send message.\n", name);
        sprintf(message, "%s%s", time, event);
        WriteFile(name, message, strlen(message));
        if (RightorErrorType == NameNoExist)
        {
            char ErrorType[] = "[Error Reason] User name doesn\'t exist.\n";
            WriteFile(name, ErrorType, strlen(ErrorType));
        }
        else
        {
            char ErrorType[] = "[Error Reason] Password Error.\n";
            WriteFile(name, ErrorType, strlen(ErrorType));
        }
    }
    return 1;
}
/***************************************************************************
  �������ƣ�WriteOfflinelog
  ��    �ܣ���������Ϣд����־
  ����������û���
  �� �� ֵ��
  ˵    ������־�ļ�λ��log�ļ����£�ÿ����־�ļ����ļ������û���ͬ��
***************************************************************************/
int WriteOfflinelog(char *name)
{
    char time[30];
    char event[20];
    char message[50];
    StrTime(time);
    sprintf(event, "%s is off line.\n", name);
    sprintf(message, "%s%s", time, event);
    WriteFile(name, message, strlen(message));
    // printf("%s",message);
    return 1;
}
/***************************************************************************
  �������ƣ�WriteChgPswdlog
  ��    �ܣ���������Ϣд����־
  ����������û���
  �� �� ֵ��
  ˵    ������־�ļ�λ��log�ļ����£�ÿ����־�ļ����ļ������û���ͬ��
***************************************************************************/
int WriteChgPswdlog(char *name)
{
    char time[30];
    char event[40];
    char message[70];
    StrTime(time);
    sprintf(event, "%s has changed password.\n", name);
    sprintf(message, "%s%s", time, event);
    WriteFile(name, message, strlen(message));
    return 1;
}
/***************************************************************************
  �������ƣ�WriteSendText
  ��    �ܣ���������Ϣ�¼�д����־
  ����������������û������������û���
  �� �� ֵ��
  ˵    �������ɹ����ͣ����¼�д�뷢���ߺͽ�������־����ʧ�ܣ����¼���ԭ��д�뷢������־
***************************************************************************/
int WriteSendText(char *SendName, char *RecvName, int RightorErrorType)
{
    char time[30];
    char event[50];
    char message[100];
    StrTime(time);
    if (RightorErrorType == SendSucc)
    {
        sprintf(event, "%s has sent a message to %s successfully.\n", SendName, RecvName);
        sprintf(message, "%s%s", time, event);
        WriteFile(SendName, message, strlen(message));
        sprintf(event, "%s has accepted a message from %s successfully.\n", RecvName, SendName);
        sprintf(message, "%s%s", time, event);
        WriteFile(RecvName, message, strlen(message));
    }
    else
    {
        sprintf(event, "%s failed to send message.\n", SendName);
        sprintf(message, "%s%s", time, event);
        WriteFile(SendName, message, strlen(message));
        if (RightorErrorType == GramWrong)
        {
            char ErrorType[] = "[Error Reason] Grammar mistakes\n";
            WriteFile(SendName, ErrorType, strlen(ErrorType));
        }
        else if (RightorErrorType == FriNotExist)
        {
            char ErrorType[] = "[Error Reason] User name doesn\'t exist\n";
            WriteFile(SendName, ErrorType, strlen(ErrorType));
        }
        else
        {
            char ErrorType[] = "[Error Reason] User is offline\n";
            WriteFile(SendName, ErrorType, strlen(ErrorType));
        }
    }
    return 1;
}
/***************************************************************************
  �������ƣ�WriteAllLog
  ��    �ܣ���ȫ���Ա�������ݵ���־
  ����������û���
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int WriteAllLog(char *name)
{
    char time[30];
    char event[40];
    char message[70];
    char allstr[] = "all";
    StrTime(time);
    sprintf(event, "%s has sent a message to all users.\n", name);
    sprintf(message, "%s%s", time, event);
    WriteFile(allstr, message, strlen(message));
    WriteFile(name, message, strlen(message));
    return 1;
}