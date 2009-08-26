#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
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
class CTableFrameSink : public ITableFrameSink, public ITableFrameEvent
{
	//�߼�����
protected:
	BYTE							m_cbPackCount;						//�Ƹ���Ŀ
	BYTE							m_cbMainColor;						//���ƻ�ɫ
	BYTE							m_cbMainValue;						//������ֵ

	//������Ϣ
protected:
	BYTE							m_cbValueOrder[2];					//��ֵ�ȼ�

	//��Ϸ����
protected:
	WORD							m_wBankerUser;						//ׯ���û�
	WORD							m_wCurrentUser;						//��ǰ���

	//������Ϣ
protected:
	BYTE							m_cbCallCard;						//�����˿�
	BYTE							m_cbCallCount;						//������Ŀ
	WORD							m_wCallCardUser;					//�����û�

	//״̬����
protected:
	bool							m_bCallCard[4];						//���Ʊ�־
	BYTE							m_cbScoreCardCount;					//�˿���Ŀ
	BYTE							m_cbScoreCardData[12*2];			//�÷��˿�

	//���Ʊ���
protected:
	WORD							m_wTurnWinner;						//ʤ�����
	WORD							m_wFirstOutUser;					//�����û�
	BYTE							m_cbOutCardCount[4];				//������Ŀ
	BYTE							m_cbOutCardData[4][MAX_COUNT];		//�����б�

	//�����˿�
protected:
	BYTE							m_cbConcealCount;					//������Ŀ
	BYTE							m_cbConcealCard[8];					//�����˿�

	//�û��˿�
protected:
	BYTE							m_cbHandCardCount[4];				//�˿���Ŀ
	BYTE							m_cbHandCardData[4][MAX_COUNT];		//�����˿�

	//�������
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	ITableFrame						* m_pITableFrame;					//��ܽӿ�
	const tagGameServiceOption		* m_pGameServiceOption;				//���ò���
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

	//�����ӿ�
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
	virtual bool __cdecl SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);
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

	//�����¼�
public:
	//����ͬ��
	virtual bool __cdecl OnEventUserReqReady(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//�������
	virtual bool __cdecl OnEventUserReqOffLine(WORD wChairID, IServerUserItem * pIServerUserItem, WORD wOffLineCount) { return wOffLineCount<3; }
	//��������
	virtual bool __cdecl OnEventUserReqReConnect(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//��������
	virtual bool __cdecl OnEventUserReqSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bReqLookon) { return true; }
	//��������
	virtual bool __cdecl OnEventUserReqStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bReqLookon) { return true; }

	//�����¼�
public:
	//�û�ͬ��
	virtual bool __cdecl OnEventUserReady(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool __cdecl OnEventUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool __cdecl OnEventUserReConnect(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool __cdecl OnEventUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser) { return true; }
	//�û�����
	virtual bool __cdecl OnEventUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);

	//��Ϸ�¼�
protected:
	//�û�����
	bool OnUserCallCard(WORD wChairID, BYTE cbCallCard, BYTE cbCallCount);
	//�������
	bool OnUserCallFinish(WORD wChairID);
	//�����˿�
	bool OnUserConcealCard(WORD wChairID, BYTE cbConcealCard[], BYTE cbConcealCount);
	//�û�����
	bool OnUserOutCard(WORD wChairID, BYTE cbCardData[], BYTE cbCardCount);

	//�˹����ܺ���
protected:
	//��������
	BYTE AI_GetCallCardMask(WORD wCallCardUser);
	//��������
	BYTE AI_GetCallCardMaskByLessCardCount(WORD wUser);
	//����
	bool AI_CallCard(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbCallColor);
	//���
	bool AI_ConcealCard(WORD wChairID, IServerUserItem * pIServerUserItem);
	//����
	bool AI_AutomatismOutCard(WORD wChairID, IServerUserItem * pIServerUserItem);
	//Ѱ����ѳ���
	bool AI_SearchFirstBestOutCard(BYTE cbHandCardData[],
		BYTE cbHandCardCount,
		WORD wChairID,
		tagOutCardResult& OutCardResult);
	//�жϸ������Ƶ��ƴ���
	bool AI_CompareMostCardExist(BYTE cbHandCardData[],
		BYTE cbHandCardCount,tagTractorDataInfo TractorDataInfo);
	//�жϸ���ͬ�Ƶ��ƴ���
	bool AI_CompareMostCardExist(BYTE cbHandCardData[],
		BYTE cbHandCardCount,tagSameDataInfo SameDataInfo);
	//�жϸ����Ƶ��ƴ���
	bool AI_CompareMostCardExist(BYTE cbHandCardData[],
		BYTE cbHandCardCount,BYTE cbCardData);
	//Ѱ��ƥ�����
	bool AI_SearchMatchOutCard(BYTE cbHandCardData[],
		BYTE cbHandCardCount,
		BYTE cbMatchCardData[],
		BYTE cbMatchCardCount,
		tagOutCardResult& OutCardResult);
	//��������
protected:
	//�ɷ��˿�
	bool DispatchUserCard();
};

//////////////////////////////////////////////////////////////////////////

#endif