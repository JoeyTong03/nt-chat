#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define BUFSIZE 2048

int main(int argc,char *argv[])
{
	int client_sockfd;
	int len;
	struct sockaddr_in remote_addr;//客户端网络地址结构体
	char buf[BUFSIZE];
	memset(&remote_addr,0,sizeof(remote_addr));
	remote_addr.sin_family=AF_INET;//通信协议为因特网
	remote_addr.sin_port=htons(atoi(argv[2]));//服务器端口号
	remote_addr.sin_addr.s_addr=inet_addr(argv[1]);//服务器IP地址

	/*创建客户端套接字*/
	if((client_sockfd=socket(PF_INET,SOCK_STREAM,0))<0)
	{
		perror("socket error");
		return 1;
	}

	/*将套接字绑定到客户端网络地址上*/
	if(connect(client_sockfd,(struct sockaddr *)&remote_addr,sizeof(struct sockaddr))<0)
	{
		perror("connect error");
		return 1;
	}

	char sendData[]="Hello Mr Server,I'm lady Client1!\n";
	if(send(client_sockfd,sendData,strlen(sendData),0)<0)
	{
		perror("send error");
		return 1;
	}

	// len=recv(client_sockfd,buf,BUFSIZE,0);//接收服务器端信息
	// buf[len]='\0';
	// printf("from server:%s\n",buf);
	// printf("----------------------------------\n");
	printf("Begin communicate with server:\n");
	int i=1;
	while(1)
	{
		printf("[Message[%d] to server:",i++);
		scanf("%s",buf);
		if(strcmp(buf,"quit")==0)
			break;
		len=send(client_sockfd,buf,strlen(buf),0);
		if((len=recv(client_sockfd,buf,BUFSIZE,0))<=0)
			break;
		
		buf[len]='\0';
		printf("reply from server:%s\n",buf);
	}
	close(client_sockfd);
	return 0;
}
