#pragma once
#include "Stdafx.h"
#include "GameServiceExport.h"

//���������ʶ
enum CALVALUEFLAG
{
	CALVALUEFLAG_HIGH	= 0,//����
	CALVALUEFLAG_SELF,		//����
	CALVALUEFLAG_LESS,		//����
	CALVALUEFLAG_COUNT,
};

//���㵥Ԫ
class CCalculateItem : public ICalculateItem
{
	//��Ԫ��
	friend class CCalculateFrame;
public:
	//���캯��
	CCalculateItem(void);
	//��������
	~CCalculateItem(void);

	//���ýӿ�
public:
	//��������
	virtual void __cdecl AddUserRefCount();
	//��������
	virtual void __cdecl ReleaseUserRefCount();
	//�ж�����
	virtual bool __cdecl IsUserRefCountZero();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual bool __cdecl Release() { if (IsValid()) delete this; return true; }
	//�Ƿ���Ч
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CCalculateItem))?true:false; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//ICalculateItem��ܽӿ�
public:
	//�û� I D
	virtual DWORD __cdecl GetUserID() { return m_ServerUserData.UserData.dwUserID; }
	//���û� I D
	virtual DWORD __cdecl GetParentUserID(){return m_ServerUserData.UserData.dwParentUserID;}
	//�û��ȼ�
	virtual BYTE __cdecl GetUserLevel(){ return m_ServerUserData.UserData.cbLevel;}
	//��ȡ��Ϣ
	virtual tagServerUserData * __cdecl GetUserData() { return &m_ServerUserData; };
	//��ȡ�û�����ID
	virtual WORD	 __cdecl GetChairID(){return m_wChairID;}

	//��ȡ��Ͷע
	virtual bool __cdecl GetTotalBetScore(DECIMAL* pTotalBetScore);
	//��ȡ���ɲ�
	virtual bool __cdecl GetTotalWinScore(DECIMAL* pTotalWinScore);
	//��ȡ�ܳ�ˮ
	virtual bool __cdecl GetTotalTaxScore(DECIMAL* pTotalTaxScore);

	//��ȡ�ɲ�ռ��
	virtual bool __cdecl GetWinScoreOccupancy(DECIMAL* pHighScore,
		DECIMAL* pSelfScore,DECIMAL *pLessScore);
	//��ȡ��ˮռ��
	virtual bool __cdecl GetTaxScoreOccupancy(DECIMAL* pHighScore,
		DECIMAL* pSelfScore,DECIMAL *pLessScore);
	//��ȡϴ���뽻�ն��
	virtual bool __cdecl GetValidBetScoreAndPaidScore(DECIMAL* pTotalScore,
		DECIMAL* pHighScore, DECIMAL *pHighRollbackScore,
		DECIMAL* pLessScore, DECIMAL *pLessRollbackScore,
		DECIMAL* pHighPaidScore, DECIMAL* pLessPaidScore);
	//��ȡͶע��ϸ��Ϣ
	virtual bool __cdecl GetDetailBetScore(BYTE *pDetailBetScore,
											WORD& wDetailBetScoreSize);

	//��λ����
	virtual bool __cdecl Reset();

	//���ýӿ�
public:
	//��ʼ��
	virtual bool __cdecl InitCalculateItem(IServerUserItem*	pIServerUserItem,
											CCalculateFrame* pCalculateFrame,
											WORD wBetRegionCount,
											WORD wChairID = INVALID_CHAIR);
	//��������
