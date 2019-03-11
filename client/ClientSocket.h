#pragma once


#include <QTcpSocket>
#include <QtEndian>
#include <QVector>
#include"ClientFileFrame.h"
#include <QDialog>

class ClientSocket:public QDialog
{

	Q_OBJECT

private:

public:

	ClientSocket(QWidget* parent=NULL);

	//����֡����
	enum SendFrameHeadType
	{
		SfhRegister = 0,	//����֡
		SfhText,			//�ı���Ϣ֡
		SfhChangeSecret,	//����֡
		SfhOffLine,			//����֡
		SfhFileHead,		//�ļ���Ϣ֡1(�����ļ������ļ�ͷ��Ϣ)
		SfhFileBody,		//�ļ���Ϣ֡2(�����ļ�����)
		SfhFileDownload,	//����֪ͨ֡(�ͻ�����������)
		SfhFileHeadReply,	//�ļ���Ϣ֡1Ӧ��֡
		SfhFileBodyReply	//�ı���Ϣ֡2Ӧ��֡
	};

	//����֡ͷ
	const char SendFrameHead[9] = {
		0x11,	//����֡
		0x12,	//�ı���Ϣ֡
		0x15,	//����֡
		0x16,	//����֡
		0x20,	//�ļ���Ϣ֡1(�����ļ������ļ�ͷ��Ϣ)
		0x21,	//�ļ���Ϣ֡2(�����ļ�����)
		0x22,	//����֪ͨ֡(�ͻ�����������)
		0x23,	//�ļ���Ϣ֡1Ӧ��֡
		0x24	//�ı���Ϣ֡2Ӧ��֡
	};

	//����֡����
	enum ReceiveFrameHeadType
	{
		RfhRegister = 0,	//����֡
		RfhTextReply,		//�ı�Ӧ��֡
		RfhText,			//�ı���Ϣ֡
		RfhBeKicked,		//������λ֡
		RfhOnOffLine,		//��/����֡
		RfhInit,			//���ѳ�ʼ��֡
		RfhFileHeadReply,	//�ļ���Ϣ֡1Ӧ��֡
		RfhFileBodyReply,	//�ı���Ϣ֡2Ӧ��֡
		RfhFileInform,		//�����ļ���Ϣ֡
		RfhFileHead,		//�ļ���Ϣ֡1(�����ļ������ļ�ͷ��Ϣ)
		RfhFileBody			//�ļ���Ϣ֡2(�����ļ�����)
	};

	//����֡ͷ
	const unsigned char ReceiveFrameHead[11] = {
		0x71,	//����֡
		0x72,	//�ı�Ӧ��֡
		0x77,   //�ı���Ϣ֡
		0x73,	//������λ֡
		0x75,	//��/����֡
		0x76,	//���ѳ�ʼ��֡
		0x80,	//�ļ���Ϣ֡1Ӧ��֡
		0x81,	//�ı���Ϣ֡2Ӧ��֡
		0x82,	//�����ļ���Ϣ֡
		0x83,	//�ļ���Ϣ֡1(�����ļ������ļ�ͷ��Ϣ)
		0x84	//�ļ���Ϣ֡2(�����ļ�����)
	};

	//C�յ�����S���ı���Ϣ֡���۲��־λ[8-15]
	enum SglOrAll
	{
		Single = 0x00,
		All = 0x01
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


	void SendFileHead(const QString AcceptUserName, uint32_t FileSize,QString FileName)const;
	void SendFileBody(const QString AcceptUserName, uint32_t OrderNum, char* FileCont, uint32_t ContLength)const;


	void SendFileDownload(const QString SendUserName, QString FileName)const;
	void SendFileHeadReply(RplyAllowAccept allowupload)const;
	void SendFileBodyReply(uint32_t FrameOrder)const;


	//�ı�֡����
	std::pair<QString, QString> AcceptText(char* TextFrame);
	QString AcceptOnOffLineName(char* OnOffLineFrame);
	QVector<QString> AcceptInitFri(QByteArray InitFriFrame);
	RplyAllowUpload AcceptFileHeadReply(FrameS2CFileHeadRply*Frame);
	uint32_t AcceptFileBodyReply(FrameS2CFileBodyRply*Frame);
	void AcceptFileInform(FrameS2CFileInform*Frame, uint8_t&AccType, QString&SendUserName, uint32_t&FileSize, uint32_t&PackNum, QString&FileName);
	void AcceptFileHead(FrameS2CFileHead*Frame, QString&SendUserName, uint32_t&FileSize, uint32_t&PackNum, QString&FileName);
	void AcceptFileBody(FrameS2CFileBody*Frame, QString&SendUserName,uint32_t&PackOrder,QString&FileCont, uint32_t ContLength);

	friend class MainWidget;

	QString _destu;
	QString _file;
	

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

	//�ļ���ʼ��֡
	void Signal_InitFile();

signals:
	void Signal_DownLoadInfo(QString _username, QString _file);
};


