#include "StdAfx.h"
#include ".\tableframesink.h"

CTableFrameSink::CTableFrameSink(void)
{
}

CTableFrameSink::~CTableFrameSink(void)
{
}
//管理接口
//初始化
bool __cdecl CTableFrameSink::InitTableFrameSink(IUnknownEx * pIUnknownEx)
{
	if(!__super::InitTableFrameSink(pIUnknownEx))
		return false;
	return true;
}
//复位桌子
void __cdecl CTableFrameSink::RepositTableFrameSink()
{
	__super::RepositTableFrameSink();
	return;
}

//游戏事件
//游戏开始
bool __cdecl CTableFrameSink::OnEventGameStart()
{
	return true;
}
//游戏结束
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	return true;
}
//发送场景
bool __cdecl CTableFrameSink::SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret)
{
	return true;
}
//人工智能游戏动作
bool __cdecl CTableFrameSink::OnPerformAIGameAction()
{
	return true;
}
//事件接口
//定时器事件
bool __cdecl CTableFrameSink::OnTimerMessage(WORD wTimerID, WPARAM wBindParam)
{
	return false;
}
//游戏消息处理
bool __cdecl CTableFrameSink::OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}
//框架消息处理
bool __cdecl CTableFrameSink::OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}
