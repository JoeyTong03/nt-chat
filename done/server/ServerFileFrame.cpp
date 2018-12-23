#include"ServerFileFrame.h"
/***************************************************************************
�������ƣ�CrtFrameFileHeadRply
��    �ܣ������ļ���Ϣ֡1Ӧ��֡
����������ļ���Ϣ֡1Ӧ��֡����,Ӧ������
�� �� ֵ��֡����
˵    ���������ⲿ����һ��֡��ָ����Ϊ�������ں���������ռ�
***************************************************************************/
int CrtFrameFileHeadRply(FrameS2CFileHeadRply*&p_Frame, RplyAllowUpload RplyType)
{
	p_Frame = new FrameS2CFileHeadRply;
	p_Frame->FH.type = RfhFileHeadReply;
	p_Frame->FH.ack = RplyType;
	p_Frame->FH.length = (uint16_t)sizeof(FrameS2CFileHeadRply);
	return (int)p_Frame->FH.length;
}
/***************************************************************************
�������ƣ�CrtFrameFileBodyReply
��    �ܣ������ļ���Ϣ֡2Ӧ��֡
����������ļ���Ϣ֡2Ӧ��֡����,���
�� �� ֵ��֡����
˵    ���������ⲿ����һ��֡��ָ����Ϊ�������ں���������ռ�
***************************************************************************/
int CrtFrameFileBodyReply(FrameS2CFileBodyRply*&p_Frame,uint32_t FrameOrder)
{
	p_Frame = new FrameS2CFileBodyRply;
	p_Frame->FH.type = RfhFileBodyReply;
	p_Frame->FH.ack = ACKNULL;
	p_Frame->FH.length = (uint16_t)sizeof(FrameS2CFileBodyRply);
	p_Frame->FrameOrder = FrameOrder;
	return (int)p_Frame->FH.length;
}
/***************************************************************************
�������ƣ�CrtFrameFileInform
��    �ܣ����ɴ����ļ���Ϣ֡
���������֡����,_SendUserName:�û���,_FileSize:�ļ���С,_PackNum:��������,_FileName:�ļ���,ChkAll:Ⱥ���ж�
�� �� ֵ��֡����
˵    ���������ⲿ����һ��֡��ָ����Ϊ�������ں���������ռ�
***************************************************************************/
int CrtFrameFileInform(FrameS2CFileInform*&p_Frame, char* _SendUserName, uint32_t _FileSize, char* _FileName, SglOrAll ChkAll)
{
	p_Frame = new FrameS2CFileInform;
	p_Frame->FH.type = RfhFileInform;
	p_Frame->FH.ack = ChkAll;
	p_Frame->FH.length = (uint16_t)sizeof(FrameS2CFileInform);
	memcpy(p_Frame->SendUserName, _SendUserName, UserNameLength);
	p_Frame->FileSize = _FileSize;
	p_Frame->PackNum = _FileSize / FrameFileLength;
	if (_FileSize % FrameFileLength != 0)
		p_Frame->PackNum++;
	memcpy(p_Frame->FileName, _FileName, FileNameLength);
	return (int)p_Frame->FH.length;
}
/***************************************************************************
�������ƣ�CrtFrameFileHead
��    �ܣ������ļ���Ϣ֡1
���������֡����,_SendUserName:�û���,_FileSize:�ļ���С,_PackNum:��������,_FileName:�ļ���
�� �� ֵ��֡����
˵    ���������ⲿ����һ��֡��ָ����Ϊ�������ں���������ռ�
***************************************************************************/
int CrtFrameFileHead(FrameS2CFileHead*&p_Frame, char* _SendUserName, uint32_t _FileSize, char* _FileName)
{
	p_Frame = new FrameS2CFileHead;
	p_Frame->FH.type = RfhFileHead;
	p_Frame->FH.ack = ACKNULL;
	p_Frame->FH.length = (uint16_t)sizeof(FrameS2CFileHead);
	memcpy(p_Frame->SendUserName, _SendUserName, UserNameLength);
	p_Frame->FileSize = _FileSize;
	p_Frame->PackNum = _FileSize / FrameFileLength;
	if (_FileSize % FrameFileLength != 0)
		p_Frame->PackNum++;
	memcpy(p_Frame->FileName, _FileName, FileNameLength);
	return (int)p_Frame->FH.length;
}
/***************************************************************************
�������ƣ�CrtFrameFileBody
��    �ܣ������ļ���Ϣ֡2
���������֡����,_SendUserName:�������û���,_OrderNum:�ļ������,FileCont:�ļ������ļ�,Contlength:���ݲ��ֳ���
�� �� ֵ��֡����
˵    ���������ⲿ����һ��֡��ָ����Ϊ�������ں���������ռ�
***************************************************************************/
int CrtFrameFileBody(FrameS2CFileBody*&p_Frame, char* _SendUserName, uint32_t _OrderNum, char* FileCont,uint16_t Contlength)
{
	p_Frame = new FrameS2CFileBody;
	p_Frame->FH.type = RfhFileBody;
	p_Frame->FH.ack = ACKNULL;
	p_Frame->FH.length = (uint16_t)(sizeof(FrameHead) + UserNameLength + sizeof(uint32_t) + Contlength);
	memcpy(p_Frame->SendUserName, _SendUserName, UserNameLength);
	p_Frame->OrderNum = _OrderNum;
	memcpy(p_Frame->FileCont, FileCont, Contlength);
	return (int)p_Frame->FH.length;
}
/***************************************************************************
�������ƣ�AnlsFrameFileHead
��    �ܣ������ļ���Ϣ֡1
���������֡��ָ��,�������û���,�ļ���С,�ļ�������,�ļ���
�� �� ֵ��
˵    ��������1��֡�������������ǿ������ת���ķ�ʽ���룬����2,5���û���������ļ�Ƭ����������������룬����3,4����Ӧ���������÷�ʽ����
***************************************************************************/
int AnlsFrameFileHead(FrameC2SFileHead*p_Frame,char*_AcceptUserName,uint32_t&_FileSize,uint32_t&_PackNum,char* _FileName)
{
	memcpy(_AcceptUserName, p_Frame->AcceptUserName, UserNameLength);
	_FileSize = p_Frame->FileSize;
	_PackNum = p_Frame->PackNum;
	memcpy(_FileName, p_Frame->FileName, FileNameLength);
	return 1;
}
/***************************************************************************
�������ƣ�AnlsFrameFileBody
��    �ܣ������ļ���Ϣ֡2
���������֡��ָ��,�����ļ��û�����,�ļ������(����),�ļ�����,���ݳ���(����)
�� �� ֵ��
˵    ��������1��֡�������������ǿ������ת���ķ�ʽ���룬����2�Ͳ���4���û���������ļ�Ƭ����������������룬����3,5����Ӧ���������÷�ʽ����
***************************************************************************/
int AnlsFrameFileBody(FrameC2SFileBody*p_Frame, char*_AcceptUserName, uint32_t&_OrderNum, char* _FileCont, uint16_t&ContLength)
{
	memcpy(_AcceptUserName, p_Frame->AcceptUserName, UserNameLength);
	_OrderNum = p_Frame->OrderNum;
	ContLength = p_Frame->FH.length - sizeof(FrameHead) - UserNameLength - sizeof(p_Frame->OrderNum);
	memcpy(_FileCont, p_Frame->FileCont, ContLength);
	return 1;
}
/***************************************************************************
�������ƣ�AnlsFrameFileDownload
��    �ܣ���������֪ͨ֡
���������֡��ָ��,�����ļ��û���,�ļ���
�� �� ֵ��
˵    ��������1��֡�������������ǿ������ת���ķ�ʽ���룬����2�Ͳ���4���û���������ļ�Ƭ����������������룬����3,5����Ӧ���������÷�ʽ����
***************************************************************************/
int AnlsFrameFileDownload(FrameC2SFileDownload*p_Frame,char*_SendUserName,char*_FileName)
{
	memcpy(_SendUserName,p_Frame->SendUserName,UserNameLength);
	memcpy(_FileName,p_Frame->FileName,FileNameLength);
	return 1;
}
/***************************************************************************
�������ƣ�AnlsFrameFileHeadReply
��    �ܣ������ļ���Ϣ֡1Ӧ��֡
���������֡��ָ��
�� �� ֵ��Ӧ������(AcceptOk(0x00):�������,AcceptRefuse(0x01):�ܾ�����)
˵    ����
***************************************************************************/
RplyAllowAccept AnlsFrameFileHeadReply(FrameC2SFileHeadReply*p_Frame)
{
	return (RplyAllowAccept)p_Frame->FH.ack;
}
/***************************************************************************
�������ƣ�AnlsFrameFileBodyReply
��    �ܣ������ı���Ϣ֡2Ӧ��֡
���������֡��ָ��
�� �� ֵ���ļ������
˵    ����
***************************************************************************/
uint32_t AnlsFrameFileBodyReply(FrameC2SFileBodyReply*p_Frame)
{
	return p_Frame->FrameOrder;
}

