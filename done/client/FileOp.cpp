/*
	fileOp.cpp
	�ļ��շ�ʱʹ�õĵĲ���
*/

#include "FileOp.h"

using namespace std;

//���ļ�_filename�е����ݶ�ȡ��_myfile->head��,�����ȶ�ȡ��_myfile->filelength��
int readFileIn(const char _filename[], MyFile* _myfile)
{
	//���ļ�
	ifstream infile(_filename, ios::in|ios::binary);
	if (!infile.is_open())
	{
		cout << "error opening file"; 
		return -1;
	}

	//��ȡ�ļ�����
	infile.seekg(0, ios_base::end);
	streampos sp = infile.tellg();
	_myfile->fileLength = sp;

	//���붯̬�ռ�
	_myfile->textHead = new(nothrow)char[_myfile->fileLength];
	if (NULL == _myfile->textHead)
	{
		cout << "alloc fail!" << endl;
		return -1;
	}

	//���ļ��е����ݰᵽ_myfile->headָ��ָ��Ŀռ���
	infile.seekg(0, ios_base::beg);
	infile.read(_myfile->textHead, _myfile->fileLength);
	infile.close();
	return 0;
}

/*����·���ַ�������һ���༶Ŀ¼*/
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

//����Դ�û�����Ŀ���û������ļ����񴴽���Ӧ��Ŀ¼�������Ŀ¼��Ŀ¼��Ϊ��Դ�û���-Ŀ���û���-�ļ���
int mkDir(char _sourceUsername[], char _targetUsername[], char _fileName[], char dirName[])
{
	char s[64];
	sprintf(s,"%s-%s-%s", _sourceUsername, _targetUsername, _fileName);
	strcpy(dirName, "tmpFiles\\");
	strcat(dirName, s);
	createMultiFileList(dirName);
	return 0;
}

//�����յ���(0,1000]�ֽڵ�����д�뱾����ʱ�ļ���
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

//����ʱ�ļ����ϳ�һ�����ļ�,goalFileNameΪĿ���ļ�����tmpDirPath����ʱ�ļ���Ŀ¼����xxx-xxx-xxx
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

#ifdef TESTFILEOP

int main()
{
	/*����10M�ļ�*/
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


	/*����readFileIn����*/
	/*MyFile myfile;
	char fileName[] = "data.txt";
	readFileIn(fileName, &myfile);
	cout << "FileSize is " << myfile.fileLength << endl;

	char* p = myfile.textHead;
	cout << p[0] << endl;
	cout << (p + BLOCKSIZE)[0] << endl;
	delete[] myfile.head;
	return 0;*/

	/*����mkDir����*/
	//char sourceName[] = "Miao";
	//char targetName[] = "tong";
	//char fileName[] = "nt";
	//unsigned int serialNumber = 2;
	//unsigned int fileLenth = 18;
	//char *text;
	//char dirName[MAXDIRNAMELEN];
	//text = new(nothrow) char[fileLenth];
	//if (NULL == text)
	//{
	//	cout << "alloc fail!\n" << endl;
	//	return -1;
	//}
	//strcpy(text, "I AM A SUPER WOMAN");
	//mkDir(sourceName, targetName, fileName, dirName);
	//saveToLocalTemp(dirName, serialNumber, text, fileLenth);
	//delete[] text;
	//return 0;

	/*����combineTmpFiles*/
	char tmpDir[] = "tmpFiles\\Miao-tong-nt";
	char goalFileName[] = "nt";
	unsigned int tmpFileNum = 4;
	combineTmpFiles(goalFileName, tmpDir, tmpFileNum);
	return 0;
}


#endif // TESTFILEOP

