#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <time.h>
#include <direct.h>//���ڴ����ļ���
#include <cstring>
#define FILESIZE 10*1024*1024
#define BLOCKSIZE 1000
#define MAXDIRNAMELEN 128

struct MyFile
{
	char* textHead = NULL;
	unsigned int fileLength;
};

//���ļ�_filename�е����ݶ�ȡ��_myfile->head��,�����ȶ�ȡ��_myfile->filelength��
int readFileIn(const char _filename[], MyFile* _myfile);

/*����·���ַ�������һ���༶Ŀ¼*/
int createMultiFileList(char *strDirPath);

//����Դ�û�����Ŀ���û������ļ�������createMultiFileList�񴴽���Ӧ��Ŀ¼��Ŀ¼��Ϊ��Դ�û���-Ŀ���û���-�ļ���
int mkDir(char _sourceUsername[], char _targetUsername[], char _fileName[], char dirName[]);

//�����յ���(0,1000]�ֽڵ�����д�뱾����ʱ�ļ���
int saveToLocalTemp(char _dirName[],unsigned int _serialNumber, char* _text, unsigned int _fileLength);

//����ʱ�ļ����ϳ�һ�����ļ�,goalFileNameΪĿ���ļ�����tmpDirPath����ʱ�ļ���Ŀ¼����xxx-xxx-xxx
int combineTmpFiles(char goalFileName[], char tmpDir[], unsigned int tmpFilesNum);
