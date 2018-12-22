/* FileOp.h */
#include <iostream>
#include <fstream>
#include <time.h>
#define FILESIZE 10*1024*1024
#define BLOCKSIZE 1000
struct MyFile
{
	char* head=NULL;
	unsigned int filelength;
};

int readFileIn(const char _filename[], MyFile* _myfile,unsigned int* _filesize);