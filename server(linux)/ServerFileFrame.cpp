#include"ServerFileOpt.h"
/***************************************************************************
函数名称：CrtFrameFileHeadRply
功    能：生成文件信息帧1应答帧
输入参数：文件信息帧1应答帧引用,应答类型
返 回 值：帧长度
说    明：函数外部定义一个帧的指针作为参数，在函数内申请空间
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
函数名称：CrtFrameFileBodyReply
功    能：生成文件信息帧2应答帧
输入参数：文件信息帧2应答帧引用,序号
返 回 值：帧长度
说    明：函数外部定义一个帧的指针作为参数，在函数内申请空间
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
函数名称：CrtFrameFileInform
功    能：生成传输文件消息帧
输入参数：帧引用,_SendUserName:用户名,_FileSize:文件大小,_PackNum:包的数量,_FileName:文件名,ChkAll:群发判断
返 回 值：帧长度
说    明：函数外部定义一个帧的指针作为参数，在函数内申请空间
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
函数名称：CrtFrameFileHead
功    能：生成文件信息帧1
输入参数：帧引用,_SendUserName:用户名,_FileSize:文件大小,_PackNum:包的数量,_FileName:文件名
返 回 值：帧长度
说    明：函数外部定义一个帧的指针作为参数，在函数内申请空间
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
函数名称：CrtFrameFileBody
功    能：生成文件信息帧2
输入参数：帧引用,_SendUserName:发送者用户名,_OrderNum:文件包序号,FileCont:文件内容文件,Contlength:内容部分长度
返 回 值：帧长度
说    明：函数外部定义一个帧的指针作为参数，在函数内申请空间
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
函数名称：AnlsFrameFileHead
功    能：解析文件信息帧1
输入参数：帧的指针,接受者用户名,文件大小,文件包数量,文件名
返 回 值：
说    明：参数1将帧数组的数组名以强制类型转换的方式输入，参数2,5将用户名数组和文件片段数组的数组名输入，参数3,4以相应变量的引用方式输入
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
函数名称：AnlsFrameFileBody
功    能：解析文件信息帧2
输入参数：帧的指针,接收文件用户姓名,文件包序号(引用),文件内容,数据长度(引用)
返 回 值：
说    明：参数1将帧数组的数组名以强制类型转换的方式输入，参数2和参数4将用户名数组和文件片段数组的数组名输入，参数3,5以相应变量的引用方式输入
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
函数名称：AnlsFrameFileHeadReply
功    能：解析文件信息帧1应答帧
输入参数：帧的指针
返 回 值：应答类型(AcceptOk(0x00):允许接收,AcceptRefuse(0x01):拒绝接收)
说    明：
***************************************************************************/
RplyAllowAccept AnlsFrameFileHeadReply(FrameC2SFileHeadReply*p_Frame)
{
	return (RplyAllowAccept)p_Frame->FH.ack;
}
/***************************************************************************
函数名称：AnlsFrameFileBodyReply
功    能：解析文本信息帧2应答帧
输入参数：帧的指针
返 回 值：文件包序号
说    明：
***************************************************************************/
uint32_t AnlsFrameFileBodyReply(FrameC2SFileBodyReply*p_Frame)
{
	return p_Frame->FrameOrder;
}