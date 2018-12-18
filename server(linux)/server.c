#include "server.h"

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
	fd_set rfd;
	FD_ZERO(&rfd);
	int maxfd = *connect_fd;
	FD_SET(*connect_fd,&rfd);
	if(select(maxfd+1,&rfd,NULL,NULL,NULL)>0)//有可读的数据
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
	}
	
	return 0;
}


/* 登录时初始化，负责用户登录时的认证、改密 */
int initLogin(int *connect_fd,MYSQL *mysql,char username[],int client_num)
{
	char recvBuf[LEN32];
	char *sendBuf; 
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
		if(recv(*connect_fd,recvBuf,LEN32,0)<0)
		{
			printf("Recv username error!\n");
			close(*connect_fd);
			return -2;
		}
	}
		
	/* 对首帧进行检验 */
	int identifyResult=identify(mysql,recvBuf,username,client_num);
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
		default://正确
			replyType=Right;
			break;
			
	}
		
	/* 发送应答帧 */
	initReplyFrame(replyType,&sendBuf);
	FD_SET(*connect_fd,&wfd);
	if(select(maxfd+1,NULL,&wfd,NULL,NULL)>0)//有可写的数据
	{
		if(send(*connect_fd,sendBuf,LEN32,0)<0)
		{
			printf("Send reply error!\n");
			close(*connect_fd);
			return -3;
		}
	}
		
			
	/* 如果需要修改密码，等待客户端发送SfhChangeSecret帧，并获取其中的密码，更新数据库 */
	if(replyType==NeedUpdateSecret)
		if(changeSecret(*connect_fd,mysql,username)<0)
			return -4;
	
	
	/* 发送好友初始化帧 */
	char *nameList=GetAllUsers(mysql);
	char *friInitFrame;
	CrtFriInit(nameList,&friInitFrame);
	FD_ZERO(&wfd);
	FD_SET(*connect_fd,&wfd);
	
	if(select(maxfd+1,NULL,&wfd,NULL,NULL)>0)//有可写的数据
	{
		if(send(*connect_fd,friInitFrame,strlen(friInitFrame)+1,0)<0)
		{
			printf("Send friInitFrame error!\n");
			close(*connect_fd);
			return -5;
		}
	}
	return 0;
}	

