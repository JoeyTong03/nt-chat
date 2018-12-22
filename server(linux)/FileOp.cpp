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
		exit(1);
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
		_mkdir(szPathTemp);
	}
	if (iCreatedLength < ipathLength)
	{
		_mkdir(strDirPath);
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
	sprintf(filePath, "%s\\%d", _dirName, _serialNumber);
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

//将临时文件整合成一整个文件ps:接口还没设计完全.....
int combineTmpFiles(char goalFilePath[], unsigned int tmpFilesNum)
{

}

int main()
{
	/*生成10M文件*/
/*
	ofstream outfile("data.txt", ios::binary | ios::out);
	char *data;
	data = new(nothrow) char[FILESIZE];
	if (data == NULL)
	{
	printf("alloc fail!\n");
	return -1;
	}
	else
	printf("alloc success!\n");
	srand((int)time(0));
	for (int i = 0; i < FILESIZE; i++)
	data[i] = rand() % 10 + '0';
	cout << "hhh" << endl;
	outfile.write(data, FILESIZE);*/


	/*测试readFileIn函数*/
	/*MyFile myfile;
	char fileName[] = "data.txt";
	readFileIn(fileName, &myfile);
	cout << "FileSize is " << myfile.fileLength << endl;
	
	char* p = myfile.textHead;
	cout << p[0] << endl;
	cout << (p + BLOCKSIZE)[0] << endl;
	delete[] myfile.head;
	return 0;*/

	/*测试mkDir函数*/
	char sourceName[] = "Miao";
	char targetName[] = "tong";
	char fileName[] = "nt";
	unsigned int serialNumber = 2;
	unsigned int fileLenth = 18;
	char *text;
	char dirName[MAXDIRNAMELEN];
	text = new(nothrow) char[fileLenth];
	if (NULL == text)
	{
		cout << "alloc fail!\n" << endl;
		return -1;
	}
	strcpy(text, "I AM A SUPER WOMAN");
	mkDir(sourceName, targetName, fileName, dirName);
	saveToLocalTemp(dirName, serialNumber, text, fileLenth);
	delete[] text;
	return 0;
}
