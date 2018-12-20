
#define SERVER
#ifdef SERVER

#include <time.h>
#include <fcntl.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/file.h>//flock,lockf
#include <errno.h>
#include <signal.h>
#include <sys/prctl.h>//修改进程名
#include <sys/msg.h> //消息队列


#define BUFSIZE 1024
#define LEN32	32	//32字节的帧
#define LEN16	16	//16字节的帧

#define MAXNUM_CLIENT 10 //最多的客户端数量

#include "ServerWriteLog.h"
#include "db.h"

#define FLAG_ALL 1
#define FLAG_SOMEONE 0

//解析报道帧时的结果类型
enum IdentifyResultType
{
	Right=1,
	SecretErr,
	NameNoExist,
	NeedUpdateSecret
};

typedef struct Msg
{
	long mtype;
	char mtext[BUFSIZE];
}Msg;


/* 等待子进程结束，避免出现僵尸进程 */
void wait4children(int sig);

/* 初始化创建socket描述符 */
int initSocketFd(int *socket_fd,int port);

/* 初始化连接socket描述符 */
int initConnection(int* connect_fd,int socket_fd );

/* 登录时初始化，负责用户登录时的认证、改密 */
int initLogin(int *connect_fd,MYSQL *mysql,char username[],int client_num);

/* 检查报道帧，判断用户合法性；同时参数传递获得username */
int identify(MYSQL *mysql,char buf[],char username[],int client_num);

/* 等待接收改密帧，并在服务端完成数据更新 */
int changeSecret(int *connect_fd, MYSQL *mysql, char username[]);

/* 向其他用户发送上线帧 */
int sendOnlineFrame(MYSQL* mysql,char username[]);


/* 向其他用户发送上线帧后进入聊天状态 */
int interactBridge(int *connect_fd, MYSQL *mysql,char username[],int client_num);



/* 解析sendBuf获得目标用户名targetUserName */
int getTargetUsername(char buf[],char targetUsername[],int* isToALL);

/* username将数据帧发给所有用户 */
int toAllUsers(MYSQL* mysql,char username[],char *msg);


//得到每一条服务端子进程向客户端所发的消息
int GetSendMessage(MYSQL* _mysql,char* username,char*** buf);

//服务端子进程向其他子进程发送包装好的帧
void SetMessageToDB(MYSQL* _mysql,char* fromuser,char* touser,char* msg);

#endif