/* 向其他用户发送上线帧后进入聊天状态 */
int interactBridge(int *connect_fd, MYSQL *mysql,char username[],int client_num)
{
	/* 生成一个上线帧 */
	char *onlineFrame;
	CrtOnLineFrame(username,&onlineFrame);	//函数内部申请空间
	
	/* 创建消息队列 */
	int msg_id0,msg_id1;
	createMsgQue(client_num,&msg_id0,&msg_id1);
	
	/* 传递给父进程 */
	sub2main(onlineFrame,msg_id1);
	
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
				sub2main(recvBuf,msg_id1);
				
				/* 判断是否是下线帧 */
				if(getType(recvBuf)==SfhOffLine)//如果是下线帧
				{
					printf("User %s offline!\n",username);
					
					/* 删除数据库“在线用户表”中的该用户记录 */
					DelOnlineUser(mysql,username);
					
					close(*connect_fd);
					break;
				}
			}
			
		}
		if(ret>0 && FD_ISSET(*connect_fd,&wfd))//有可写的数据
		{
			/* 从父进程读取帧到sendBuf中 */
			if(subFromMain(sendBuf,msg_id0)==1)
			{
				/* 发送给该子进程关联的用户 */
				if(send(*connect_fd,sendBuf,BUFSIZE,0)<0)
				{
					printf("Send fail!\n");
					close(*connect_fd);
					return -3;
				}	
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
int identify(MYSQL *mysql,char buf[],char username[],int client_num)
{
	char secret[15];
	int tmp;
	
	/* 判断收到的是否为报道帧，如果不是直接断开连接*/
	if(getType(buf)!=SfhRegister)
	{
		printf("The first frame isn't SfhRegister!\n");
		return -2;
	}
	
	/* 解析报道帧，获取用户名及密码 */
	analysisSfhRegister(buf,username,secret);
	
	/* 检查用户名及密码是否正确 */
	tmp=JudgeUser(mysql,username,secret);
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
	AddOnlineUser(mysql,username,client_num);
	return 0;
}

/* 改密 */
int changeSecret(int *connect_fd, MYSQL *mysql, char username[])
{
	char buf[LEN16];
	char newSecret[16];
	int maxfd;
	fd_set rfd;
	FD_ZERO(&rfd);
	FD_SET(*connect_fd,&rfd);
	maxfd=*connect_fd+1;
	if(select(maxfd+1,&rfd,NULL,NULL,NULL)>0)//有可读的数据
	{
		/* 接收从client端发来的改密帧 */
		if(recv(*connect_fd,buf,LEN16,0)<0)
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
	UpdateSecret(mysql,username,newSecret);
	return 0;
}


/* 创建消息队列,key=client_num*100---main->sub
	key=client_num*100+1---sub->main
	返回消息队列标识符
*/
int createMsgQue(int _client_num,int *msg_id0,int *msg_id1)
{
	*msg_id0=msgget(_client_num*100,IPC_CREAT | 0666);
	if(*msg_id0==-1)
	{
		printf("client %d create MQ0 fail!",_client_num);
		return -2;
	}

	*msg_id1=msgget(_client_num*100+1,IPC_CREAT | 0666);
	if(*msg_id1==-1)
	{
		printf("client %d create MQ1 fail!",_client_num);
		return -3;
	}
	return 0;
}

                                                                                                                                                
/* 
功能：子进程向父进程发送数据 
参数：sendBuf--要发送的数据  index--是第几个子进程
*/
int sub2main(char *sendBuf,int msg_id1)
{
	struct Msg msg;
	msg.mtype=1;
	memcpy(msg.mtext,sendBuf,strlen(sendBuf)+1);
	//msg.mtext=sendBuf;
	
	if(msgsnd(msg_id1,&msg,strlen(sendBuf)+1,0)==-1)
	{
		printf("Client send to %d fail!\n",msg_id1);
		return -2;
	}
	return 0;
}

/* 
功能：子进程从父进程读数据
参数：subRecvBuf--接收的数据  index--第几个子进程
 */
int subFromMain(char subRecvBuf[],int msg_id0)
{
	struct Msg msg;
	msg.mtype=1;
	memset(msg.mtext,0,BUFSIZE);
	
	//有数据可以读
	if(msgrcv(msg_id0,&msg,BUFSIZE,0,IPC_NOWAIT)>0)
	{
		memcpy(subRecvBuf,msg.mtext,BUFSIZE);
		return 1;
	}
	else if(errno==ENOMSG)
		return 0;
	else
	{
		printf("Client write to %d fail!\n",msg_id0);
		return -1;
	}
	return 0;
}

/* 收发子进程过来的数据 */
int transferMsg(MYSQL *mysql,int client_num)
{
	int i=0;
	int frType;
	int msg_id0,msg_id1;
	char *text,*frame;
	struct Msg recvMsg,sendMsg;
	
	char *username;
	char targetUser[20];
	
	/* 对所有子进程的消息队列都试图读一遍 */
	for(;i<client_num;i++)
	{
		/* 获得msg_id0,msg_id1 */
		createMsgQue(client_num,&msg_id0,&msg_id1);
		
		/* 获得该子进程对应的用户名 */
		username=GetOnlineUsername(mysql,client_num);
		
		//接收消息
		memset(recvMsg.mtext,0,BUFSIZE);
		if(msgrcv(msg_id1,&recvMsg,BUFSIZE,0,IPC_NOWAIT)>0)//有数据可读
		{
			frType=getType(recvMsg.mtext);
			switch(frType)
			{
				case SfhOnLine：//如果是上/下线帧，解析出username后封装s->c的帧，转发给所有用户
				case SfhOffLine:
					analysisSfhOnOffLine(recvMsg.mtext,username);
					CrtOnOffFrame(username,0,&frame);
					memset(sendMsg.mtext,0,BUFSIZE);
					memcpy(sendMsg.mtext,frame,strlen(frame)+1);
					toAllUser(&sendMsg,client_num);
					break;
				case SfhText://如果是文本信息帧
					analysisSfhText(recvMsg.mtext,targetUser,&text);
					CrtTextFrame(username,text,&frame);
					memset(sendMsg.mtext,0,BUFSIZE);
					memcpy(sendMsg.mtext,frame,strlen(frame)+1);
					if(strcmp(targetUser,"all")==0)
						toAllUser(&sendMsg,client_num);
					else
						toSomeone(&sendMsg,targetUser);
			}
		}
		else if(errno==ENOMSG)//超时，没数据可读
			continue;
		else
		{
			printf("Server read from %d fail!\n",client_num);
			return -1;
		}
	}
}

/* 将数据群发给所有用户 */
int toAllUser(Msg *msg,int client_num)
{
	int i=0;
	int msg_id0,msg_id1;
	//在父进程中client_num代表子进程数量
	for(;i<client_num;i++)
	{
		/* 获得msg_id0,msg_id1（已存在直接返回对应的文件描述符） */
		createMsgQue(client_num,&msg_id0,&msg_id1);
		msgsnd(msg_id0,msg,strlen(msg.mtext)+1,0);
	}
	return 0;
}

/* 将数据发送给目标用户targetUser */
int toSomeone(MYSQL *mysql,Msg *msg,char targetUser[])
{
	int x = GetOnlineId(mysql,targetUser);
	int msg_id0,msg_id1;
	createMsgQue(x,&msg_id0,&msg_id1);
	if(msgsnd(msg_id0,msg,strlen(msg.mtext)+1,0)==-1)
	{
		printf("Client send to %d fail!\n",msg_id0);
		return -2;
	}
	else
		return 0;
}
