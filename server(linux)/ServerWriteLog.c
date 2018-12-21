#include "ServerWriteLog.h"
/***************************************************************************
  函数名称：WriteFile
  功    能：写入文件
  输入参数：文件名，缓存区，长度
  返 回 值：
  说    明：
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
  函数名称：StrTime
  功    能：将当前时间转化为字符数组
  输入参数：
  返 回 值：当前时间字符串指针
  说    明：
***************************************************************************/
int StrTime(char *TimeStr)
{
    struct timespec time;
    clock_gettime(CLOCK_REALTIME, &time); //获取相对于1970到现在的秒数
    struct tm nowTime;
    localtime_r(&time.tv_sec, &nowTime);
    sprintf(TimeStr, "[%04d/%02d/%02d %02d:%02d:%02d] ", nowTime.tm_year + 1900, nowTime.tm_mon + 1, nowTime.tm_mday, nowTime.tm_hour, nowTime.tm_min, nowTime.tm_sec);
    return 1;
}
/***************************************************************************
  函数名称：WriteReglog
  功    能：将登陆信息写入日志
  输入参数：用户名，登陆成功或失败原因
  返 回 值：
  说    明：日志文件位于log文件夹下，每个日志文件的文件名与用户名同名
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
  函数名称：WriteOfflinelog
  功    能：将下线信息写入日志
  输入参数：用户名
  返 回 值：
  说    明：日志文件位于log文件夹下，每个日志文件的文件名与用户名同名
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
  函数名称：WriteChgPswdlog
  功    能：将下线信息写入日志
  输入参数：用户名
  返 回 值：
  说    明：日志文件位于log文件夹下，每个日志文件的文件名与用户名同名
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
  函数名称：WriteSendText
  功    能：将发送信息事件写入日志
  输入参数：发送者用户名，接受者用户名
  返 回 值：
  说    明：若成功发送，将事件写入发送者和接受者日志；若失败，将事件及原因写入发送者日志
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
  函数名称：WriteAllLog
  功    能：向全体成员发送数据的日志
  输入参数：用户名
  返 回 值：
  说    明：
***************************************************************************/
int WriteAllLog(char*name)
{
    char time[30];
    char event[40];
    char message[70];
    StrTime(time);
    sprintf(event, "%s has sent a message to all users.\n", name);
    sprintf(message,"%s%s",time, event);
    WriteFile("all", message, strlen(message));
    WriteFile(name, message, strlen(message));
    return 1;
}