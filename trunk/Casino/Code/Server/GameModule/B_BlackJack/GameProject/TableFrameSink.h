#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "..\CommandDefinition\CMD_BlackJack.h"
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
class CTableFrameSink : public ITableFrameSink
{
	//��Ϸ����
protected:
	WORD							m_wPlayerCount;							//�������	
	WORD							m_wCurrentUser;							//��ǰ�û�
	
	//�������
protected:	
	BYTE							m_cbTableCutCardCount[GAME_PLAYER];		//�����˿�
	BYTE							m_cbTableCutCardArray[GAME_PLAYER][13];	//�����˿�  
	bool                            m_cbCutCard[GAME_PLAYER];               //�Ƿ����

	//��ʼ�˿�
protected:
	BYTE							m_cbTableCardCount[GAME_PLAYER];		//�˿���Ŀ
	BYTE							m_cbTableCardArray[GAME_PLAYER][13];	//�����˿�  
    
	//���Ʊ���
protected:
	BYTE			            	m_cbRandCardArray[52];					//ɢ���˿�
	BYTE			            	m_cbSendCardCount;				        //�ѷ��˿�

	//��Ϸׯ��
protected:	
	int 						    m_nBankerStation;						//ׯ��λ��
	BOOL                            m_bBankerWin;                           //ׯ�һ�ʤ
	BOOL							m_bBankerGetCard;                       //ׯ��Ҫ��  
	DOUBLE							m_fBankerScore;							//ׯ�Ҷ��

	//��Ϸ�м�
protected:
	int								m_nNotBanker;
	DOUBLE							m_fNotBankerScore[GAME_PLAYER];			//�мҶ��

	//��ʶ��ע
protected:
	BOOL							m_bAddGold[GAME_PLAYER];				//�Ƿ��ע	
	bool                            m_bAddGoldEnd;                          //��ע����	

	//�����Ϣ
protected:
	DOUBLE							m_fMaxGold;								//�����ע
	DOUBLE							m_fTurnBasicGold;						//������ע
	DOUBLE							m_fTableGold[2*GAME_PLAYER];			//��ע���
	DOUBLE                          m_fUserAddGold[GAME_PLAYER];            //�û���ע
	
	//���״̬
protected:
	BYTE							m_cbCardStatus[GAME_PLAYER];		   //��ʼ����
	BYTE							m_cbCutCardStatus[GAME_PLAYER];		   //�����û�

	//�������
protected:
	CGameLogic						m_GameLogic;							//��Ϸ�߼�
	ITableFrame						* m_pITableFrame;						//��ܽӿ�
	const tagGameServiceOption		* pGameServiceOption;					//���ò���
	ICalculateFrame					* m_pICalculateFrame;				//����ӿ�
	const tagGameServiceAttrib		* m_pGameServiceAttrib;				//���Բ���
	CCalculateSink					m_CalculateSink;					//����ص�

	//���Ա���
protected:
	static const enStartMode		m_GameStartMode;						//��ʼģʽ

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
	virtual bool __cdecl SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret);
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

	//��Ϸ����
protected:
	//��ȡ�˿�
	BYTE GetRandCard();
	//�û�ǿ��
	bool OnUserLeft(bool bAddGoldEnd);
    //�û�ǿ��
	bool OnUserLeft(WORD wChairID, bool bAddGoldEnd);
	//����ɼ�
	bool CalcScore(BYTE cbCardStatus, WORD wChairID, DOUBLE & fGameGold, DOUBLE & fBankerScore, bool bCutCard);
	//���Ͷע
	DOUBLE GetLastGold(WORD wChairID);
};

//////////////////////////////////////////////////////////////////////////

#endif