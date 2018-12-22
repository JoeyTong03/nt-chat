#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <time.h>
#include <direct.h>//用于创建文件夹
#include <cstring>
#define FILESIZE 10*1024*1024
#define BLOCKSIZE 1000
#define MAXDIRNAMELEN 128
struct MyFile
{
	char* textHead = NULL;
	unsigned int fileLength;
};

//将文件_filename中的内容读取到_myfile->head中,将长度读取到_myfile->filelength中
int readFileIn(const char _filename[], MyFile* _myfile);

/*根据路径字符串创建一、多级目录*/
int createMultiFileList(char *strDirPath);

//根据源用户名、目标用户名、文件名调用createMultiFileList获创建对应的目录，目录名为：源用户名-目标用户名-文件名
int mkDir(char _sourceUsername[], char _targetUsername[], char _fileName[], char dirName[]);

//将接收到的(0,1000]字节的数据写入本地临时文件中
int saveToLocalTemp(char _dirName[],unsigned int _serialNumber, char* _text, unsigned int _fileLength);

//将临时文件整合成一整个文件ps:接口还没设计完全.....
int combineTmpFiles(char goalFilePath[], unsigned int tmpFilesNum);
