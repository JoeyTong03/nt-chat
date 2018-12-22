/*
	fileOp.cpp
	�ļ��շ�ʱʹ�õĵĲ���
*/

#include "FileOp.h"

using namespace std;
int readFileIn(const char _filename[], MyFile* _myfile)
{
	//���ļ�
	ifstream infile(_filename, ios::in|ios::binary);
	if (!infile.is_open())
	{
		cout << "error opening file"; 
		exit(1);
	}

	//��ȡ�ļ�����
	infile.seekg(0, ios_base::end);
	streampos sp = infile.tellg();
	_myfile->fileLength = sp;

	//���붯̬�ռ�
	_myfile->head = new(nothrow)char[_myfile->fileLength];
	if (NULL == _myfile->head)
	{
		cout << "alloc fail!" << endl;
		return -1;
	}

	//���ļ��е����ݰᵽ_myfile->headָ��ָ��Ŀռ���
	infile.seekg(0, ios_base::beg);
	infile.read(_myfile->head, _myfile->fileLength);
	infile.close();
	return 0;
}

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

	MyFile myfile;
	char fileName[] = "data.txt";
	readFileIn(fileName, &myfile);
	cout << "FileSize is " << myfile.fileLength << endl;
	
	char* p = myfile.head;
	cout << p[0] << endl;
	cout << (p + BLOCKSIZE)[0] << endl;
	delete[] myfile.head;
	return 0;
}
