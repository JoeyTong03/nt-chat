
#include"ClientSocket.h"
#include"ClientFileFrame.h"

/***************************************************************************
�������ƣ�SendFileHead
��    �ܣ������ļ���Ϣ֡1
���������SendUserName:�û���,_FileSize:�ļ���С,_PackNum:��������,_FileName:�ļ���
�� �� ֵ��
˵    ����
***************************************************************************/
void ClientSocket::SendFileHead(const QString AcceptUserName, uint32_t FileSize, QString FileName)const
{
	FrameC2SFileHead Frame;
	Frame.FH.type = SendFrameHead[SfhFileHead];
	Frame.FH.ack = ACKNULL;
	Frame.FH.length = sizeof(FrameC2SFileHead);
	memcpy(Frame.AcceptUserName, AcceptUserName.toStdString().data(), UserNameLength);
	Frame.FileSize = FileSize;
	Frame.PackNum = FileSize / FrameFileLength;
	if (FileSize%FrameFileLength != 0)
		Frame.PackNum++;
	memcpy(Frame.FileName, FileName.toStdString().data(), FileNameLength);
	socket->write((char*)&Frame, Frame.FH.length);
	socket->waitForBytesWritten();
}
/***************************************************************************
�������ƣ�SendFileBody
��    �ܣ������ļ���Ϣ֡2
���������SendUserName:�������û���,_OrderNum:�ļ������,FileCont:�ļ������ļ�,Contlength:���ݲ��ֳ���
�� �� ֵ��
˵    ����
***************************************************************************/
void ClientSocket::SendFileBody(const QString AcceptUserName, uint32_t OrderNum,char* FileCont, uint32_t ContLength)const
{
	FrameC2SFileBody Frame;
	Frame.FH.type = SendFrameHead[SfhFileBody];
	Frame.FH.ack = ACKNULL;
	Frame.FH.length = (uint16_t)(sizeof(FrameHead) + UserNameLength + sizeof(uint32_t) + ContLength);
	memcpy(Frame.AcceptUserName, AcceptUserName.toStdString().data(), UserNameLength);
	Frame.OrderNum = OrderNum;
	memcpy(Frame.FileCont, FileCont, ContLength);
	socket->write((char*)&Frame, Frame.FH.length);
	socket->waitForBytesWritten();
}
/***************************************************************************
�������ƣ�SendFileDownload
��    �ܣ���������֡
���������SendUserName�������û���FileName�ļ���
�� �� ֵ��
˵    ����
***************************************************************************/
void ClientSocket::SendFileDownload(const QString SendUserName, QString FileName)const
{
	FrameC2SFileDownload Frame;
	Frame.FH.type = SendFrameHead[SfhFileDownload];
	Frame.FH.ack = ACKNULL;
	Frame.FH.length = sizeof(FrameC2SFileDownload);
	memcpy(Frame.SendUserName, SendUserName.toStdString().data(), UserNameLength);
	memcpy(Frame.FileName, FileName.toStdString().data(), FileNameLength);
	socket->write((char*)&Frame, Frame.FH.length);
	socket->waitForBytesWritten();
}
/***************************************************************************
�������ƣ�SendFileHeadReply
��    �ܣ������ļ���Ϣ֡1Ӧ��֡
���������Ӧ������
�� �� ֵ��
˵    ����
***************************************************************************/
void ClientSocket::SendFileHeadReply(RplyAllowAccept allowaccept)const
{
	FrameC2SFileHeadReply Frame;
	Frame.FH.type = SendFrameHead[SfhFileHeadReply];
	Frame.FH.ack = allowaccept;
	Frame.FH.length = sizeof(FrameC2SFileHeadReply);
	socket->write((char*)&Frame, Frame.FH.length);
	socket->waitForBytesWritten();
}
/***************************************************************************
�������ƣ�SendFileBodyReply
��    �ܣ������ļ���Ϣ֡2Ӧ��֡
������������
�� �� ֵ��
˵    ����
***************************************************************************/
void ClientSocket::SendFileBodyReply(uint32_t FrameOrder)const
{
	FrameC2SFileBodyReply Frame;
	Frame.FH.type = SendFrameHead[SfhFileBodyReply];
	Frame.FH.ack = ACKNULL;
	Frame.FH.length = sizeof(FrameC2SFileBodyReply);
	Frame.FrameOrder = FrameOrder;
	socket->write((char*)&Frame, Frame.FH.length);
	socket->waitForBytesWritten();
}
/***************************************************************************
�������ƣ�AcceptFileHeadReply
��    �ܣ������ļ���Ϣ֡1Ӧ��֡
���������֡��ָ��
�� �� ֵ��Ӧ������(AcceptOk(0x00):�������,AcceptRefuse(0x01):�ܾ�����)
˵    ����
***************************************************************************/
RplyAllowUpload ClientSocket::AcceptFileHeadReply(FrameS2CFileHeadRply*Frame)
{
	return (RplyAllowUpload)Frame->FH.ack;
}
/***************************************************************************
�������ƣ�AcceptFileBodyReply
��    �ܣ������ı���Ϣ֡2Ӧ��֡
���������֡��ָ��
�� �� ֵ���ļ������
˵    ����
***************************************************************************/
uint32_t ClientSocket::AcceptFileBodyReply(FrameS2CFileBodyRply*Frame)
{
	return Frame->FrameOrder;
}
/***************************************************************************
�������ƣ�AcceptFileInform
��    �ܣ������ļ�������Ϣ֡
���������֡��ָ��,Ⱥ���ж�,�������û���,�ļ���С,�ļ�������,�ļ���
�� �� ֵ��
˵    ��������1��֡�������������ǿ������ת���ķ�ʽ���룬����2,5���û���������ļ�Ƭ����������������룬����3,4����Ӧ���������÷�ʽ����
***************************************************************************/
void ClientSocket::AcceptFileInform(FrameS2CFileInform*Frame, uint8_t&AccType,QString&SendUserName, uint32_t&FileSize, uint32_t&PackNum, QString&FileName)
{
	AccType = Frame->FH.ack;
	SendUserName = QString(QLatin1String(Frame->SendUserName));
	FileSize = Frame->FileSize;
	PackNum = Frame->PackNum;
	FileName = QString(QLatin1String(Frame->FileName));
}
/***************************************************************************
�������ƣ�AcceptFileHead
��    �ܣ������ļ���Ϣ֡1
���������֡��ָ��,�������û���,�ļ���С,�ļ�������,�ļ���
�� �� ֵ��
˵    ��������1��֡�������������ǿ������ת���ķ�ʽ���룬����2,5���û���������ļ�Ƭ����������������룬����3,4����Ӧ���������÷�ʽ����
***************************************************************************/
void ClientSocket::AcceptFileHead(FrameS2CFileHead*Frame, QString&SendUserName, uint32_t&FileSize, uint32_t&PackNum, QString&FileName)
{
	SendUserName = QString(QLatin1String(Frame->SendUserName));
	FileSize = Frame->FileSize;
	PackNum = Frame->PackNum;
	FileName = QString(QLatin1String(Frame->FileName));
}
/***************************************************************************
�������ƣ�AcceptFileBody
��    �ܣ������ļ���Ϣ֡2
���������֡��ָ��,�����ļ��û�����,�ļ������(����),�ļ�����,���ݳ���(����)
�� �� ֵ��
˵    ��������1��֡�������������ǿ������ת���ķ�ʽ���룬����2�Ͳ���4���û���������ļ�Ƭ����������������룬����3,5����Ӧ���������÷�ʽ����
***************************************************************************/
void ClientSocket::AcceptFileBody(FrameS2CFileBody*Frame, QString&SendUserName, uint32_t&PackOrder, QString&FileCont, uint32_t ContLength)
{
	SendUserName = QString(QLatin1String(Frame->SendUserName));
	PackOrder = Frame->OrderNum;
	ContLength = Frame->FH.length - sizeof(FrameHead) - UserNameLength - sizeof(Frame->OrderNum);
	FileCont= QString(QLatin1String(Frame->FileCont));
}