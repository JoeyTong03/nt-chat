/*
	fileOp.cpp
	文件收发时使用的的操作
*/

#include "FileOp.h"

using namespace std;

//将文件_filename中的内容读取到_myfile->head中,将长度读取到_myfile->filelength中
int readFileIn(const char _filename[], MyFile* _myfile)
{
	//打开文件
	ifstream infile(_filename, ios::in|ios::binary);
	if (!infile.is_open())
	{
		cout << "error opening file"; 
		return -1;
	}

	//获取文件长度
	infile.seekg(0, ios_base::end);
	streampos sp = infile.tellg();
	_myfile->fileLength = sp;

	//申请动态空间
	_myfile->textHead = new(nothrow)char[_myfile->fileLength];
	if (NULL == _myfile->textHead)
	{
		cout << "alloc fail!" << endl;
		return -1;
	}

	//将文件中的数据搬到_myfile->head指针指向的空间中
	infile.seekg(0, ios_base::beg);
	infile.read(_myfile->textHead, _myfile->fileLength);
	infile.close();
	return 0;
}

/*根据路径字符串创建一、多级目录*/
int createMultiFileList(char *strDirPath)
{
	if (strlen(strDirPath) > MAXDIRNAMELEN)
	{
		return -1;
	}
	int ipathLength = strlen(strDirPath);
	int ileaveLength = 0;
	int iCreatedLength = 0;
	char szPathTemp[MAXDIRNAMELEN] = { 0 };
	for (int i = 0; (NULL != strchr(strDirPath + iCreatedLength, '\\')); i++)
	{
		ileaveLength = strlen(strchr(strDirPath + iCreatedLength, '\\')) - 1;
		iCreatedLength = ipathLength - ileaveLength;
		strncpy(szPathTemp, strDirPath, iCreatedLength);
		mkdir(szPathTemp,0777);
	}
	if (iCreatedLength < ipathLength)
	{
		mkdir(strDirPath,0777);
	}
	return 0;
}

//根据源用户名、目标用户名、文件名获创建对应的目录并进入该目录，目录名为：源用户名-目标用户名-文件名
int mkDir(char _sourceUsername[], char _targetUsername[], char _fileName[], char dirName[])
{
	char s[64];
	sprintf(s,"%s-%s-%s", _sourceUsername, _targetUsername, _fileName);
	strcpy(dirName, "tmpFiles\\");
	strcat(dirName, s);
	createMultiFileList(dirName);
	return 0;
}

//将接收到的(0,1000]字节的数据写入本地临时文件中
int saveToLocalTemp(char _dirName[], unsigned int _serialNumber, char* _text, unsigned int _fileLength)
{
	char filePath[MAXDIRNAMELEN];
	mkdir(_dirName,0777);
	sprintf(filePath, "%s/%d", _dirName, _serialNumber);
	ofstream outfile(filePath, ios::out | ios::binary);
	if (!outfile.is_open())
	{
		cout << filePath << " open fail!" << endl;
		return -1;
	}
	outfile.write(_text, _fileLength);
	outfile.close();
	return 0;
}

