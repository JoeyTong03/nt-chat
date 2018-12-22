#pragma once

#include <QWidget>
#include <QTcpSocket>

class ClientSocket : public QWidget
{
	Q_OBJECT

public:
	ClientSocket(QWidget *parent = Q_NULLPTR);
	~ClientSocket();

private:

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

	//Socket���Ӻ���
	bool connectServer();

	//Socket�Ͽ�����
	void closeConnect();

	//����֡����
	void SendRegisterFrame(const QString _username, const QString _secret)const;
	void SendTextFrame(const QString _str, const QString _username)const;
	void SendChangeSecretFrame(const QString _secret)const;
	void SendOffLineFrame()const;

	//�ı�֡����
	std::pair<QString, QString> AcceptText(char* TextFrame);
	QString AcceptOnOffLineName(char* OnOffLineFrame);
	QVector<QString> AcceptInitFri(char* InitFriFrame);
		
private slots:

	void onSocketDisconnected();	//�źŲۺ�����socket����
	void onSocketReadyRead();		//�źŲۺ�����socket�����ݿɶ�

signals:

	//����Ӧ��֡�����ź� 1-�ɹ� 2-������� 3-�û��������� 4-��Ҫ���� default:��Ϣ����
	void Signal_RegisterACK(char _type);

	//�ı���ϢӦ��֡�����ź� 0x00 - ���ͳɹ� 0xFF - ��Ϣ�﷨���� 0xFE - �������Ʋ����� 0xFD - ����δ����
	void Signal_SendTextACK(char _type);

	//�ı���Ϣ֡�ź�
	void Signal_AcceptText(QString _text, QString _from);

	//������λ֡�ź�
	void Signal_Abdication();

	//��������֡�ź�
	void Signal_FriendOnline(QString _user);

	//��������֡�ź�
	void Signal_FriendOffline(QString _user);

	//���ѳ�ʼ���ź�
	void Signal_InitFriend(QVector<QString> _userlist);
};

