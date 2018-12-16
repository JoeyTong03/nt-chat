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

void wait4children(int sig)
{
	int status;
	while (waitpid(-1, &status, WNOHANG) > 0);
}