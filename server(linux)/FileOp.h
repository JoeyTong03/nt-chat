#include <iostream>
#include <fstream>
#include <time.h>
#define FILESIZE 10*1024*1024
#define BLOCKSIZE 1000
struct MyFile
{
	char* head = NULL;
	unsigned int fileLength;
};

//将文件_filename中的内容读取到_myfile->head中,将长度读取到_myfile->filelength中
int readFileIn(const char _filename[], MyFile* _myfile);