#ifndef TABLE_FRAME_HEAD_FILE
#define TABLE_FRAME_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameServiceExport.h"
#include "AttemperEngineSink.h"
#include "CalculateFrame.h"

//////////////////////////////////////////////////////////////////////////

//�û��ӿ�����
typedef CArrayTemplate<IServerUserItem *> CUserItemPtrArray;

//��Ϸ���ӿ��
class CTableFrame : public ITableFrameManager
{
	//���Ա���
protected:
	WORD							m_wTableID;							//���Ӻ���
	WORD							m_wChairCount;						//������Ŀ

	//״̬����
protected:
	bool							m_bLocked;							//������־
	bool							m_bStarted;							//��ʼ��־
	BYTE							m_bGameStatus;						//��Ϸ״̬
	ULONGLONG						m_ullTimeStart;						//��ʼʱ��
	DWORD							m_dwTableOwnerID;					//̨�� I D
	TCHAR							m_szPassword[PASS_LEN];				//��������
	WORD							m_wUserCount;						//�û���Ŀ
	WORD							m_wAIGameUserCount;					//AI�û���Ŀ

	//�����Ϣ
protected:
	bool							m_bWriteScore;						//�Ƿ�д��
	DWORD							* m_dwPlayerID;						//��� I D

	//�û���Ϣ
protected:
	bool							* m_bCheckBetScoreFlag;				
	bool							* m_bAllowLookon;					//�Թ۱�־
	WORD							* m_wOffLineCount;					//���ߴ���
	IServerUserItem					** m_pIUserItem;					//�û�ָ��
	CUserItemPtrArray				m_LookonUserItemPtr;				//�Թ��û�

	//������Ϣ
protected:
	const tagGameServiceAttrib		* m_pGameServiceAttrib;				//��������
	const tagGameServiceOption		* m_pGameServiceOption;				//��������

	//����ӿ�
protected:
	ITableFrameSink					* m_pITableFrameSink;				//���ӽӿ�
	ITableFrameEvent				* m_pITableFrameEvent;				//�¼��ӿ�
	IGameServiceFrame				* m_pIGameServiceFrame;				//���ܽӿ�

	//�������
protected:
	CQueueServiceEvent				m_DataBaseEvent;					//���ݿ�֪ͨ

	//������Ϣ
protected:
	CCalculateFrame					m_CalculateFrame;					//������
	ULONGLONG						m_ullGameRoundID;					//��Ϸ�ּ�¼ID

	//��������
public:
	//���캯��
	CTableFrame();
	//��������
	virtual ~CTableFrame();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual bool __cdecl Release() { if (IsValid()) delete this; return true; }
	//�Ƿ���Ч
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CTableFrame))?true:false; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//���ýӿ�
public:
	//��ʼ��
	virtual bool __cdecl InitTableFrame(WORD wTableID, tagTableFrameParameter * pTableFrameParameter);

	//��Ϣ�ӿ�
public:
	//���Ӻ���
	virtual WORD __cdecl GetTableID() { return m_wTableID; }
	//��Ϸ����
	virtual WORD __cdecl GetChairCount() { return m_wChairCount; }
	//��Ϸʱ��
	virtual ULONGLONG __cdecl GetPlayTimeCount() { return (ULONGLONG)_time64(NULL)-m_ullTimeStart; }
	//��λ����Ŀ
	virtual WORD __cdecl GetNullChairCount(){return m_wChairCount - m_wUserCount;}
	//��λ�ú���
	virtual WORD __cdecl GetNullChairID();
	//��ͬIP��ַλ��ID
	virtual WORD __cdecl GetSameIPChairID(IServerUserItem * pIServerUserItem);
	//��ͨ�������
	virtual WORD __cdecl GetGameUserCount(){return m_wUserCount - m_wAIGameUserCount;}

	//AI��Ϣ�ӿ�
public:
	//AI�������
	virtual WORD __cdecl GetAIGameUserCount(){return m_wAIGameUserCount;}
	//��ȡAI��ɱ����ͨ���������
	virtual WORD __cdecl GetAIKillGameUserChairID();
	//�Ƿ�AI��ɱ��ͨ���
	virtual bool __cdecl IsAIKillGameUser(); 

	//�û��ӿ�
public:
	//ö���û�
	virtual IServerUserItem * __cdecl EnumLookonUserItem(WORD wIndex);
	//��ȡ�û�
	virtual IServerUserItem * __cdecl GetServerUserItem(WORD wChairID);

	//״̬�ӿ�
public:
	//��ȡ״̬
	virtual BYTE __cdecl GetGameStatus() { return m_bGameStatus; }
	//����״̬
	virtual void __cdecl SetGameStatus(BYTE bGameStatus) { m_bGameStatus=bGameStatus; }
	//��ȡ����
	virtual const tagGameServiceAttrib * __cdecl GetGameServiceAttrib() { return m_pGameServiceAttrib; }
	//��ȡ����
	virtual const tagGameServiceOption * __cdecl GetGameServiceOption() { return m_pGameServiceOption; }

	//��ʱ���ӿ�
