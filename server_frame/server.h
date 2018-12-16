#pragma once
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

#define MAXNUM_CLIENT 10 //最多允许同时上线的客户端数
#define BUFSIZE 1024
#include "UserNode.h"

//帧类型
enum FrameHeadType
{
	SfhRegister = 0,	//报到帧
	SfhText,			//文本信息帧
	SfhChangeSecret,	//改密帧
	SfhOffLine			//下线帧
};

/* 等待子进程结束，避免出现僵尸进程 */
void wait4children(int sig)
{
	int status;
	while (waitpid(-1, &status, WNOHANG) > 0);
}

/* 初始化创建socket描述符 */
int initSocketFd(int *socket_fd,int port)
{				
	struct sockaddr_in servaddr;		//serverTCP协议族
	if ((*socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printf("Fail to create socket:%s(errno:%d)\n", strerror(errno), errno);
		return -2;
	}

	//置端口复用
	int opt = 1;
	if (setsockopt(*socket_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0) {
		printf("Fail to reuse port：errno=%d(%s)\n", errno, strerror(errno));
		close(*socket_fd);
		return -3;
	}

	//初始化
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//IP地址设置成INADDR_ANY
	servaddr.sin_port = htons(port);//设置的端口
		
	//设置非阻塞
	int flags;
	if((flags = fcntl(*socket_fd,F_GETFL,0)) == -1)
	{
		printf("%s(errno:%d)\n",strerror(errno),errno);
		return -4;
	}
	if(fcntl(*socket_fd,F_SETFL,flags|O_NONBLOCK) == -1)
	{
		printf("%s(errno:%d)\n",strerror(errno),errno);
		return -5;
	}
	
	//绑定socket_fd
	if (bind(*socket_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) 
	{
		printf("Fail to bind socket:%s(errno:%d)\n", strerror(errno), errno);
		return -6;
	}

	//开始监听是否有客户端连接
	if (listen(*socket_fd, MAXNUM_CLIENT) == -1) 
	{
		printf("Fail to listen socket:%s(errno:%d)\n", strerror(errno), errno);
		return -7;
	}
	printf("Wait for clients......\n");
	return 0;
}

/* 初始化连接socket描述符 */
int initConnection(int *connect_fd,socket_fd)
{
	if ((*connect_fd = accept(socket_fd, (struct sockaddr*)NULL, NULL)) == -1) 
	{
		printf("Socket connect error:%s(errno:%d)\n", strerror(errno), errno);
		return -1;
	}
	//设置非阻塞模式
	if((flags = fcntl(*connect_fd,F_GETFL,0)) == -1){
		printf("%s(errno:%d)\n",strerror(errno),errno);
		return -2;
	}
	if(fcntl(*connect_fd,F_SETFL,flags|O_NONBLOCK) == -1){
		printf("%s(errno:%d)\n",strerror(errno),errno);
		return -3;
	}
	return 0;
}


/* 登录时初始化，负责用户登录时的认证、改密 */
int initLogin(int *connect_fd,char username[])
{
	char recvBuf[BUFSIZE],sendBuf[BUFSIZE]; 
	int isFirstLogin;//是否是首次登陆
	char newSecret[15];//新密码
	int replyType=Right;//应答类型缺省值Right
	
	fd_set rfd,wfd;
	FD_ZERO(&rfd);
	FD_ZERO(&wfd);
	int maxfd = *connect_fd;
	
	FD_SET(*connect_fd,&rfd);
	if(select(maxfd+1,&rfd,NULL,NULL,NULL)>0)//有可读的数据
	{
		/* 接收从client端发来的帧 */
		if(recv(*connect_fd,recvBuf,BUFSIZE,0)<0)
		{
			printf("Recv username error!\n");
			close(*connect_fd);
			return -2;
		}
	}
		
	/* 对首帧进行检验 */
	int identifyResult=identify(recvBuf,username);
	switch(identifyResult)
	{
		case -2://不是报道帧,直接关闭连接
			close(*connect_fd);
			break;
		case -3://密码错误
			replyType=SecretErr;
			break;
		case -4://用户不存在
			replyType=NameNoExist;
			break;
		case -5://需要改密
			replyType=NeedUpdateSecret;
			break;
			
	}
		
	/* 发送应答帧 */
	memset(sendBuf,0,BUFSIZE);
	initReplyFrame(replyType,sendBuf);
	FD_SET(*connect_fd,&wfd);
	if(select(maxfd+1,NULL,&wfd,NULL,NULL)>0)//有可写的数据
	{
		if(send(*connect_fd,sendBuf,BUFSIZE,0)<0)
		{
			printf("Send reply error!\n");
			close(*connect_fd);
			return -3;
		}
	}
		
			
	/* 如果需要修改密码，等待客户端发送SfhChangeSecret帧，并获取其中的密码，更新数据库 */
	if(replyType==NeedUpdateSecret)
		if(changeSecret(connect_fd,username)<0)
			return -4;
		
	return 0;
}	
	
int interactBridge(int *connect_fd,char username[])
{
	/* 生成一个上线帧 */
	char onlineFrame[32];
	initOnlineFrame(username,onlineFrame);	
	
	/* 传递给父进程 */
	
	
	/* 进入聊天状态，收发client端的帧，直接扔给父进程 */
	fd_set rfd,wfd;
	int maxfd;
	char recvBuf[BUFSIZE],sendBuf[BUFSIZE];
	int ret;
	while(1)
	{
		FD_ZERO(&rfd);
		FD_ZERO(&wfd);
		FD_SET(*connect_fd,&rfd);
		FD_SET(*connect_fd,&wfd);
		memset(recvBuf,0,BUFSIZE);
		memset(sendBuf,0,BUFSIZE);
		maxfd=*connect_fd;
		ret=select(maxfd+1,&rfd,&wfd,NULL);
		if(ret>0 && FD_ISSET(*connect_fd,&rfd))//有可读的数据
		{
			if(recv(*connect_fd,recvBuf,BUFSIZE,0)<0)
			{
				printf("Recv fail!\n");
				close(*connect_fd);
				return -2;
			}
				
			else
			{
				/* 传递给父进程 */
				
				
				/* 判断是否是下线帧 */
				if(getType(recvBuf)==SfhOffLine)//如果是下线帧
				{
					printf("User %s offline!\n",username);
					
					/* 删除数据库“在线用户表”中的该用户记录 */
					DelUser(username);
					
					close(*connect_fd);
					break;
				}
			}
			
		}
		if(ret>0 && FD_ISSET(*connect_fd,&wfd))//有可写的数据
		{
			/* 从父进程读取帧到sendBuf中 */
			

			/* 发送给该子进程关联的用户 */
			if(send(*connect_fd,sendBuf,BUFSIZE,0)<0)
			{
				printf("Send fail!\n");
				close(*connect_fd);
				return -3;
			}	
		}
	}
	return 0;
}	
		


/* 
	判断连接后收到的第一帧是否为报道帧；
	如果是则根据帧中携带的用户名与密码查询数据库进行认真 
	参数：喂buf,吐username
	*/
int identify(char buf[],char username[])
{
	char secret[15];
	int tmp;
	MYSQL mysql;
	
	/* 判断收到的是否为报道帧，如果不是直接断开连接*/
	if(getType(buf)!=SfhRegister)
	{
		printf("The first frame isn't SfhRegister!\n");
		return -2;
	}
	
	/* 解析报道帧，获取用户名及密码 */
	analysisSfhRegister(buf,username,secret);
	
	/* 检查用户名及密码是否正确 */
	tmp=JudgeUser(&mysql,username,secret);
	switch(tmp)
	{
		case Right:
			break;
		case SecretErr:
			printf("Secret is error!\n");
			return -3;
		case NameNoExist:
			printf("User is not exist!\n");
			return -4;
		default:
			printf("Need update secret!\n");
			return -5;
	}
}

/* 改密 */
int changeSecret(int *connect_fd,char username[])
{
	char buf[BUFSIZE];
	char newSecret[15];
	int maxfd;
	fd_set rfd;
	FD_ZERO(&rfd);
	FD_SET(*connect_fd,&rfd);
	maxfd=*connect_fd+1;
	MYSQL mysql;
	if(select(maxfd+1,&rfd,NULL,NULL,NULL)>0)//有可读的数据
	{
		/* 接收从client端发来的改密帧 */
		if(recv(*connect_fd,buf,BUFSIZE,0)<0)
		{
			printf("Recv SfhChangeSecret error!\n");
			close(*connect_fd);
			return -2;
		}
	}
	
	if(getType(buf)!=SfhChangeSecret)//如果不是改密帧，直接关闭连接
	{
		printf("Not SfhChangeSecret!\n");
		close(*connect_fd);
		return -3;
	}
	/* 解析改密帧，获取新密码 */
	analysisSfhChangeSecret(buf,newSecret);
		
	/* 更新数据库该用户的密码 */
	UpdateSecret(&mysql,username,newSecret);
	return 0;
}