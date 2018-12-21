
#include "server.h"
#include <stdint.h>

/* �ȴ��ӽ��̽�����������ֽ�ʬ���� */
void wait4children(int sig)
{
	int status;
	while (waitpid(-1, &status, WNOHANG) > 0)
		;
}

/* ��ʼ������socket������ */
int initSocketFd(int *socket_fd, int port)
{
	struct sockaddr_in servaddr; //serverTCPЭ����
	if ((*socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("Fail to create socket:%s(errno:%d)\n", strerror(errno), errno);
		return -2;
	}

	//�ö˿ڸ���
	int opt = 1;
	if (setsockopt(*socket_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
	{
		printf("Fail to reuse port��errno=%d(%s)\n", errno, strerror(errno));
		close(*socket_fd);
		return -3;
	}

	//��ʼ��
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); //IP��ַ���ó�INADDR_ANY
	servaddr.sin_port = htons(port);			  //���õĶ˿�

	//���÷�����
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

	//��socket_fd
	if (bind(*socket_fd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
	{
		printf("Fail to bind socket:%s(errno:%d)\n", strerror(errno), errno);
		return -6;
	}

	//��ʼ�����Ƿ��пͻ�������
	if (listen(*socket_fd, MAXNUM_CLIENT) == -1)
	{
		printf("Fail to listen socket:%s(errno:%d)\n", strerror(errno), errno);
		return -7;
	}
	printf("Wait for clients......\n");
	return 0;
}

/* ��ʼ������socket������ */
int initConnection(int *connect_fd, int socket_fd)
{
	if ((*connect_fd = accept(socket_fd, (struct sockaddr *)NULL, NULL)) == -1)
	{
		printf("Socket connect error:%s(errno:%d)\n", strerror(errno), errno);
		return -1;
	}

	//���÷�����ģʽ
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

/* ��¼ʱ��ʼ���������û���¼ʱ����֤������ */
int initLogin(int *connect_fd, MYSQL *mysql, char username[], int client_num)
{
	char recvBuf[LEN32];
	char *sendBuf;
	char newSecret[15];	//������
	int replyType = Right; //Ӧ������ȱʡֵRight

	fd_set rfd, wfd;
	FD_ZERO(&rfd);
	FD_ZERO(&wfd);
	int maxfd = *connect_fd;

	FD_SET(*connect_fd, &rfd);
	if (select(maxfd + 1, &rfd, NULL, NULL, NULL) > 0) //�пɶ�������
	{
		/* ���մ�client�˷�����֡ */
		if (recv(*connect_fd, recvBuf, LEN32, 0) < 0)
		{
			printf("Recv username error!\n");
			close(*connect_fd);
			return -2;
		}
	}

	/* ����֡���м��飬�����ȷֱ�Ӹ������ݿ�*/
	int identifyResult = identify(mysql, recvBuf, username, client_num);
	switch (identifyResult)
	{
	case -2: //���Ǳ���֡,ֱ�ӹر�����
		close(*connect_fd);
		break;
	case -3: //�������
		replyType = SecretErr;
		break;
	case -4: //�û�������
		replyType = NameNoExist;
		break;
	case -5: //��Ҫ����
		replyType = NeedUpdateSecret;
		break;
	default: //��ȷ
		replyType = Right;
		break;
	}

	/* ����Ӧ��֡ */
	initReplyFrame(replyType, &sendBuf);
	// FD_SET(*connect_fd, &wfd);
	// if (select(maxfd + 1, NULL, &wfd, NULL, NULL) > 0) //�п�д������
	// {
	if (send(*connect_fd, sendBuf, LEN32, 0) < 0)
	{
		printf("Send reply error!\n");
		close(*connect_fd);
		return -3;
	}
	// }

	/* ���������û� */
	AddOnlineUser(mysql, username, client_num);
//	printf("user:%s finish - addonlineuser\n", username);

	/* ����½��Ϣд����־�������ɹ�/ʧ�� */
	//WriteReglog(username, replyType);
//	printf("user:%s finish - writelog\n", username);

	/* �����Ҫ�޸����룬�ȴ��ͻ��˷���SfhChangeSecret֡������ȡ���е����룬�������ݿ� */
	if (replyType == NeedUpdateSecret)
	{
		printf("Change keyword begin\n");
		if (changeSecret(connect_fd, mysql, username) < 0)
			return -4;
		WriteChgPswdlog(username);
		printf("Change keyword end\n");
	}

//	printf("user:%s finish - updatekeyword\n", username);

	/* ���ÿͻ��˷��ͺ��ѳ�ʼ��֡ */
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

	/* �������û����͸��û�������֡ */
	sendOnlineFrame(mysql, username);

	return 0;
}

/* �������û���������֡���������״̬ */
int interactBridge(int *connect_fd, MYSQL *mysql, char username[], int client_num)
{

	printf("Begin communication!\n");

	int sendCount = 0;	 //��Ҫ���͸��ӽ��̶�Ӧ�ͻ��˵�����֡����
	char **sendBuf;		   //��Ҫ���͸��ӽ��̶�Ӧ�ͻ��˵�����֡(��ά����)
	char recvBuf[BUFSIZE]; //���ӽ��̶�Ӧ�ͻ����յ�������֡

	fd_set rfd;
	int maxfd;
	struct timeval tv;
	int ret;

	char targetUsername[20];
	int isToALL = 0;   //�Ƿ��Ƿ��������û�
	char *msg;		   //�ӽ��̶�Ӧ�Ŀͻ���׼�����͵�����֡
	char *text;
	int frameType = 0; //���ӽ��̶�Ӧ�Ŀͻ����յ���֡������

	int replyType=Right;
	while (1)
	{
		/* �鿴�Ƿ���������Ҫ���͸����ӽ��̶�Ӧ�Ŀͻ��� */
		sendCount = GetSendMessage(mysql, username, &sendBuf);

		int i;
		for (i = 0; i < sendCount; i++)
		{
			uint16_t len=0;
			memcpy(&len,&(sendBuf[i][2]),2);
			
			
			if (send(*connect_fd, sendBuf[i], len, 0) < 0)
			{
				printf("Send error!\n");
				return -2;
			}

			printf("[others send to %s]:\n", username);
			Str2int2(sendBuf[i],len);
			
		}

		/* �鿴�ӽ��̶�Ӧ�Ŀͻ����Ƿ�������Ҫ���� */
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
			/* ���մ�client�˷�����֡ */
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
				/*********************************
				 * �������ı���Ϣ֡ ֡ͷ4�ֽ� [0]-0x12 [1]-0x0 [2:3]-֡������Ϣ��ʼ��ʾ���ͷ�@���� '\0' ֮������Ϣ
				 * Ҫ����ȥ���ı���Ϣ֡
				 *********************************/
				 
				analysisSfhText(recvBuf,targetUsername,&text);
				CrtTextFrame(username, text, &msg);
				
				uint16_t msglen=0;
				printf("%s is going to send:",username);
				msglen=getMsgLen(msg);
				Str2int2(msg,msglen);
				
				if (strcmp(targetUsername,"all")==0)
				{
					toAllUsers(mysql, username, msg);
					WriteAllLog(username);
				}
				else
				{
					SetMessageToDB(mysql, username, targetUsername, msg);
					printf("[%s send data to %s]:\n", username, targetUsername);
					Str2int2(msg,msglen);
					WriteSendText(username, targetUsername, 0); //0�Ƿ��ͳɹ���ʧ�ܵ�����
				}
				
				/* �����ı���ϢӦ��֡ */
				replyType=Right;
				CrtTextReplyFrame(replyType,&msg);
				if (send(*connect_fd, msg, 4, 0) < 0)
				{
					printf("Send textReply error!\n");
					return -3;
				}
				printf("%s send textReply successfully!\n",username);
			}
		}
	}

	return 0;
}

/* 
	�ж����Ӻ��յ��ĵ�һ֡�Ƿ�Ϊ����֡��
	����������֡��Я�����û����������ѯ���ݿ�������� 
	������ιbuf,��username
	*/
int identify(MYSQL *mysql, char buf[], char username[], int client_num)
{
	char secret[15];
	int tmp;

	/* �ж��յ����Ƿ�Ϊ����֡���������ֱ�ӶϿ�����*/
	if (getType(buf) != SfhRegister)
	{
		printf("The first frame isn't SfhRegister!\n");
		return -2;
	}

	/* ��������֡����ȡ�û��������� */
	analysisSfhRegister(buf, username, secret);

	/* ����û����������Ƿ���ȷ */
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

/* ���� */
int changeSecret(int *connect_fd, MYSQL *mysql, char username[])
{
	char buf[LEN16];
	char newSecret[16];
	int maxfd;
	fd_set rfd;
	FD_ZERO(&rfd);
	FD_SET(*connect_fd, &rfd);
	maxfd = *connect_fd + 1;
	if (select(maxfd + 1, &rfd, NULL, NULL, NULL) > 0) //�пɶ�������
	{
		/* ���մ�client�˷����ĸ���֡ */
		if (recv(*connect_fd, buf, LEN16, 0) < 0)
		{
			printf("Recv SfhChangeSecret error!\n");
			close(*connect_fd);
			return -2;
		}
	}

	if (getType(buf) != SfhChangeSecret) //������Ǹ���֡��ֱ�ӹر�����
	{
		printf("Not SfhChangeSecret!\n");
		close(*connect_fd);
		return -3;
	}

	//	Str2int2(buf,16);

	/* ��������֡����ȡ������ */
	analysisSfhChangeSecret(buf, newSecret);

	printf("newsecret %s\n", newSecret);

	/* �������ݿ���û������� */
	UpdateSecret(mysql, username, newSecret);

	return 0;
}


int sendOnlineFrame(MYSQL *mysql, char username[])
{
	/* ����һ������֡ */
	char *onlineFrame;
	int onlineFrame_length;
	onlineFrame_length = CrtOnLineFrame(username, &onlineFrame); //�����ڲ�����ռ�

	// printf("The online-frame is ");
	// Str2int2(onlineFrame, onlineFrame_length);
	// printf("\n");

	/* ������֡���͸������û� */

	toAllUsers(mysql, username, onlineFrame);

	free(onlineFrame);

	printf("onlineFrame\n");
	return 0;
}

int toAllUsers(MYSQL *mysql, char username[], char *msg)
{

	int i = 0;

	//������������û�
	char *AllOnlineUser= NULL;
	AllOnlineUser = GetAllOnlineUsers(mysql);

	//��õ�ǰ�����û�������
	int UserNum = 0;
	char *curUser = (char *)malloc(sizeof(char) * 16); //�û����16���ֽ�
	int usernamelen = 0;
	for (i = 0;; i++)
	{
		if ((AllOnlineUser[i] == '@' || AllOnlineUser[i] == '#'))
		{
			//�״ν��룬ֱ������
			if (usernamelen == 0)
				continue;

			/* һ���û���ʶ����� */
			curUser[usernamelen] = '\0'; //���β��
			if (strcmp(username, curUser))
			{
				//�Լ��������Լ�
				SetMessageToDB(mysql, username, curUser, msg);
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

/* ����buf�е�һ��e���±꣬isEnd�����Ƿ��Ѿ�����β */
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