public:
	//���ö�ʱ��
	virtual bool __cdecl SetGameTimer(WORD wTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM wBindParam);
	//ɾ����ʱ��
	virtual bool __cdecl KillGameTimer(WORD wTimerID);

	//����ӿ�
public:
	//��������
	virtual bool __cdecl SendUserData(IServerUserItem * pIServerUserItem, WORD wSubCmdID);
	//��������
	virtual bool __cdecl SendUserData(IServerUserItem * pIServerUserItem, WORD wSubCmdID, void * pData, WORD wDataSize);
	//��������
	virtual bool __cdecl SendTableData(WORD wChairID, WORD wSubCmdID);
	//��������
	virtual bool __cdecl SendTableData(WORD wChairID, WORD wSubCmdID, void * pData, WORD wDataSize);
	//��������
	virtual bool __cdecl SendLookonData(WORD wChairID, WORD wSubCmdID);
	//��������
	virtual bool __cdecl SendLookonData(WORD wChairID, WORD wSubCmdID, void * pData, WORD wDataSize);

	//��Ϣ�ӿ�
public:
	//���ͷ�����Ϣ
	virtual bool __cdecl SendRoomMessage(IServerUserItem * pIServerUserItem, WORD wErrCode,LPCTSTR lpszMessage, WORD wMessageType);
	//������Ϸ��Ϣ
	virtual bool __cdecl SendGameMessage(IServerUserItem * pIServerUserItem, WORD wErrCode,LPCTSTR lpszMessage, WORD wMessageType);

	//��Ϸ�ӿ�
public:
	//��ɢ��Ϸ
	virtual bool __cdecl DismissGame();
	//������Ϸ
	virtual bool __cdecl ConcludeGame();
	//����
	virtual bool __cdecl WriteGameEnd(BYTE cbReason,
						BYTE *pEndData,
						WORD wEndDataSize,
						void* pCalculateContext,
						ICalculateSink* pCalculateSink);
	//���ͳ���
	virtual bool __cdecl SendGameScene(IServerUserItem * pIServerUserItem, void * pData, WORD wDataSize);
	//����ӿ�
public:
	//��ȡ������
	virtual ICalculateFrame* __cdecl GetICalculateFrame();

	//ִ�нӿ�
public:
	//���¶���
	virtual bool __cdecl PerformSitDownAction(WORD wChairID, IServerUserItem * pIServerUserItem);
	//�Թ۶���
	virtual bool __cdecl PerformLookonAction(WORD wChairID, IServerUserItem * pIServerUserItem);
	//�뿪����
	virtual bool __cdecl PerformStandUpAction(IServerUserItem * pIServerUserItem);
	//�˹�������Ϸ����
	virtual bool __cdecl PerformAIGameAction();

	//�û��¼�
public:
	//�����¼�
	virtual bool __cdecl OnUserOffLine(WORD wChairID);
	//�ؽ��¼�
	virtual bool __cdecl OnUserReConnect(WORD wChairID);
	//�������
	virtual bool __cdecl OnUserReqOffLine(WORD wChairID);

	//ϵͳ�¼�
public:
	//��ʱ���¼�
	virtual bool __cdecl OnEventTimer(WORD wTimerID, WPARAM wBindParam);
	//��Ϸ�¼�����
	virtual bool __cdecl OnEventSocketGame(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//����¼�����
	virtual bool __cdecl OnEventSocketFrame(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//��Ϣ�ӿ�
public:
	//����״̬
	virtual bool __cdecl IsTableLocked() { return m_bLocked; }
	//��Ϸ״̬
	virtual bool __cdecl IsGameStarted() { return m_bStarted; }
	//��Ϸ״̬
	virtual bool __cdecl IsUserPlaying(IServerUserItem * pIServerUserItem);
	
	//�����¼�
public:
	//������Ϸ�ּ�¼-������ö���¼�
	void OnEventAllocGameRoundAndCheckBetScore(DBR_GR_AllocGameRoundAndCheckBetScoreResult * pAllocGameRoundAndCheckBetScoreResult);
	//������Ϸ�ּ�¼�¼�
	void OnEventAllocGameRoundResult(DBR_GR_AllocGameRoundResult * pAllocGameRoundResult);

	//�ڲ�����
private:
	//���Կ�ʼ��Ϸ
	void TryStartGame();
	//��ʼ��Ϸ
	void StartGame();
	//��ʼ�ж�
	bool StartVerdict();
	//ȫ���������ж�
	bool IsAllAIGameUser();

	//��������
protected:
	//��������ʧ��
	void SendSitFailedPacket(IServerUserItem * pIServerUserItem, WORD wFailedCode);
};

//////////////////////////////////////////////////////////////////////////

#endif