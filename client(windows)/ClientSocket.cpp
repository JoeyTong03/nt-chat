#include "ClientSocket.h"
#include<iomanip>
#include<iostream>
using namespace std;

ClientSocket::ClientSocket()
{

}

ClientSocket::~ClientSocket()
{

}

/*************************************
 * �������ƣ�connectServer
 * ��    �ܣ��ͻ������ӷ�����
 * ��    ����
 * ��    �أ�bool - �Ƿ���ȷ����
 * ˵    ����ip/port�������й̶�Ϊ����
*************************************/
bool ClientSocket::connectServer()
{
	//�����µ�Socket�׽���
	socket = new QTcpSocket;
	socket->abort();

	//��������
	socket->connectToHost(ip, port);
	if (!socket->waitForConnected(3000)) //��ʽ���ӵȴ�3s
	{
		//�ȴ����ӳɹ�
		qDebug() << "connect failed";
		return false;
	}

	return true;
}

/*************************************
 * �������ƣ�closeServer
 * ��    �ܣ��ͻ��˶Ͽ�����
 * ��    ����
 * ��    �أ�
 * ˵    ����
*************************************/
void ClientSocket::closeConnect()
{
	socket->close();

}

/***************************************************************************
�������ƣ�Str2int
��    �ܣ���ʾ֡
���������buf:֡,length:֡����
�� �� ֵ��
˵    ����
***************************************************************************/
void Str2int(char*buf, int length)
{
	int i;
	for (i = 0; i<length; i++)
	{
		if ((buf[i] >= 'a'&&buf[i] <= 'z') || (buf[i] >= 'A' && buf[i] <= 'Z') || (buf[i] >= '0'&&buf[i] <= '9'))
			printf("%c ", buf[i]);
		else
			printf("%x ", (int)buf[i]);
		// if(i!=0&&i%4==0)
		//     printf(" ");
	}
	printf("%c", '\n');
}

/*************************************
 * �������ƣ�SendRegisterFrame
 * ��    �ܣ����ͱ���֡
 * ��    ����_username - �û��� secret - ����
 * ��    �أ�
 * ˵    ��������֡ - 32�ֽڣ�4�ֽ�֡ͷ+16�ֽ��û���+12�ֽ�����
 *			 ֡ͷ [0-7] 0x11 [8-15] 0x0 [16-31] 0x0020(���ĳ���-32byte)
*************************************/
void ClientSocket::SendRegisterFrame(const QString _username, const QString _secret) const
{
	//32�ֽڱ���
	char buf[32] = { 0 };

	//֡ͷ[0-7]
	memcpy(&(buf[0]), &SendFrameHead[SfhRegister], sizeof(char));

	//���ĳ���[16-31]??
	//char FrameLength = 0x20;
	//memcpy(&(buf[3]), &FrameLength, sizeof(char));
	uint16_t FrameLength = 32;
	memcpy(buf+2,&FrameLength,2);


	//�û��� - 16�ֽ�
	memcpy(&(buf[4]), _username.toStdString().c_str(), _username.length());

	//���� - 12�ֽ�
	memcpy(&(buf[20]), _secret.toStdString().c_str(), _secret.length());

	//������ת������


	socket->write(buf, sizeof(buf));

	//Str2int(buf, 32);

}

/***************************************************************************
�������ƣ�SendTextFrame
��    �ܣ������ı���Ϣ֡
���������_str���ı���Ϣ
		  DestUsername��Ŀ���û���
�� �� ֵ�����ͳɹ�����1��ʧ�ܷ���0
˵    ����4�ֽ�֡ͷ+@Ŀ���û���(�û�����all)+'\0'+�ı���Ϣ
***************************************************************************/
void ClientSocket::SendTextFrame(const QString _str,QString DestUsername)const
{
	uint16_t FrameLength = 4 + 1 + DestUsername.length() + _str.length() + 2;
	char* buf = new char[FrameLength];
	if (FrameLength > 1024)//�ı���Ϣ��������
	{
		qDebug() << "Length Limit" << endl;
		return;
	}
	memset(buf, '\0', FrameLength);
	//֡ͷ
	memcpy(buf, SendFrameHead + SfhText, sizeof(char));
	//���ĳ���
	memcpy(buf + 2, &FrameLength, sizeof(uint16_t));
	//����Ŀ��ͻ���
	char*name = new char[2 + DestUsername.length()];
	strcpy(name + 1, DestUsername.toStdString().data());
	name[0] = '@';
	memcpy(buf + 4, name, 1 + DestUsername.length());
	//�ı���Ϣ
	memcpy(buf + 6 + DestUsername.length(), _str.toStdString().c_str(), _str.length());

	socket->write(buf, FrameLength);

}

