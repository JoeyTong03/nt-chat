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

#define BUFSIZE 1024
#define LEN32	32	//32字节的帧
#define LEN16	16	//16字节的帧

#include "ServerFrame.h"
#include "db.h"

//解析报道帧时的结果类型
enum IdentifyResultType
{
	Right=1,
	SecretErr,
	NameNoExist,
	NeedUpdateSecret
}

struct Msg
{
	long mtype;
	char mtext[BUFSIZE];
};

/* 等待子进程结束，避免出现僵尸进程 */
void wait4children(int sig);

/* 初始化创建socket描述符 */
int initSocketFd(int *socket_fd,int port);

/* 初始化连接socket描述符 */
int initConnection(int *connect_fd,socket_fd);

/* 登录时初始化，负责用户登录时的认证、改密 */
int initLogin(int *connect_fd,MYSQL *mysql,char username[],int client_num);

/* 检查报道帧，判断用户合法性；同时参数传递获得username */
int identify(MYSQL *mysql,char buf[],char username[],int client_num);

/* 等待接收改密帧，并在服务端完成数据更新 */
int changeSecret(int *connect_fd, MYSQL *mysql, char username[]);

/* 向其他用户发送上线帧后进入聊天状态 */
int interactBridge(int *connect_fd, MYSQL *mysql,char username[],int client_num);

/* 建立消息队列/获取client_num参数对应的子进程的消息队列句柄*/
int createMsgQue(int _client_num,int *msg_id0,int *msg_id1);

/* 子进程向父进程发送帧 */
int sub2main(char *sendBuf,int msg_id1);

/* 子进程从父进程获取帧 */
int subFromMain(char subRecvBuf[],int msg_id0);

/* 收发子进程过来的数据 */
int transferMsg(MYSQL *mysql,int client_num);

/* 将数据群发给所有用户 */
int toAllUser(Msg *msg,int client_num);

/* 将数据发送给目标用户targetUser */
int toSomeone(MYSQL *mysql,Msg *msg,char targetUser[]);