//将临时文件整合成一整个文件,goalFileName为目标文件名，tmpDirPath是临时文件的目录，如xxx-xxx-xxx
int combineTmpFiles(char goalFileName[],char tmpDir[], unsigned int tmpFilesNum)
{
	char tmpFilePath[10];
	char goalFilePath[32];
	MyFile myfile;

	sprintf(goalFilePath, "%s\\%s", tmpDir, goalFileName);
	ofstream outfile(goalFilePath, ios::out | ios::binary | ios::app);
	if (!outfile.is_open())
	{
		cout << goalFilePath << " open fail!" << endl;
		return -1;
	}

	for (int i = 1; i <= tmpFilesNum; i++)
	{
		sprintf(tmpFilePath, "%s\\%d", tmpDir, i);
		cout << tmpFilePath << endl;
		readFileIn(tmpFilePath, &myfile);
		outfile.write(myfile.textHead, myfile.fileLength);
		delete[](myfile.textHead);
	}
	outfile.close();
	return 0;
}
/***************************************************************************
  函数名称：TmpFile2Str
  功    能：临时文件转字符串
  输入参数：文件夹路径，文件内容指针，文件序号
  返 回 值：
  说    明：
***************************************************************************/
int TmpFile2Str(char*tmpDir,char*FileCont,uint32_t FileOrder)
{
	char loc[64];
	sprintf(loc,"%s/%d",tmpDir,FileOrder);

	ifstream infile(loc, ios::in | ios::binary);
	if (!infile.is_open())
	{
		cout << "Open " << loc << " Error" << endl;
		return(-1);
	}
	infile.getline(FileCont,1000);
	return 1;
}
/***************************************************************************
  函数名称：get_file_count
  功    能：获取文件夹下的文件数
  输入参数：路径
  返 回 值：
  说    明：
***************************************************************************/
int get_file_count(char *root)
{
	DIR *dir;
	struct dirent *ptr;
	int total = 0;
	char path[1024];
	dir = opendir(root); /* 打开目录*/
	if (dir == NULL)
	{
		perror("fail to open dir");
		exit(1);
	}
	errno = 0;
	while ((ptr = readdir(dir)) != NULL)
	{
		//顺序读取每一个目录项；
		//跳过“..”和“.”两个目录
		if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)
		{
			continue;
		}
		//printf("%s%s/n",root,ptr->d_name);
		//如果是目录，则递归调用 get_file_count函数
		if (ptr->d_type == DT_DIR)
		{
			sprintf(path, "%s%s/", root, ptr->d_name);
			//printf("%s/n",path);
			total += get_file_count(path);
		}
		if (ptr->d_type == DT_REG)
		{
			total++;
			//printf("%s%s/n", root, ptr->d_name);
		}
	}
	if (errno != 0)
	{
		printf("fail to read dir"); //失败则输出提示信息
		exit(1);
	}
	closedir(dir);
	return total;
}

/***************************************************************************
  函数名称：CrtFileDir
  功    能：创建路径
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int CrtFileDir(char*loc,char*SendName,char*RecvName,char*FileName)
{
	sprintf(loc,"%s-%s-%s",SendName,RecvName,FileName);
	return 1;
}
/***************************************************************************
  函数名称：get_file_size
  功    能：得到文件长度
  输入参数：文件路径
  返 回 值：
  说    明：
***************************************************************************/
uint32_t get_file_size(const char *path)  
{  
    uint32_t filesize = -1;  
    FILE *fp;  
    fp = fopen(path, "r");  
    if(fp == NULL)  
        return filesize;  
    fseek(fp, 0L, SEEK_END);  
    filesize = ftell(fp);  
    fclose(fp);  
    return filesize;  
}
// int main()
// {
// 	/*生成10M文件*/
// /*
// 	ofstream outfile("data.txt", ios::binary | ios::out);
// 	char *data;
// 	data = new(nothrow) char[FILESIZE];
// 	if (data == NULL)
// 	{
// 	printf("alloc fail!\n");
// 	return -1;
// 	}
// 	else
// 	printf("alloc success!\n");
// 	srand((int)time(0));
// 	for (int i = 0; i < FILESIZE; i++)
// 	data[i] = rand() % 10 + '0';
// 	cout << "hhh" << endl;
// 	outfile.write(data, FILESIZE);*/


// 	/*测试readFileIn函数*/
// 	/*MyFile myfile;
// 	char fileName[] = "data.txt";
// 	readFileIn(fileName, &myfile);
// 	cout << "FileSize is " << myfile.fileLength << endl;
	
// 	char* p = myfile.textHead;
// 	cout << p[0] << endl;
// 	cout << (p + BLOCKSIZE)[0] << endl;
// 	delete[] myfile.head;
// 	return 0;*/

// 	/*测试mkDir函数*/
// 	//char sourceName[] = "Miao";
// 	//char targetName[] = "tong";
// 	//char fileName[] = "nt";
// 	//unsigned int serialNumber = 2;
// 	//unsigned int fileLenth = 18;
// 	//char *text;
// 	//char dirName[MAXDIRNAMELEN];
// 	//text = new(nothrow) char[fileLenth];
// 	//if (NULL == text)
// 	//{
// 	//	cout << "alloc fail!\n" << endl;
// 	//	return -1;
// 	//}
// 	//strcpy(text, "I AM A SUPER WOMAN");
// 	//mkDir(sourceName, targetName, fileName, dirName);
// 	//saveToLocalTemp(dirName, serialNumber, text, fileLenth);
// 	//delete[] text;
// 	//return 0;

// 	/*测试combineTmpFiles*/
// 	char tmpDir[] = "tmpFiles\\Miao-tong-nt";
// 	char goalFileName[] = "nt";
// 	unsigned int tmpFileNum = 4;
// 	combineTmpFiles(goalFileName, tmpDir, tmpFileNum);
// 	return 0;
// }
