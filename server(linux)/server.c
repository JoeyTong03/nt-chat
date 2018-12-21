
#include "server.h"
#include <stdint.h>

/* 等待子进程结束，避免出现僵尸进程 */
void wait4children(int sig)
{
	int status;
	while (waitpid(-1, &status, WNOHANG) > 0)
		;
}

/* 初始化创建socket描述符 */
int initSocketFd(int *socket_fd, int port)
{
	struct sockaddr_in servaddr; //serverTCP协议族
	if ((*socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("Fail to create socket:%s(errno:%d)\n", strerror(errno), errno);
		return -2;
	}

	//置端口复用
	int opt = 1;
	if (setsockopt(*socket_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
	{
		printf("Fail to reuse port：errno=%d(%s)\n", errno, strerror(errno));
		close(*socket_fd);
		return -3;
	}

	//初始化
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); //IP地址设置成INADDR_ANY
	servaddr.sin_port = htons(port);			  //设置的端口

	//设置非阻塞
	int flags;
	if ((flags = fcntl(*socket_fd, F_GETFL, 0)) == -1)
	{
		printf("%s(errno:%d)\n", strerror(errno), errno);
		return -4;
	}
	if (fcntl(*socket_fd, F_SETFL, flags | O_NONBLOCK) == -1)
	{
		printf("%s(errno:%d)\n", strerror(errno), errno);
		return -5;
	}

	//绑定socket_fd
	if (bind(*socket_fd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
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
int initConnection(int *connect_fd, int socket_fd)
{
	if ((*connect_fd = accept(socket_fd, (struct sockaddr *)NULL, NULL)) == -1)
	{
		printf("Socket connect error:%s(errno:%d)\n", strerror(errno), errno);
		return -1;
	}

	//设置非阻塞模式
	int flags = 0;
	if ((flags = fcntl(*connect_fd, F_GETFL, 0)) == -1)
	{
		printf("%s(errno:%d)\n", strerror(errno), errno);
		return -2;
	}

	if (fcntl(*connect_fd, F_SETFL, flags | O_NONBLOCK) == -1)
	{
		printf("%s(errno:%d)\n", strerror(errno), errno);
		return -3;
	}

	return 0;
}

/* 登录时初始化，负责用户登录时的认证、改密 */
int initLogin(int *connect_fd, MYSQL *mysql, char username[], int client_num)
{
	char recvBuf[LEN32];
	char *sendBuf;
	char newSecret[15];	//新密码
	int replyType = Right; //应答类型缺省值Right

	fd_set rfd, wfd;
	FD_ZERO(&rfd);
	FD_ZERO(&wfd);
	int maxfd = *connect_fd;

	FD_SET(*connect_fd, &rfd);
	if (select(maxfd + 1, &rfd, NULL, NULL, NULL) > 0) //有可读的数据
	{
		/* 接收从client端发来的帧 */
		if (recv(*connect_fd, recvBuf, LEN32, 0) < 0)
		{
			printf("Recv username error!\n");
			close(*connect_fd);
			return -2;
		}
	}

	/* 对首帧进行检验，如果正确直接更新数据库*/
	int identifyResult = identify(mysql, recvBuf, username, client_num);
	switch (identifyResult)
	{
	case -2: //不是报道帧,直接关闭连接
		close(*connect_fd);
		break;
	case -3: //密码错误
		replyType = SecretErr;
		break;
	case -4: //用户不存在
		replyType = NameNoExist;
		break;
	case -5: //需要改密
		replyType = NeedUpdateSecret;
		break;
	default: //正确
		replyType = Right;
		break;
	}

	/* 发送应答帧 */
	initReplyFrame(replyType, &sendBuf);
	// FD_SET(*connect_fd, &wfd);
	// if (select(maxfd + 1, NULL, &wfd, NULL, NULL) > 0) //有可写的数据
	// {
	if (send(*connect_fd, sendBuf, LEN32, 0) < 0)
	{
		printf("Send reply error!\n");
		close(*connect_fd);
		return -3;
	}
	// }

	/* 更新在线用户 */
	AddOnlineUser(mysql, username, client_num);
//	printf("user:%s finish - addonlineuser\n", username);

	/* 将登陆信息写入日志，包括成功/失败 */
	//WriteReglog(username, replyType);
//	printf("user:%s finish - writelog\n", username);

	/* 如果需要修改密码，等待客户端发送SfhChangeSecret帧，并获取其中的密码，更新数据库 */
	if (replyType == NeedUpdateSecret)
	{
		printf("Change keyword begin\n");
		if (changeSecret(connect_fd, mysql, username) < 0)
			return -4;
		WriteChgPswdlog(username);
		printf("Change keyword end\n");
	}

//	printf("user:%s finish - updatekeyword\n", username);

	/* 给该客户端发送好友初始化帧 */
	char *nameList = GetAllUsers(mysql);
//	printf("user:%s finish - getalluser\n", username);

	char *friInitFrame;
	int friInitFrame_length;
	friInitFrame_length = CrtFriInit(nameList, &friInitFrame);
//	printf("user:%s finish - crtfriInit\n", username);

	if (send(*connect_fd, friInitFrame, friInitFrame_length, 0) < 0)
	{
		printf("Send friInitFrame error!\n");
		close(*connect_fd);
		return -5;
	}
//	printf("user:%s finish - send friend init frame\n", username);

//	printf("user:%s - len:%d - nameList:\n", username, friInitFrame_length);
//	Str2int2(friInitFrame, friInitFrame_length);
//	printf("\n\n");

	printf("send online frame begin \n");

	/* 给其他用户发送该用户的上线帧 */
	sendOnlineFrame(mysql, username);


	printf("init Login end\n");
	return 0;
}

/* 向其他用户发送上线帧后进入聊天状态 */
int interactBridge(int *connect_fd, MYSQL *mysql, char username[], int client_num)
{

	printf("Begin communication!\n");

	int sendCount = 0;	 //需要发送给子进程对应客户端的数据帧数量
	char **sendBuf;		   //需要发送给子进程对应客户端的数据帧(二维数组)
	char recvBuf[BUFSIZE]; //从子进程对应客户端收到的数据帧

	fd_set rfd;
	int maxfd;
	struct timeval tv;
	int ret;

	char targetUsername[20];
	int isToALL = 0;   //是否是发给所有用户
	char *msg;		   //子进程对应的客户端准备发送的数据帧
	int frameType = 0; //从子进程对应的客户端收到的帧的类型

	while (1)
	{
		/* 查看是否有数据需要发送给该子进程对应的客户端 */
		sendCount = GetSendMessage(mysql, username, &sendBuf);

		printf("user: %s sendcount: %d",username,sendCount);

		int i;
		for (i = 0; i < sendCount; i++)
		{
			if (send(*connect_fd, sendBuf[i], BUFSIZE, 0) < 0)
			{
				printf("Send error!\n");
				return -2;
			}
			printf("[others send to %s]:%s\n", username, sendBuf[i]);
			usleep(10);
		}

		/* 查看子进程对应的客户端是否有数据要发送 */
		FD_ZERO(&rfd);
		tv.tv_sec = 0;
		tv.tv_usec = 300;
		maxfd = *connect_fd;
		FD_SET(*connect_fd, &rfd);
		ret = select(maxfd + 1, &rfd, NULL, NULL, &tv);
		if (ret < 0)
		{
			printf("select error!\n");
			break;
		}
		else if (ret == 0)
			continue;
		else
		{
			/* 接收从client端发来的帧 */
			if (recv(*connect_fd, recvBuf, BUFSIZE, 0) < 0)
			{
				printf("Recv frame error!\n");
				close(*connect_fd);
				return -3;
			}

			frameType = getType(recvBuf);

			if (frameType == SfhOnLine || frameType == SfhOffLine)
			{
				toAllUsers(mysql, username, recvBuf);
				if (frameType == SfhOffLine)
				{
					printf("User %s offline!\n", username);
					DelOnlineUser(mysql, username);
					WriteOfflinelog(username);
				}
			}
			else if (frameType == SfhText)
			{
				CrtTextFrame(username, recvBuf, &msg);

				isToALL = 0;
				getTargetUsername(recvBuf, targetUsername, &isToALL);

				if (isToALL)
				{
					toAllUsers(mysql, username, msg);
					WriteAllLog(username);
				}
				else
				{
					SetMessageToDB(mysql, username, targetUsername, msg);
					printf("[%s send data to %s]:%s\n", username, targetUsername, recvBuf);
					WriteSendText(username, targetUsername, 0); //0是发送成功或失败的类型
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
int identify(MYSQL *mysql, char buf[], char username[], int client_num)
{
	char secret[15];
	int tmp;

	/* 判断收到的是否为报道帧，如果不是直接断开连接*/
	if (getType(buf) != SfhRegister)
	{
		printf("The first frame isn't SfhRegister!\n");
		return -2;
	}

	/* 解析报道帧，获取用户名及密码 */
	analysisSfhRegister(buf, username, secret);

	/* 检查用户名及密码是否正确 */
	tmp = JudgeUser(mysql, username, secret);

	switch (tmp)
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
	FD_SET(*connect_fd, &rfd);
	maxfd = *connect_fd + 1;
	if (select(maxfd + 1, &rfd, NULL, NULL, NULL) > 0) //有可读的数据
	{
		/* 接收从client端发来的改密帧 */
		if (recv(*connect_fd, buf, LEN16, 0) < 0)
		{
			printf("Recv SfhChangeSecret error!\n");
			close(*connect_fd);
			return -2;
		}
	}

	if (getType(buf) != SfhChangeSecret) //如果不是改密帧，直接关闭连接
	{
		printf("Not SfhChangeSecret!\n");
		close(*connect_fd);
		return -3;
	}

	//	Str2int2(buf,16);

	/* 解析改密帧，获取新密码 */
	analysisSfhChangeSecret(buf, newSecret);

	printf("newsecret %s\n", newSecret);

	/* 更新数据库该用户的密码 */
	UpdateSecret(mysql, username, newSecret);

	return 0;
}

/* 如果是给所有用户的，则isToALL=1;否则通过targetUsername返回目标用户名 */
int getTargetUsername(char buf[], char targetUsername[], int *isToALL)
{
	strcpy(targetUsername, buf + 4);
	if (strcmp(targetUsername, "all") == 0)
		*isToALL = 1;
	return 0;
}

int sendOnlineFrame(MYSQL *mysql, char username[])
{
	/* 生成一个上线帧 */
	char *onlineFrame;
	int onlineFrame_length;
	onlineFrame_length = CrtOnLineFrame(username, &onlineFrame); //函数内部申请空间

	// printf("The online-frame is ");
	// Str2int2(onlineFrame, onlineFrame_length);
	// printf("\n");

	/* 将上线帧发送给所有用户 */
	toAllUsers(mysql, username, onlineFrame);

	free(onlineFrame);

	printf("onlineFrame\n");
	return 0;
}

int toAllUsers(MYSQL *mysql, char username[], char *msg)
{
	// char *p;
	// char *allUserText = NULL; //所有在线用户，格式：“@name1@name2....#”
	// char targetUsername[20];
	// int isEnd = 0;
	// int len = 0;
	// allUserText = GetAllOnlineUsers(mysql);
	// p = allUserText + 1; //避开第一个@
	// while (1)
	// {
	// 	len = myfind(p, '@', &isEnd);
	// 	strncpy(targetUsername, p, len);
	// 	p += (len + 1);
	// 	if (strcmp(targetUsername, username) == 0)
	// 		continue;
	// 	SetMessageToDB(mysql, username, targetUsername, msg);
	// 	if (isEnd == 1)
	// 		break;
	// 	printf("[%s send data to %s]:", username, targetUsername);
	// 	uint16_t frameLen = getMsgLen(msg);
	// 	Str2int2(msg, frameLen);
	// 	printf("\n");
	// }

	int i = 0;

	//获得所有在线用户
	char *AllOnlineUser= NULL;
	AllOnlineUser = GetAllOnlineUsers(mysql);

	//获得当前在线用户的数量
	int UserNum = 0;
	char *curUser = (char *)malloc(sizeof(char) * 16); //用户名最长16个字节
	int usernamelen = 0;
	for (i = 0;; i++)
	{
		if ((AllOnlineUser[i] == '@' || AllOnlineUser[i] == '#'))
		{
			//首次进入，直接跳过
			if (usernamelen == 0)
				continue;

			/* 一个用户名识别完毕 */
			curUser[usernamelen] = '\0'; //添加尾零
			if (strcmp(username, curUser))
			{
				SetMessageToDB(mysql, username, curUser, msg);
			}
			else
			{
				//如果重名则跳过
				continue;
			}

			usernamelen = 0;

			if (AllOnlineUser[i] == '#')
				break;

			continue;
		}

		curUser[usernamelen] = AllOnlineUser[i];
		usernamelen++;
	}
}

/* 返回buf中第一个e的下标，isEnd返回是否已经到结尾 */
int myfind(char buf[], char e, int *isEnd)
{
	int i;
	for (i = 0; buf[i] != e && buf[i] != '#'; i++)
		;
	if (buf[i] == '#')
		*isEnd = 1;
	return i;
}

uint16_t getMsgLen(char *msg)
{
	uint16_t len;
	char lenStr[2];
	memcpy(&len, msg + 2, 2);
	return len;
}
