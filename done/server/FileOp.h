#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <time.h>
#include <unistd.h>//用于创建文件夹
#include <cstring>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include<stdint.h>
#include<iostream>
#include<fstream>
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

/***************************************************************************
  函数名称：TmpFile2Str
  功    能：临时文件转字符串
  输入参数：文件夹路径，文件内容指针，文件序号
  返 回 值：
  说    明：
***************************************************************************/
int TmpFile2Str(char*tmpDir,char*FileCont,uint32_t FileOrder);
/***************************************************************************
  函数名称：get_file_count
  功    能：获取文件夹下的文件数
  输入参数：路径
  返 回 值：
  说    明：
***************************************************************************/
int get_file_count(char *root);
/***************************************************************************
  函数名称：CrtFileDir
  功    能：创建路径
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int CrtFileDir(char*loc,char*SendName,char*RecvName,char*FileName);
/***************************************************************************
  函数名称：get_file_size
  功    能：得到文件长度
  输入参数：文件路径
  返 回 值：
  说    明：
***************************************************************************/
uint32_t get_file_size(const char *path)  ;
