#include "StdAfx.h"
#include ".\tableframesink.h"

CTableFrameSink::CTableFrameSink(void)
{
}

CTableFrameSink::~CTableFrameSink(void)
{
}
//����ӿ�
//��ʼ��
bool __cdecl CTableFrameSink::InitTableFrameSink(IUnknownEx * pIUnknownEx)
{
	if(!__super::InitTableFrameSink(pIUnknownEx))
		return false;
	return true;
}
//��λ����
void __cdecl CTableFrameSink::RepositTableFrameSink()
{
	__super::RepositTableFrameSink();
	return;
}

//��Ϸ�¼�
//��Ϸ��ʼ
bool __cdecl CTableFrameSink::OnEventGameStart()
{
	return true;
}
//��Ϸ����
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	return true;
}
//���ͳ���
bool __cdecl CTableFrameSink::SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret)
{
	return true;
}
//�˹�������Ϸ����
bool __cdecl CTableFrameSink::OnPerformAIGameAction()
{
	return true;
}
//�¼��ӿ�
//��ʱ���¼�
bool __cdecl CTableFrameSink::OnTimerMessage(WORD wTimerID, WPARAM wBindParam)
{
	return false;
}
//��Ϸ��Ϣ����
bool __cdecl CTableFrameSink::OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}
//�����Ϣ����
bool __cdecl CTableFrameSink::OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}
