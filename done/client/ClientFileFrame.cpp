
#include"ClientSocket.h"
#include"ClientFileFrame.h"

/***************************************************************************
函数名称：SendFileHead
功    能：发送文件信息帧1
输入参数：SendUserName:用户名,_FileSize:文件大小,_PackNum:包的数量,_FileName:文件名
返 回 值：
说    明：
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
函数名称：SendFileBody
功    能：生成文件信息帧2
输入参数：SendUserName:发送者用户名,_OrderNum:文件包序号,FileCont:文件内容文件,Contlength:内容部分长度
返 回 值：
说    明：
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
函数名称：SendFileDownload
功    能：下载请求帧
输入参数：SendUserName发送者用户名FileName文件名
返 回 值：
说    明：
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
函数名称：SendFileHeadReply
功    能：生成文件信息帧1应答帧
输入参数：应答类型
返 回 值：
说    明：
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
函数名称：SendFileBodyReply
功    能：生成文件信息帧2应答帧
输入参数：序号
返 回 值：
说    明：
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
函数名称：AcceptFileHeadReply
功    能：解析文件信息帧1应答帧
输入参数：帧的指针
返 回 值：应答类型(AcceptOk(0x00):允许接收,AcceptRefuse(0x01):拒绝接收)
说    明：
***************************************************************************/
RplyAllowUpload ClientSocket::AcceptFileHeadReply(FrameS2CFileHeadRply*Frame)
{
	return (RplyAllowUpload)Frame->FH.ack;
}
/***************************************************************************
函数名称：AcceptFileBodyReply
功    能：解析文本信息帧2应答帧
输入参数：帧的指针
返 回 值：文件包序号
说    明：
***************************************************************************/
uint32_t ClientSocket::AcceptFileBodyReply(FrameS2CFileBodyRply*Frame)
{
	return Frame->FrameOrder;
}
/***************************************************************************
函数名称：AcceptFileInform
功    能：解析文件传输消息帧
输入参数：帧的指针,群发判断,接受者用户名,文件大小,文件包数量,文件名
返 回 值：
说    明：参数1将帧数组的数组名以强制类型转换的方式输入，参数2,5将用户名数组和文件片段数组的数组名输入，参数3,4以相应变量的引用方式输入
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
函数名称：AcceptFileHead
功    能：解析文件信息帧1
输入参数：帧的指针,接受者用户名,文件大小,文件包数量,文件名
返 回 值：
说    明：参数1将帧数组的数组名以强制类型转换的方式输入，参数2,5将用户名数组和文件片段数组的数组名输入，参数3,4以相应变量的引用方式输入
***************************************************************************/
void ClientSocket::AcceptFileHead(FrameS2CFileHead*Frame, QString&SendUserName, uint32_t&FileSize, uint32_t&PackNum, QString&FileName)
{
	SendUserName = QString(QLatin1String(Frame->SendUserName));
	FileSize = Frame->FileSize;
	PackNum = Frame->PackNum;
	FileName = QString(QLatin1String(Frame->FileName));
}
/***************************************************************************
函数名称：AcceptFileBody
功    能：解析文件信息帧2
输入参数：帧的指针,接收文件用户姓名,文件包序号(引用),文件内容,数据长度(引用)
返 回 值：
说    明：参数1将帧数组的数组名以强制类型转换的方式输入，参数2和参数4将用户名数组和文件片段数组的数组名输入，参数3,5以相应变量的引用方式输入
***************************************************************************/
void ClientSocket::AcceptFileBody(FrameS2CFileBody*Frame, QString&SendUserName, uint32_t&PackOrder, QString&FileCont, uint32_t ContLength)
{
	SendUserName = QString(QLatin1String(Frame->SendUserName));
	PackOrder = Frame->OrderNum;
	ContLength = Frame->FH.length - sizeof(FrameHead) - UserNameLength - sizeof(Frame->OrderNum);
	FileCont= QString(QLatin1String(Frame->FileCont));
}