public:
	//��ȡ����
	CCalculateItem*	__cdecl	GetParent(){return m_pParentCalculateItem;}
	//��������
	void __cdecl SetParent(CCalculateItem* pItem){m_pParentCalculateItem = pItem;}
	//���ü���
	void __cdecl Active(bool b){m_bActive = b;}

	//�û���ϵ��ȷ
	virtual bool __cdecl IsUserTreeFrameOK();

	//Ͷע
	bool __cdecl BetScore(DECIMAL *pdecBetScore, WORD wCount);
	//Ͷע
	bool __cdecl BetScore(DECIMAL* pdecUserBetScore,
		DECIMAL* pdecLessBetScore,
		CCalculateItem* pLessCalculateItemChain[USERLEVEL_GAMEUSER+1],
		CCalculateItem* pLessCalculateItem);
	//���������Ͷע
	bool __cdecl AddPartedBetScore(DECIMAL* pdecUserBetScore,
		DECIMAL* pdecHighBetScore,
		DECIMAL* pdecSelfBetScore,
		DECIMAL* pdecLessBetScore);
	//������ߵ���Ͷע
	bool __cdecl AddParentPartedBetScore(DECIMAL* pdecUserBetScore,
		DECIMAL* pdecHighBetScore,
		DECIMAL* pdecSelfBetScore,
		DECIMAL* pdecLessBetScore);

	//�ɲ�
	bool __cdecl WinScore(DECIMAL *pdecWinScore,
		DECIMAL *pdecUserTaxScore, 
		WORD wCount);
	//�ɲ�
	bool __cdecl WinScore(DECIMAL* pdecUserWinScore,
		DECIMAL* pdecLessWinScore,
		DECIMAL* pdecUserTaxScore,
		DECIMAL *pdecLessTaxScore, 
		CCalculateItem* pLessCalculateItemChain[USERLEVEL_GAMEUSER+1],
		CCalculateItem* pLessCalculateItem);

	//����������ɲ�
	bool __cdecl AddPartedWinScore(DECIMAL* pdecUserWinScore,
		DECIMAL* pdecHighWinScore,
		DECIMAL* pdecSelfWinScore,
		DECIMAL* pdecLessWinScore);
	//������ߵ����ɲ�
	bool __cdecl AddParentPartedWinScore(DECIMAL* pdecUserWinScore,
		DECIMAL* pdecHighWinScore,
		DECIMAL* pdecSelfWinScore,
		DECIMAL* pdecLessWinScore);

	//����������ˮ
	bool __cdecl AddPartedTaxScore(DECIMAL* pdecUserTaxScore,
		DECIMAL* pdecHighTaxScore,
		DECIMAL* pdecSelfTaxScore,
		DECIMAL* pdecLessTaxScore);
	//������ߵ����ˮ
	bool __cdecl AddParentPartedTaxScore(DECIMAL* pdecUserTaxScore,
		DECIMAL* pdecHighTaxScore,
		DECIMAL* pdecSelfTaxScore,
		DECIMAL* pdecLessTaxScore);

	//�����ɲ�
	bool __cdecl CalculateWinScore(void* pCalculateContext,
		ICalculateSink* pCalculateSink);
	//�����ܽ��
	bool __cdecl CalculateResult(void* pCalculateContext,
		ICalculateSink* pCalculateSink);

	//��������
protected:
	bool				m_bActive;	
	LONG				m_lUserRefCount;				//�������ü���
	CCalculateItem*		m_pParentCalculateItem;			//����

	CCalculateFrame*	m_pCalculateFrame;				//������
	tagServerUserData	m_ServerUserData;				//�û���Ϣ
	WORD				m_wChairID;						//����λ��
	
	WORD				m_wBetRegionCount;				//Ͷע������Ŀ
	//������Ϣ
protected:
	DECIMAL				m_TotalBetScore;				//��Ͷע

	//����Ͷע��Ϣ
protected:
	DECIMAL*			m_TotalPartedBetScore;			//����Ͷע������Ͷע��Ϣ
	DECIMAL*			m_PartedBetScore[CALVALUEFLAG_COUNT];//������Ͷע����Ͷע��Ϣ
	DECIMAL*			m_ParentPartedBetScore[CALVALUEFLAG_COUNT];//���ߵ���Ͷע����Ͷע��Ϣ

	//�����ɲ���Ϣ
protected:
	DECIMAL*			m_TotalPartedWinScore;			//����Ͷע�������ɲ���Ϣ
	DECIMAL*			m_PartedWinScore[CALVALUEFLAG_COUNT];//������Ͷע�����ɲ���Ϣ
	DECIMAL*			m_ParentPartedWinScore[CALVALUEFLAG_COUNT];//���ߵ���Ͷע�����ɲ���Ϣ

	//�����ˮ��Ϣ
protected:
	DECIMAL*			m_TotalPartedTaxScore;			//����Ͷע�����ܳ�ˮ��Ϣ
	DECIMAL*			m_PartedTaxScore[CALVALUEFLAG_COUNT];//������Ͷע�����ˮ��Ϣ
	DECIMAL*			m_ParentPartedTaxScore[CALVALUEFLAG_COUNT];//���ߵ���Ͷע�����ˮ��Ϣ

	//����Ͷע��Ϣ
