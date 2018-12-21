#pragma once


#include <QTcpSocket>
#include <QtEndian>
#include <QVector>


class ClientSocket
{
private:

public:
	//����֡����
	enum SendFrameHeadType
	{
		SfhRegister = 0,	//����֡
		SfhText,			//�ı���Ϣ֡
		SfhChangeSecret,	//����֡
		SfhOffLine			//����֡
	};

	//����֡ͷ
	const char SendFrameHead[4] = {
		0x11,	//����֡
		0x12,	//�ı���Ϣ֡
		0x15,	//����֡
		0x16	//����֡
	};

	//����֡����
	enum ReceiveFrameHeadType
	{
		RfhRegister = 0,	//����֡
		RfhTextReply,		//�ı�Ӧ��֡
		RfhText,			//�ı���Ϣ֡
		RfhBeKicked,		//������λ֡
		RfhOnOffLine,		//��/����֡
		RfhInit				//���ѳ�ʼ��֡
	};

	//����֡ͷ
	const char ReceiveFrameHead[6] = {
		0x71,	//����֡
		0x72,	//�ı�Ӧ��֡
		0x77,   //�ı���Ϣ֡
		0x73,	//������λ֡
		0x75,	//��/����֡
		0x76	//���ѳ�ʼ��֡
	};

	//��������
	const int USERNAME_MAXSIZE = 16;	//�û����Ϊ16�ֽ�
	const int SECRET_MAXSIZE = 12;		//�����Ϊ12�ֽ�

	//�����������
	const QString ip = "192.168.80.230";
	const int port = 4000;

	//QTcpSocket
	QTcpSocket* socket;

public:
	ClientSocket();
	~ClientSocket();

	//Socket���Ӻ���
	bool connectServer();

	//Socket�Ͽ�����
	void closeConnect();

	//����֡����
	void SendRegisterFrame(const QString _username, const QString _secret)const;
	void SendTextFrame(const QString _str, const QString _username)const;
	void SendChangeSecretFrame(const QString _secret)const;
	void SendOffLineFrame()const;

	//����֡ͷ����
	void AnlsFrame(char*buf);

	//�ı�֡����
	QString AcceptText(char* TextFrame, QString&FromName);
	QString AcceptOnOffLineName(char* OnOffLineFrame);
	QVector<QString> AcceptInitFri(char* InitFriFrame);

	friend class MainWidget;

	

private slots:
	void WaitForMessage();

};

void Str2int2(char*buf, int length);