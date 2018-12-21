
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
#include <sys/prctl.h>//�޸Ľ�����
#include <sys/msg.h> //��Ϣ����


#define BUFSIZE 1024
#define LEN32	32	//32�ֽڵ�֡
#define LEN16	16	//16�ֽڵ�֡

#define MAXNUM_CLIENT 10 //���Ŀͻ�������

#include "ServerWriteLog.h"
#include "db.h"

#define FLAG_ALL 1
#define FLAG_SOMEONE 0



typedef struct Msg
{
	long mtype;
	char mtext[BUFSIZE];
}Msg;


/* �ȴ��ӽ��̽�����������ֽ�ʬ���� */
void wait4children(int sig);

/* ��ʼ������socket������ */
int initSocketFd(int *socket_fd,int port);

/* ��ʼ������socket������ */
int initConnection(int* connect_fd,int socket_fd );

/* ��¼ʱ��ʼ���������û���¼ʱ����֤������ */
int initLogin(int *connect_fd,MYSQL *mysql,char username[],int client_num);

/* ��鱨��֡���ж��û��Ϸ��ԣ�ͬʱ�������ݻ��username */
int identify(MYSQL *mysql,char buf[],char username[],int client_num);

/* �ȴ����ո���֡�����ڷ����������ݸ��� */
int changeSecret(int *connect_fd, MYSQL *mysql, char username[]);

/* �������û���������֡ */
int sendOnlineFrame(MYSQL* mysql,char username[]);


/* �������û���������֡���������״̬ */
int interactBridge(int *connect_fd, MYSQL *mysql,char username[],int client_num);



/* ����sendBuf���Ŀ���û���targetUserName */
int getTargetUsername(char buf[],char targetUsername[],int* isToALL);

/* username������֡���������û� */
int toAllUsers(MYSQL* mysql,char username[],char *msg);

/* ����buf�е�һ��e���±꣬isEnd�����Ƿ��Ѿ�����β */
int myfind(char buf[],char e,int *isEnd);

uint16_t getMsgLen(char *msg);
#endif