/***************************************************************************
�������ƣ�SendChangeSecretFrame
��    �ܣ����͸���֡
����������µ�����ֵ
�� �� ֵ��
˵    ����16�ֽڣ�4�ֽ�֡ͷ+12�ֽ��޸ĺ����룩
***************************************************************************/
void ClientSocket::SendChangeSecretFrame(const QString _secret)const
{
	char* buf = new char[16];
	uint16_t FrameLength = 16;
	memset(buf, '\0', FrameLength);
	//֡ͷ
	memcpy(buf, SendFrameHead + SfhChangeSecret, sizeof(char));
	//���ĳ���
	memcpy(buf + 2, &FrameLength, sizeof(uint16_t));
	//����������
	memcpy(buf + 4, _secret.toStdString().c_str(), _secret.length());

	socket->write(buf, FrameLength);

}

/***************************************************************************
�������ƣ�SendOffLineFrame
��    �ܣ�client����server�˷�������֡
���������
�� �� ֵ��
˵    ����
***************************************************************************/
void ClientSocket::SendOffLineFrame()const
{
	uint16_t FrameLength = 4;
	char*buf = new char[FrameLength];
	memset(buf, '\0', FrameLength);
	//֡ͷ
	memcpy(buf, SendFrameHead + SfhOffLine, sizeof(char));
	//���ĳ���
	memcpy(buf + 2, &FrameLength, sizeof(uint16_t));

	socket->write(buf, FrameLength);

}

/***************************************************************************
�������ƣ�AnlsFrame
��    �ܣ�����֡ͷ
�������������server��֡
�� �� ֵ��
˵    ����@��С��Ҫ�޸ĵĲ��֣�������case������ע�͵ĵط�����ÿ�����client��Ҫ�����Ĵ���
		֡���������Ч������ÿ��case�ڲ����������ݿ�ע��
***************************************************************************/
void ClientSocket::AnlsFrame(char*buf)
{
	switch ((int)buf[0])
	{
	case 0x71://����֡
		switch ((int)buf[1])
		{
		case 0://��½�ɹ�
			//
			break;
		case 1://�������
			//
			break;
		case 2://�û���������
			//
			break;
		case 3://�Ƿ���Ҫ����
			//
			break;
		default:
			break;
		}
		break;
	case 0x72://�ı���ϢӦ��֡
		switch ((int)buf[1])
		{
		case 0x00://ת���ɹ�
			//
			break;
		case 0xFF://��Ϣ�﷨����
			//
			break;
		case 0xFE://�������Ʋ�����
			//
			break;
		case 0xFD://����δ����
			//
			break;
		default:
			break;
		}
		break;
	case 0x77://�ı���Ϣ֡����server��ת�������ģ�
	{
		QString FromName;//��Ϣ��Դ
		QString Text = AcceptText(buf, FromName);//�ı���Ϣ
		//�����ı���Ϣ
		break;
	}
	case 0x73://������λ֡
		//������λ
		break;
	case 0x75://����/����֡
	{
		QString name = AcceptOnOffLineName(buf);//���߻������û���
		if ((int)buf[1] == 0)
		{
			//����
		}
		else
		{
			//����
		}
		break;
	}
	case 0x76://���ѳ�ʼ��
	{
		QVector<QString>namelist = AcceptInitFri(buf);
		//namelistΪ�����б�(vector)
		//
		break;
	}
	default:
		break;
	}
}

/***************************************************************************
�������ƣ�AcceptText
��    �ܣ������ı���Ϣ֡
����������ı���Ϣ֡����Ϣ��Դ�û���
�� �� ֵ�������ı�
˵    ����
***************************************************************************/
QString ClientSocket::AcceptText(char* TextFrame,QString&FromName)
{
	QString Text;
	Text.clear();
	FromName.clear();
	uint16_t TextLength;
	memcpy(&TextLength, TextFrame + 2, 2);
	FromName = QString(QLatin1String(TextFrame + 5));
	Text = QString(QLatin1String(TextFrame + 6 + FromName.size()));
	return Text;
}

/***************************************************************************
�������ƣ�AcceptOnOffLineName
��    �ܣ�������/����֡
���������������֡,
�� �� ֵ�����߻������û��������߻�����(0x0���ߣ�0x1����)
˵    ����
***************************************************************************/
QString ClientSocket::AcceptOnOffLineName(char* OnOffLineFrame)
{
	QString name = QString(QLatin1String(OnOffLineFrame + 4));
	//OnOffLine = (int)OnOffLineFrame[1];
	return name;
}

/***************************************************************************
�������ƣ�AcceptInitFri
��    �ܣ��������ѳ�ʼ��֡
������������ѳ�ʼ��֡�������û����б�
�� �� ֵ��
˵    ����
***************************************************************************/
QVector<QString> ClientSocket::AcceptInitFri(char* InitFriFrame)
{
	QVector<QString> NameList;
	uint8_t FriNum;
	memcpy(&FriNum, InitFriFrame + 1, 1);
	int startloc = 5;
	for (int i = 0; i < FriNum; i++)
	{
		QString name = QString(QLatin1String(InitFriFrame + startloc));
		NameList.push_back(name);
		InitFriFrame += name.size() + 2;
	}
	return NameList;
}


//Ŀǰ����
//ntohs