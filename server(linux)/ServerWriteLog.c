#include "ServerWriteLog.h"
/***************************************************************************
  �������ƣ�WriteFile
  ��    �ܣ�д���ļ�
  ����������ļ�����������������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int WriteFile(char *FileName, char *buf, int buf_len)
{
    mkdir("log", 0777);
    char loc[10];
    sprintf(loc, "log/%s", FileName);
    int fd = open(loc, O_RDWR | O_CREAT | O_APPEND);
    write(fd, buf, buf_len);
    close(fd);
    return fd;
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
int WriteReglog(char *name,int RightorErrorType)
{
    char time[30];
    char event[50];
    char message[100];
    StrTime(time);
    if (RightorErrorType == Right||RightorErrorType==NeedUpdateSecret)
    {
        sprintf(event, "%s has logged in successfully.\n", name);
        sprintf(message,"%s%s",time, event);
        WriteFile(name, message, strlen(message));
    }
    else
    {
        sprintf(event, "%s failed to send message.\n", name);
        sprintf(message,"%s%s",time, event);
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
    sprintf(message,"%s%s",time, event);
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
    sprintf(message,"%s%s",time, event);
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
        sprintf(message,"%s%s",time, event);
        WriteFile(SendName, message, strlen(message));
        sprintf(event, "%s has accepted a message from %s successfully.\n", RecvName, SendName);
        sprintf(message,"%s%s",time, event);
        WriteFile(RecvName, message, strlen(message));
    }
    else
    {
        sprintf(event, "%s failed to send message.\n", SendName);
        sprintf(message,"%s%s",time, event);
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
int WriteAllLog(char*name)
{
    char time[30];
    char event[40];
    char message[70];
    char allstr[]="all";
    StrTime(time);
    sprintf(event, "%s has sent a message to all users.\n", name);
    sprintf(message,"%s%s",time, event);
    WriteFile(allstr, message, strlen(message));
    WriteFile(name, message, strlen(message));
    return 1;
}