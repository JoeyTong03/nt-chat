#pragma once
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "ServerFrame.h"
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