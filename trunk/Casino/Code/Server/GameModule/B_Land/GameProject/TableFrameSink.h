#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//����ص���
class CTableFrameSink;
class CCalculateSink:public ICalculateSink
{
public:
	CCalculateSink();
	~CCalculateSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual bool __cdecl Release() { if (IsValid()) delete this; return true; }
	//�Ƿ���Ч
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CCalculateSink))?true:false; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//ICalculateSink�ӿ�
public:
	//�����ɲ�
	virtual bool __cdecl CalculateResult(void* pCalculateContext,
		WORD wChairID,
		ICalculateItem* pItem,
		DECIMAL* pPartedBetScore,
		DECIMAL* pPartedWinScore,
		DECIMAL* pPartedTaxScore,
		WORD	 wBetRegionCount);
	//����ϴ��
	virtual bool __cdecl RectifyValidBetScore(void* pCalculateContext,
		ICalculateItem* pItem,
		DECIMAL* pPartedBetScore,
		WORD	 wBetRegionCount,
		DECIMAL* pValidBetScore);
	//
public:
	//��ʼ��
	bool __cdecl InitCalculateSink(ITableFrame	*pITableFrame,
		CTableFrameSink *pTableFrameSink);
	//��ʼ������������
	bool __cdecl InitCalculateContext(CMD_S_GameEnd* pGameEnd);
	//��ȡ����������
	void* __cdecl GetCalculateContext();
protected:
	ITableFrame						* m_pITableFrame;					//��ܽӿ�
	CTableFrameSink					* m_pTableFrameSink;				//��Ϸ������
	ICalculateFrame					* m_pICalculateFrame;				//����ӿ�
	const tagGameServiceAttrib		* m_pGameServiceAttrib;				//���Բ���
	const tagGameServiceOption		* m_pGameServiceOption;				//���ò���
	CMD_S_GameEnd					* m_pGameEnd;						//����������
	DECIMAL							  m_decAfterTax;					//��ˮ����
};
//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class CTableFrameSink : public ITableFrameSink
{
	//��Ϸ����
protected:
	WORD							m_wBombTime;						//ը������
	BYTE							m_bLandScore;						//��������
	WORD							m_wFirstUser;						//�׽��û�
	WORD							m_wBankerUser;						//ׯ���û�
	WORD							m_wCurrentUser;						//��ǰ���
	BYTE							m_bScoreInfo[3];					//�з���Ϣ

	//Run��Ϣ
protected:
	WORD							m_wTurnWiner;						//ʤ�����
	BYTE							m_bTurnCardCount;					//������Ŀ
	BYTE							m_bTurnCardData[20];				//�����б�

	//�˿���Ϣ
protected:
	BYTE							m_bBackCard[3];						//��Ϸ����
	BYTE							m_bCardCount[3];					//�˿���Ŀ
	BYTE							m_bOutCardCount[3];					//���ƴ���
	BYTE							m_bHandCardData[3][20];				//�����˿�

	//�������
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	const tagGameServiceOption		* m_pGameServiceOption;				//���ò���
	ITableFrame						* m_pITableFrame;					//��ܽӿ�
	ICalculateFrame					* m_pICalculateFrame;				//����ӿ�
	const tagGameServiceAttrib		* m_pGameServiceAttrib;				//���Բ���
	CCalculateSink					m_CalculateSink;					//����ص�

	//���Ա���
protected:
	static const WORD				m_wPlayerCount;						//��Ϸ����
	static const enStartMode		m_GameStartMode;					//��ʼģʽ

	//��������
public:
	//���캯��
	CTableFrameSink();
	//��������
	virtual ~CTableFrameSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual bool __cdecl Release() { if (IsValid()) delete this; return true; }
	//�Ƿ���Ч
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CTableFrameSink))?true:false; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//��ʼ��
	virtual bool __cdecl InitTableFrameSink(IUnknownEx * pIUnknownEx);
	//��λ����
	virtual void __cdecl RepositTableFrameSink();

	//��Ϣ�ӿ�
public:
	//��ʼģʽ
	virtual enStartMode __cdecl GetGameStartMode();
	//��Ϸ״̬
	virtual bool __cdecl IsUserPlaying(WORD wChairID);

	//��Ϸ�¼�
public:
	//��Ϸ��ʼ
	virtual bool __cdecl OnEventGameStart();
	//��Ϸ����
	virtual bool __cdecl OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//���ͳ���
	virtual bool __cdecl SendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);
	//�˹�������Ϸ����
	virtual bool __cdecl OnPerformAIGameAction();

	//�¼��ӿ�
public:
	//��ʱ���¼�
	virtual bool __cdecl OnTimerMessage(WORD wTimerID, WPARAM wBindParam);
	//��Ϸ��Ϣ����
	virtual bool __cdecl OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����Ϣ����
	virtual bool __cdecl OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//��Ϸ�¼�
protected:
	//�з��¼�
	bool OnUserLandScore(WORD wChairID, BYTE bLandScore);
	//�û�����
	bool OnUserOutCard(WORD wChairID, BYTE bCardData[], BYTE bCardCount);
	//�û�����
	bool OnUserPassCard(WORD wChairID);

	//�˹����ܺ���
protected:
	//�Զ�����
	bool AI_AutomatismOutCard(WORD wChairID, IServerUserItem * pIServerUserItem);
	//�Զ�����
	void AI_AutoOutCard(WORD wChairID, IServerUserItem * pIServerUserItem,BYTE bWhichOnsKindCard);
	//��������
	bool AI_PassCard(WORD wChairID, IServerUserItem * pIServerUserItem);
};

//////////////////////////////////////////////////////////////////////////

#endif