/* main.c */
#include "server.h"

int main(int argc, char **argv)
{
	/*---------------��������------------------ */
	int socket_fd;		//���ڼ������ļ�������
	int connect_fd;		//�û��������ӵ��ļ�������
	int client_num = 0; //�ڸ������б�ʾ�ж��ٸ����ӵ�client�����ӽ����б�ʾ�ӽ���ά�����Ƕ��ٺ�����
	int main_pid;		//�����̵�pid��

	int maxfd;	 //����select
	int selectRet; //select����ֵ

	int forkStatus; //fork����ֵ

	char username[20]; //�û���

	/*---------------�źŴ���------------------- */
	(void)signal(SIGCHLD, wait4children);

	/* ��ʼ��socket�ļ������� */
	if (initSocketFd(&socket_fd, atoi(argv[1])) < 0)
		return -1;

	main_pid = getpid();		 //�����̵�pid��
	while (getpid() == main_pid) //ֻ��������fork�ӽ���
	{

		/* �����Ƿ����µ����ӣ�����У���fork�ӽ��̴��������� */
		fd_set rfd;
		//struct timeval timeout = {0, 30}; //30msһ�֣���ʱ�ͽ�����һ��ѭ��
		maxfd = socket_fd;
		FD_ZERO(&rfd);
		FD_SET(socket_fd, &rfd);
		//selectRet = select(maxfd + 1, &rfd, NULL, NULL, &timeout);
		selectRet = select(maxfd + 1, &rfd, NULL, NULL, NULL);
		if (selectRet == 0) //��ʱ
			continue;

		if (selectRet > 0 && FD_ISSET(socket_fd, &rfd)) //˵�����µ�����
		{

			/* ��ʼ���������ӵ��ļ������� */
			if (initConnection(&connect_fd, socket_fd) < 0)
				continue;

			client_num++; //������Ϣ���е�key

			//���forkʧ�ܣ���ʱ1s�����fork
			while ((forkStatus = fork()) < 0)
				sleep(1);

			//�ӽ��̴�����client�˵Ľ���
			if (forkStatus == 0)
			{
				MYSQL *mysql; //���ݿ���

				/* ��ʼ�����ݿ� */
				InitDatabase(&mysql);

				printf("Client %d connected!\n", client_num);

				/* ��¼��ʼ���������û���¼ʱ����֤������ ������ȡusername*/
				initLogin(&connect_fd, mysql, username, client_num);

				/* ��ʽ������̣�ֱ��ת����������client�˵�֡ */
				interactBridge(&connect_fd, mysql, username, client_num);

				exit(0);
			}
			else if (forkStatus > 0) //������
			{
			}
		}
	}
}