protected:
	DECIMAL				m_TotalWinScore;				//���ɲ�
	DECIMAL				m_WinScore[CALVALUEFLAG_COUNT]; //�����ɲ���Ϣ
	DECIMAL				m_ParentWinScore[CALVALUEFLAG_COUNT]; //�����ɲ���Ϣ

	DECIMAL				m_TotalValidBetScore;			//��ϴ��
	DECIMAL				m_ValidBetScore[CALVALUEFLAG_COUNT]; //����ϴ����Ϣ
	DECIMAL				m_ParentValidBetScore[CALVALUEFLAG_COUNT]; //����ϴ����Ϣ

	DECIMAL				m_TotalTaxScore;				//�ܳ�ˮ
	DECIMAL				m_TaxScore[CALVALUEFLAG_COUNT]; //�����ˮ��Ϣ
	DECIMAL				m_ParentTaxScore[CALVALUEFLAG_COUNT]; //���߳�ˮ��Ϣ
};
//////////////////////////////////////////////////////////////////////////
//��˵��
typedef CArrayTemplate<CCalculateItem *> CCalculateItemArray;
typedef CArrayTemplate<DWORD>			 CCalculateItemPosArray;

//������
class CCalculateFrame : public ICalculateFrame
{
	//��Ԫ��
	friend class CCalculateItem;

public:
	//���캯��
	CCalculateFrame(void);
	//��������
	~CCalculateFrame(void);

	//�����ӿ�
public:
	//�ͷŶ���
	virtual bool __cdecl Release() { if (IsValid()) delete this; return true; }
	//�Ƿ���Ч
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CCalculateFrame))?true:false; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//ICalculateFrame��ܽӿ�
public:
	//ö�ټ��㵥Ԫ
	virtual ICalculateItem* __cdecl EnumCalculateItem(WORD wEnumIndex);
	//��ȡ���㵥Ԫ��Ŀ
	virtual DWORD	__cdecl GetCalculateItemCount();

	//��ȡ��Ͷע
	virtual bool __cdecl GetFrameTotalBetScore(DECIMAL* pTotalBetScore);
	//��ȡ���ɲ�
	virtual bool __cdecl GetFrameTotalWinScore(DECIMAL* pTotalWinScore);

	//�û�����
	virtual bool __cdecl OnEventUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool __cdecl OnEventUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);

	//��λ����
	virtual bool __cdecl Reset();
	//����������Ϣ
	virtual bool __cdecl StructCalculateItem();

	//Ͷע
	virtual bool __cdecl BetScore(IServerUserItem * pIServerUserItem,
									tagBetScoreInfo *pBetScoreInfo, WORD wCount);
	//���Ͷע
	virtual BYTE __cdecl CheckBetScore(IServerUserItem * pIServerUserItem,
									tagBetScoreInfo *pBetScoreInfo, WORD wCount);

	//�������Ի�Ա��ʽ
	virtual bool __cdecl CalculateCTP(void* pCalculateContext,
									ICalculateSink* pCalculateSink);
	//�����Ա�Ի�Ա��ʽ
	virtual bool __cdecl CalculatePTP(tagBetScoreInfo *pBetScoreInfo, WORD wCount);

	//���ýӿ�
public:
	//��ʼ��
	virtual bool __cdecl InitCalculateFrame(WORD wTableID, 
		ITableFrameManager*	pITableFrameManager,
		tagTableFrameParameter * pTableFrameParameter);

	//��������
protected:
	//��Ӽ��㵥Ԫ
	CCalculateItem* AddAndCheckUserCalculateItem(WORD wChairID, IServerUserItem * pIServerUserItem);
	//ɾ�����㵥Ԫ
	bool		    DelAndCheckUserCalculateItem(WORD wChairID, IServerUserItem * pIServerUserItem);
	//���Ҽ��㵥Ԫ
	CCalculateItem* SearchCalculateItem(DWORD dwUserID);
	//���Ӽ��㵥Ԫ
	void			AddUserCalculateItem(CCalculateItem* pItem);
	//����ص�
	void			BetScoreSink(ICalculateItem * pICalculateItem, DECIMAL* pdecTotalBetScore);

	//������Ϣ
protected:
	const tagGameServiceAttrib		* m_pGameServiceAttrib;				//��������
	const tagGameServiceOption		* m_pGameServiceOption;				//��������
	IGameServiceFrame				* m_pIGameServiceFrame;				//���ܽӿ�
	IServerUserManager				* m_pIServerUserManager;			//�û��ӿ�
	ITableFrameManager				* m_pITableFrameManager;			//���ӽӿ�
	WORD							m_wTableID;							//����ID

	//���㶨��
protected:
	CCalculateItemArray				m_CalculateItemArray;				//���㵥Ԫ����
	CCalculateItem*					m_pCompancyCalculateItem;			//��˾���㵥Ԫ

	//�������
protected:
	CQueueServiceEvent				m_DataBaseEvent;					//���ݿ�֪ͨ
};
