#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//����˿�����
#define TeShu                               0
#define SHAN								1
#define DUI_ZI								2
#define SHUN_ZI								3
#define	TONG_HUA							4
#define TONG_HUA_SHUN						5
#define SAN_TIAO							6


//�˿˻�ɫ����
#define FANG_KUAI							0
#define MEI_HUA								1
#define HONG_TAO							2
#define HEI_TAO								3

//����ԭ��
#define GER_NO_PLAYER						0x10						//û�����


//��Ϸ�߼���
class CGameLogic
{
	//��������

	//��������
private:
	static const BYTE				m_bCardArray[52];					//�˿˶���

	//��������
private:
	//��ȡ�˿˻�ɫ
	BYTE GetHuaKind(BYTE bCard);
	//��ȡ�˿���ֵ
	BYTE GetCardValoe(BYTE bCard);
	//��ȡ�˿�����
	BYTE GetCardKind(BYTE bCardList[], BYTE bCardCount);
	//���Ҷ���
	BYTE FindDoubleCard(BYTE bCardList[], BYTE bCardCount, BYTE * bOutDoubleCard);

	//���ͺ���
public:
	//�Ƿ�ͬ��˳
	bool IsTongHuaShun(BYTE bCardList[], BYTE bCardCount);
	//�Ƿ���֧
	bool IsTieZhi(BYTE bCardList[], BYTE bCardCount);
	//�Ƿ��«
	bool IsHuLu(BYTE bCardList[], BYTE bCardCount);
	//�Ƿ�ͬ��
	bool IsTongHua(BYTE bCardList[], BYTE bCardCount);
	//�Ƿ�˳��
	bool IsShunZhi(BYTE bCardList[], BYTE bCardCount);
	//�Ƿ�����
	bool IsSanTiao(BYTE bCardList[], BYTE bCardCount);
	//�Ƿ�����
	bool IsLiangDui(BYTE bCardList[], BYTE bCardCount);
	//�Ƿ����
	bool IsDuiZhi(BYTE bCardList[], BYTE bCardCount);
	//������
	bool IsTeShu(BYTE bCardList[], BYTE bCardCount);
	

	//�ԱȺ���
public:
	//�Աȵ�ֻ�˿�
	bool CompareOnlyOne(BYTE bFirstCard, BYTE bLastCard);
	//�Ա�ͬ��˳
	bool CompareTongHuaShun(BYTE bFirstList[], BYTE bLastList[]);
	//�Ա���֧
	bool CompareTeiZhi(BYTE bFirstList[], BYTE bLastList[]);
	//�ԱȺ�«
	bool CompareHuLu(BYTE bFirstList[], BYTE bLastList[]);
	//�Ա�ͬ��
	bool CompareTongHua(BYTE bFirstList[], BYTE bLastList[]);
	//�Ա�˳��
	bool CompareShunZhi(BYTE bFirstList[], BYTE bLastList[]);
	//�Ա�����
	bool CompareSanTiao(BYTE bFirstList[], BYTE bLastList[]);
	//�Ա�����
	bool CompareLiangDui(BYTE bFirstList[], BYTE bLastList[], BYTE bCardCount);
	//�Աȶ���
	bool CompareDuiZhi(BYTE bFirstList[], BYTE bLastList[], BYTE bCardCount);
	//�Ա�ɢ��
	bool CompareSanCard(BYTE bFirstList[], BYTE bLastList[], BYTE bCardCount);
	

	//���ܺ���
public:
	//�����˿�
	void RandCard(BYTE bCardBuffer[], BYTE bBufferCount);
	//�����˿�
	void SortCard(BYTE bCardList[], BYTE bCardCount);
	//�Ա��˿�
	bool CompareCard(BYTE bFirstList[], BYTE bLastList[],BYTE bCardCount);
};
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
	//��Ϸ����
protected:
	WORD							m_wOperaCount;							//��������
	WORD							m_wCurrentUser;							//��ǰ�û�
	WORD							m_wBeginTurnUser;						//��ǰ�û�
	BYTE							m_bSendCardCount;						//������Ŀ
	BYTE							m_bTableCardCount[GAME_PLAYER];			//�˿���Ŀ
	BYTE							m_bTableCardArray[GAME_PLAYER][3];		//�����˿� 
	WORD                            m_wCurrentWiner;                        //Ӯ�����Ӻ�

	//�����Ϣ
protected:
	DOUBLE							m_fMaxGold;								//�����ע
	DOUBLE							m_fTurnMaxGold;							//�����ע
	DOUBLE							m_fTurnBasicGold;						//������ע
	DOUBLE							m_fTableGold[2*GAME_PLAYER];			//��ע���

	//��������
protected:
	BYTE							m_bPlayStatus[GAME_PLAYER];				//��Ϸ״̬
	BYTE                            m_bKanPaiFlag[GAME_PLAYER+1];           //����״̬   
	
	//�������
protected:
	CGameLogic						m_GameLogic;							//��Ϸ�߼�
	CCalculateSink					m_CalculateSink;						//����ص�
	ITableFrame						* m_pITableFrame;						//��ܽӿ�
	ICalculateFrame					* m_pICalculateFrame;					//����ӿ�
	const tagGameServiceOption		* m_pGameServiceOption;					//���ò���
	const tagGameServiceAttrib		* m_pGameServiceAttrib;					//���Բ���

	//���Ա���
protected:
	static const WORD				m_wPlayerCount;							//��Ϸ����
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
	//�����¼�
	bool OnUserGiveUp(WORD wChairID);
	//��ע�¼�
	bool OnUserAddGold(WORD wChairID, DOUBLE fGold, bool bCancel);
	//�����¼�
	bool OnUserKanPai(BOOL bKanPai);

	//��Ϸ����
protected:
	//�ƶ�ʤ��
	WORD DeduceWiner(BYTE bBeginPos = 0, BYTE bEndPos = 0);	
	//�ƶ�����
	WORD DeduceLoser(BYTE bBeginPos = 0, BYTE bEndPos = 0);	
};

//////////////////////////////////////////////////////////////////////////


#endif