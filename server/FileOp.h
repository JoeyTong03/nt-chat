#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <time.h>
#include <unistd.h>//���ڴ����ļ���
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

//���ļ�_filename�е����ݶ�ȡ��_myfile->head��,�����ȶ�ȡ��_myfile->filelength��
int readFileIn(const char _filename[], MyFile* _myfile);

/*����·���ַ�������һ���༶Ŀ¼*/
int createMultiFileList(char *strDirPath);

//����Դ�û�����Ŀ���û������ļ�������createMultiFileList�񴴽���Ӧ��Ŀ¼��Ŀ¼��Ϊ��Դ�û���-Ŀ���û���-�ļ���
int mkDir(char _sourceUsername[], char _targetUsername[], char _fileName[], char dirName[]);

//�����յ���(0,1000]�ֽڵ�����д�뱾����ʱ�ļ���
int saveToLocalTemp(char _dirName[],unsigned int _serialNumber, char* _text, unsigned int _fileLength);

//����ʱ�ļ����ϳ�һ�����ļ�ps:�ӿڻ�û�����ȫ.....
int combineTmpFiles(char goalFilePath[], unsigned int tmpFilesNum);

/***************************************************************************
  �������ƣ�TmpFile2Str
  ��    �ܣ���ʱ�ļ�ת�ַ���
  ����������ļ���·�����ļ�����ָ�룬�ļ����
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int TmpFile2Str(char*tmpDir,char*FileCont,uint32_t FileOrder);
/***************************************************************************
  �������ƣ�get_file_count
  ��    �ܣ���ȡ�ļ����µ��ļ���
  ���������·��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int get_file_count(char *root);
/***************************************************************************
  �������ƣ�CrtFileDir
  ��    �ܣ�����·��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int CrtFileDir(char*loc,char*SendName,char*RecvName,char*FileName);
/***************************************************************************
  �������ƣ�get_file_size
  ��    �ܣ��õ��ļ�����
  ����������ļ�·��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
uint32_t get_file_size(const char *path)  ;
