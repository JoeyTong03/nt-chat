#include "server.h"

/*fork+阻塞*/
int main(int argc, char **argv)
{
	int socket_fd;						//用于监听的文件描述符
	struct sockaddr_in servaddr;		//serverTCP协议族
	if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printf("Fail to create socket:%s(errno:%d)\n", strerror(errno), errno);
		return -2;
	}

	//置端口复用
	int opt = 1;
	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0) {
		printf("Fail to reuse port：errno=%d(%s)\n", errno, strerror(errno));
		close(socket_fd);
		return -3;
	}

	//初始化
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//IP地址设置成INADDR_ANY
	servaddr.sin_port = htons(atoi(argv[1]));//设置的端口
									
	//绑定socket_fd
	if (bind(socket_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) 
	{
		printf("Fail to bind socket:%s(errno:%d)\n", strerror(errno), errno);
		return -6;
	}

	//开始监听是否有客户端连接
	if (listen(socket_fd, MAXNUM_CLIENT) == -1) 
	{
		printf("Fail to listen socket:%s(errno:%d)\n", strerror(errno), errno);
		return -7;
	}
	printf("Wait for clients......\n");

	(void)signal(SIGCHLD, wait4children);//捕捉子进程退出的信号
	int client_num = 0;//在父进程中表示有多少个连接的client，在子进程中表示子进程维护的是多少号连接
	int main_pid=getpid();//父进程的pid号
	int status;
	while (getpid()==main_pid)//只允许父进程fork子进程
	{
		//父进程只用于监听是否有新的连接，并接受这些连接
		int connect_fd;
		if ((connect_fd = accept(socket_fd, (struct sockaddr*)NULL, NULL)) == -1) 
		{
			printf("Socket connect error:%s(errno:%d)\n", strerror(errno), errno);
			return -9;
		}
		client_num++;
		//如果fork失败，延时1s后继续fork
		while((status=fork())<0)
			sleep(1);
		if (status==0 && client_num <= MAXNUM_CLIENT)
		{
			//关闭socket_fd，由子进程完全接手
			close(socket_fd);
			printf("Client%d connected!\n", client_num);
			while(1)
			{
				sleep(1);
			}
				
			close(connect_fd);
			exit(0);
		}
		else if (status > 0) //父进程
		{
			
			close(connect_fd);
		}
	}
}