
#include "ClientSocket.h"

#include <QMessageBox>
#include <QString>

ClientSocket::ClientSocket(QWidget *parent)
	: QWidget(parent)
{

}

ClientSocket::~ClientSocket()
{
	closeConnect();
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
	socket = new QTcpSocket(this);
	socket->abort();

	//��������
	socket->connectToHost(ip, port);
	if (!socket->waitForConnected(10000)) //��ʽ���ӵȴ�30s
	{
		//�ȴ����ӳɹ�
		qDebug() << "connect failed";
		return false;
	}

	//Qt Socket�źŲۺ���
	{
		connect(socket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnected()));
		connect(socket, SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()));
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
	memcpy(buf + 2, &FrameLength, 2);


	//�û��� - 16�ֽ�
	memcpy(&(buf[4]), _username.toStdString().c_str(), _username.length());

	//���� - 12�ֽ�
	memcpy(&(buf[20]), _secret.toStdString().c_str(), _secret.length());

	//������ת������


	socket->write(buf, sizeof(buf));
	socket->waitForBytesWritten();


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
void ClientSocket::SendTextFrame(const QString _str, QString DestUsername)const
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
	socket->waitForBytesWritten();

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
	socket->waitForBytesWritten();

}

QString Str2int2(char*buf, int length)
{
	char result[200];
	sprintf(result, "%x ", (int)buf[0]);

	int i;
	for (i = 1; i < length; i++)
	{
		if ((buf[i] >= 'a'&&buf[i] <= 'z') || (buf[i] >= 'A' && buf[i] <= 'Z') || (buf[i] >= '0'&&buf[i] <= '9') || (buf[i] == '@'))
			sprintf(result, "%s%c ", result, buf[i]);
		else
			sprintf(result, "%s%x ", result, (int)buf[i]);
		// if(i!=0&&i%4==0)
		//     printf(" ");
	}
	sprintf(result, "%s%c ", result, '\n');
	return QString(result);
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
	socket->waitForBytesWritten();

}

/***************************************************************************
�������ƣ�AcceptText
��    �ܣ������ı���Ϣ֡
����������ı���Ϣ֡����Ϣ��Դ�û���
�� �� ֵ�������ı�����Դ�û�
˵    ����
***************************************************************************/
std::pair<QString, QString> ClientSocket::AcceptText(char* TextFrame)
{
	QString Text, FromName;
	Text.clear();
	FromName.clear();

	uint16_t TextLength;
	memcpy(&TextLength, TextFrame + 2, 2);
	FromName = QString(QLatin1String(TextFrame + 5));
	Text = QString(QLatin1String(TextFrame + 6 + FromName.size()));

	return std::pair<QString, QString>(Text, FromName);
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

/***************************************************************************
�������ƣ�onSocketDisconnect
��    �ܣ������������������ӵ�����£������socket�Ͽ�
���������
�� �� ֵ��
˵    ����
***************************************************************************/
void ClientSocket::onSocketDisconnected()
{
	socket->abort();
}

/***************************************************************************
�������ƣ�onSocketDisconnect
��    �ܣ������������������ӵ�����£������socket�Ͽ�
���������
�� �� ֵ��
˵    ����
***************************************************************************/
void ClientSocket::onSocketReadyRead()
{
	QByteArray data = socket->readAll();

	int length = data.length();
	char* str = new char[length];
	memcpy(str, data.data(), length);

//	QMessageBox::about(NULL, "Hint", Str2int2(str, data.length()));


	//������ͬ֡
	{
		char head = str[0];

		switch ((int)head) {

			//�������Ǳ���֡�Ĵ���
		case 0x71:
		{
			char type = str[1];
			emit Signal_RegisterACK(type);

			break;
		}

		//�ı���ϢӦ��֡�Ĵ���
		case 0x72:
		{
			
			char type = str[1];
			emit Signal_SendTextACK(type);

			break;
		}

		//�ı���Ϣ֡�Ĵ���
		case 0x77:
		{

			//text,from_user
			std::pair<QString, QString> Text = AcceptText(str);//�ı���Ϣ
			emit Signal_AcceptText(Text.first, Text.second);

			break;
		}
		
		//������λ֡�Ĵ���
		case 0x73:
		{
			emit Signal_Abdication();
			break;
		}

		//��������/����֡�Ĵ���
		case 0x75:
		{
			QString name = AcceptOnOffLineName(str);//���߻������û���
			if ((int)str[1] == 0)
			{
				//����
				Signal_FriendOnline(name);
			}
			else
			{ 
				//����
				Signal_FriendOffline(name);
			}

			break;
		}

		//���ѳ�ʼ��֡
		case 0x76:
		{
			QVector<QString>namelist = AcceptInitFri(str);
			Signal_InitFriend(namelist);
			break;
		}

		default:
			QMessageBox::about(NULL, "Warning!!!", "���յ��Ƿ���Ϣ");
			break;
		}
	}


	delete str;

}
