/* main.c */
#include "server.h"

int main(int argc, char **argv)
{
	/*---------------变量定义------------------ */
	int socket_fd;		//用于监听的文件描述符
	int connect_fd;		//用户建立连接的文件描述符
	int client_num = 0; //在父进程中表示有多少个连接的client，在子进程中表示子进程维护的是多少号连接
	int main_pid;		//父进程的pid号

	int maxfd;	 //用于select
	int selectRet; //select返回值

	int forkStatus; //fork返回值

	char username[20]; //用户名

	/*---------------信号处理------------------- */
	(void)signal(SIGCHLD, wait4children);

	/* 初始化socket文件描述符 */
	if (initSocketFd(&socket_fd, atoi(argv[1])) < 0)
		return -1;

	main_pid = getpid();		 //父进程的pid号
	while (getpid() == main_pid) //只允许父进程fork子进程
	{

		/* 监听是否有新的连接；如果有，则fork子进程处理并发连接 */
		fd_set rfd;
		//struct timeval timeout = {0, 30}; //30ms一轮，超时就进入下一个循环
		maxfd = socket_fd;
		FD_ZERO(&rfd);
		FD_SET(socket_fd, &rfd);
		//selectRet = select(maxfd + 1, &rfd, NULL, NULL, &timeout);
		selectRet = select(maxfd + 1, &rfd, NULL, NULL, NULL);
		if (selectRet == 0) //超时
			continue;

		if (selectRet > 0 && FD_ISSET(socket_fd, &rfd)) //说明有新的连接
		{

			/* 初始化用于连接的文件描述符 */
			if (initConnection(&connect_fd, socket_fd) < 0)
				continue;

			client_num++; //用于消息队列的key

			//如果fork失败，延时1s后继续fork
			while ((forkStatus = fork()) < 0)
				sleep(1);

			//子进程处理与client端的交互
			if (forkStatus == 0)
			{
				MYSQL *mysql; //数据库句柄

				/* 初始化数据库 */
				InitDatabase(&mysql);

				printf("Client %d connected!\n", client_num);

				/* 登录初始化，处理用户登录时的认证、改密 ，并获取username*/
				initLogin(&connect_fd, mysql, username, client_num);

				/* 正式聊天过程：直接转发父进程与client端的帧 */
				interactBridge(&connect_fd, mysql, username, client_num);

				exit(0);
			}
			else if (forkStatus > 0) //父进程
			{
			}
		}
	}
}