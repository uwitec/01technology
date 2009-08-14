#ifndef DATA_BASE_SINK_HEAD_FILE
#define DATA_BASE_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameServiceExport.h"
#include "..\..\CommonDefinition\GlobalOperation.h"
//////////////////////////////////////////////////////////////////////////
//������
struct tagOperationUserInfo
{
	DWORD			dwOperationUserID;					//�����û�ID
	BYTE			cbOperationUserType;				//�����û�����
	TCHAR			szOperationAccount[NAME_LEN];		//�����ʺ�
	DWORD			dwIPAddress;						//������ַ
};
//////////////////////////////////////////////////////////////////////////
//�ṹ����

//���ݿ����
struct tagDataBaseSinkParameter
{
	tagDataBaseInfo						* pGameUserDBInfo;				//������Ϣ
	tagDataBaseInfo						* pGameScoreDBInfo;				//������Ϣ
	tagGameServiceAttrib				* pGameServiceAttrib;			//��������
	tagGameServiceOption				* pGameServiceOption;			//������Ϣ
};

//////////////////////////////////////////////////////////////////////////
//���ݿ��������ݰ�

//���ݿ������ʶ
#define	DBR_GR_LOGON_BY_ACCOUNT			1								//�ʻ���½
#define	DBR_GR_READ_ACCOUNT				2								//�ʻ���ȡ
#define DBR_GR_WRITE_GAMEROUND			3								//д����Ϸ�ּ�¼
#define DBR_GR_WRITE_USERCHART			4								//д���û�����
#define DBR_GR_ALLOC_GAMEROUNDANDCHECKBETSCORE		5					//������Ϸ�ּ�¼-������ö��
#define DBR_GR_ALLOC_GAMEROUND			6								//������Ϸ�ּ�¼
#define DBR_GR_RESET_BETSCORE			7								//��λͶע
#define DBR_GR_INSERT_BETSCORE			8								//����Ͷע
#define DBR_GR_KICK_USER				9								//�߳�����
#define DBR_GR_READ_AIUSER				20								//װ�ػ�����

//���ݿ������ʶ
#define DBR_GR_LOGON_SUCCESS			100								//��½�ɹ�
#define DBR_GR_LOGON_ERROR				101								//��½ʧ��
#define DBR_GR_READACCOUNT_SUCCESS		200								//��ȡ�ɹ�
#define DBR_GR_READACCOUNT_ERROR		201								//��ȡʧ��
#define DBR_GR_USERSCORE				400								//�û����ö��
#define DBR_GR_ALLOC_GAMEROUNDANDCHECKBETSCORE_RESULT		500			//������Ϸ�ּ�¼-������ö�Ƚ��
#define DBR_GR_ALLOC_GAMEROUND_RESULT	600								//������Ϸ�ּ�¼
#define DBR_GR_READ_AIAGENTUSER_SUCCESS	2000							//װ�ش�������˳ɹ�
#define DBR_GR_READ_AIGAMEUSER_SUCCESS	2001							//װ����һ����˳ɹ�
#define DBR_GR_READ_AIUSER_FINISH		2002							//װ�ػ��������

//�ʻ���½
struct DBR_GR_LogonByAccount
{
	DWORD								dwClientIP;						//���ӵ�ַ
	TCHAR								szAccounts[NAME_LEN];			//��¼�ʺ�
	TCHAR								szPassWord[PASS_LEN];			//��¼����
};

//�ʻ���ȡ
struct DBR_GR_ReadUser
{
	DWORD								dwUserID;						//�û� I D
};

//��½�ɹ�
struct DBR_GR_LogonSuccess
{
	//������Ϣ
	tagUserData							UserData;
	DECIMAL								decOccupancyRate;					//ռ�ɱ�
	DECIMAL								decTaxOccupancyRate;			//��ˮռ�ɱ�
	DECIMAL								decRollbackRate;					//ϴ���
	DWORD								dwGameRight;						//��ϷȨ��
	BYTE								cbUserType;							//�û�����
	DWORD								dwRealUserID;						//�û�ID
};

//��½ʧ��
struct DBR_GR_LogonError
{
	WORD								wErrorCode;						//�������
};
//��ȡ�ɹ�
struct DBR_GR_ReadUserSuccess
{
	//������Ϣ
	tagUserData							UserData;
	DECIMAL								decOccupancyRate;					//ռ�ɱ�
	DECIMAL								decTaxOccupancyRate;			//��ˮռ�ɱ�
	DECIMAL								decRollbackRate;					//ϴ���
	DWORD								dwGameRight;						//��ϷȨ��
	BYTE								cbUserType;							//�û�����
	DWORD								dwRealUserID;						//�û�ID
};
typedef CArray<DBR_GR_ReadUserSuccess,DBR_GR_ReadUserSuccess> Array_DBR_GR_ReadUserSuccess;

//��ȡʧ��
struct DBR_GR_ReadUserError
{
	WORD								wErrorCode;						//�������
};
//д����Ϸ�ּ�¼
struct DBR_GR_WriteGameRound
{
	ULONGLONG							ullGameRoundID;					//��Ϸ�ּ�¼ID
	ULONGLONG							ullStartTime;					//��Ϸ��ʼʱ��
	BYTE								cbCalculatedFlag;				//������
	BYTE								cbValidFlag;					//��Ч���
	WORD								wServerID;						//GameProjectID
	WORD								wTableID;						//����ID
	BYTE								cbEndReason;					//��Ϸ����ԭ��
	WORD								wEndDataSize;					//��Ϸ������Ϣ��С
	BYTE								cbEndData[1536];				//��Ϸ������Ϣ
};

//д���û�����
struct DBR_GR_WriteUserChart
{
	BYTE								cbUpdateUserScoreFlag;			//�޸��û����ö��

	ULONGLONG							ullGameRoundID;					//��Ϸ�ּ�¼ID

	DWORD								dwUserID;						//�û�ID
	WORD								wChairID;						//����ID

	DECIMAL								decTotalBetScore;				//��Ͷע
	DECIMAL								decTotalWinScore;				//���ɲ�
	DECIMAL								decTotalTaxScore;				//�ܳ�ˮ

	DECIMAL								decWinScoreOccupancy_High;		//����ռ���ɲ�
	DECIMAL								decWinScoreOccupancy_Self;		//����ռ���ɲ�
	DECIMAL								decWinScoreOccupancy_Less;		//����ռ���ɲ�

	DECIMAL								decTaxScoreOccupancy_High;		//����ռ�ɳ�ˮ
	DECIMAL								decTaxScoreOccupancy_Self;		//����ռ�ɳ�ˮ
	DECIMAL								decTaxScoreOccupancy_Less;		//����ռ�ɳ�ˮ

	DECIMAL								decValidBetScore_Total;			//��ϴ�����
	DECIMAL								decValidBetScore_High;			//����ϴ�����
	DECIMAL								decValidBetScore_HighRollback;	//����ϴ��
	DECIMAL								decValidBetScore_Less;			//����ϴ�����
	DECIMAL								decValidBetScore_LessRollback;	//����ϴ��

	DECIMAL								decPaidScore_High;				//���߽���
	DECIMAL								decPaidScore_Less;				//���߽���

	WORD								wDetailBetScoreSize;			//Ͷע��ϸ��С
	BYTE								cbDetailBetScore[1536];			//Ͷע��ϸ
};
//������Ϸ�ּ�¼-������ö��
struct DBR_GR_AllocGameRoundAndCheckBetScore
{
	ULONGLONG							ullStartTime;					//��Ϸ��ʼʱ��
	WORD								wServerID;						//����ID
	WORD								wTableID;						//����ID
	DWORD								dwUserID[8];					//�û�ID	
	DECIMAL								decBetScore[8];					//���ö��
};
//������Ϸ�ּ�¼-������ö�Ƚ��
struct DBR_GR_AllocGameRoundAndCheckBetScoreResult
{
	ULONGLONG							ullGameRoundID;					//��Ϸ��ID
	WORD								wServerID;						//����ID
	WORD								wTableID;						//����ID
	DWORD								dwUserID[8];					//�û�ID	
	BYTE								bCheckScoreFlag[8];				//�����
	DECIMAL								decUserScore[8];				//���ö��
};
//������Ϸ�ּ�¼
struct DBR_GR_AllocGameRound
{
	ULONGLONG							ullStartTime;					//��Ϸ��ʼʱ��
	WORD								wServerID;						//����ID
	WORD								wTableID;						//����ID
};
//������Ϸ�ּ�¼
struct DBR_GR_AllocGameRoundResult
{
	ULONGLONG							ullGameRoundID;					//��Ϸ��ID
	WORD								wServerID;						//����ID
	WORD								wTableID;						//����ID
};
//��λͶע
struct DBR_GR_ResetBetScore
{
	WORD								wServerID;						//����ID
	WORD								wTableID;						//����ID
};
//����Ͷע
struct DBR_GR_InsertBetScore
{
	DWORD								dwUserID;						//�û�ID	
	WORD								wServerID;						//����ID
	WORD								wTableID;						//����ID
	DECIMAL								decTotalBetScore;				//���ö��
};
//�߳�����
struct DBR_GR_KickUser
{
	tagOperationUserInfo				Operator;							//������
	DWORD								dwUserID;							//�����û�ID
	BYTE								cbUserType;							//�����û�����
	TCHAR								szAccount[NAME_LEN];				//�����ʺ�
	tagOperationData_KickUser			RequestData;						//��������
};
//�û����
struct DBR_GR_UserScore
{
	DWORD								dwUserID;						//�û�ID	
	DECIMAL								decScore;						//���ö��
};
//////////////////////////////////////////////////////////////////////////

//���ݿ����湳��
class CDataBaseSink : public IDataBaseSink
{
	//���ñ���
protected:
	tagDataBaseInfo						* m_pGameUserDBInfo;			//������Ϣ
	tagDataBaseInfo						* m_pGameScoreDBInfo;			//������Ϣ
	tagGameServiceAttrib				* m_pGameServiceAttrib;			//��������
	tagGameServiceOption				* m_pGameServiceOption;			//������Ϣ

	//�ӿڱ���
protected:
	IEventService						* m_pIEventService;				//�¼�����

	//�������
protected:
	CDataBaseHelper						m_GameUserDB;					//�û����ݿ�
	CDataBaseHelper						m_GameScoreDB;					//��Ϸ���ݿ�
	CQueueServiceEvent					m_AttemperEvent;				//����֪ͨ

	//��������
public:
	//���캯��
	CDataBaseSink();
	//��������
	virtual ~CDataBaseSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual bool __cdecl Release() { if (IsValid()) delete this; return true; }
	//�Ƿ���Ч
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CDataBaseSink))?true:false; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//���Ƚӿ�
public:
	//����ģ������
	virtual bool __cdecl StartService(IUnknownEx * pIUnknownEx);
	//����ģ��ر�
	virtual bool __cdecl StopService(IUnknownEx * pIUnknownEx);
	//���ݲ�������
	virtual bool __cdecl OnDataBaseRequest(BYTE cbThreadIndex,const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);

	//���ú��� 
public:
	//WriteGameRound
	bool SetEventService(IUnknownEx * pIUnknownEx);
	//���ú���
	bool InitDataBaseSink(tagDataBaseSinkParameter * pDataBaseSinkParameter, IUnknownEx * pIUnknownEx);

	//������
private:
	//��¼����
	bool OnRequestLogon(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//��ȡ����
	bool OnRequestReadUser(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//д����Ϸ�ּ�¼����
	bool OnRequestWriteGameRound(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//д���û������¼����
	bool OnRequestWriteUserChart(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//������Ϸ�ּ�¼-������ö������
	bool OnRequestAllocGameRoundAndCheckBetScore(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//������Ϸ�ּ�¼
	bool OnRequestAllocGameRound(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//��λͶע
	bool OnRequestResetBetScore(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//����Ͷע
	bool OnRequestInsertBetScore(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//�߳�����
	bool OnRequestKickUser(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//װ�ػ�����
	bool OnRequestReadAIUser(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//�洢����
protected:
	//�ʺŴ洢����
	LONG SPLogonByAccount(LPCTSTR pszAccounts, LPCTSTR pszPassword, DWORD dwClientIP);
	//�ʺŶ�ȡ����
	LONG SPReadUser(DWORD dwUserID);
	//д��Ϸ�ּ�¼�洢����
	LONG SPWriteGameRound(DBR_GR_WriteGameRound* pWriteGameRound);
	//д�û������¼�洢����
	LONG SPWriteUserChart(DBR_GR_WriteUserChart* pWriteUserChart);
	//������Ϸ�ּ�¼-������ö�ȴ洢����
	ULONGLONG SPAllocGameRoundAndCheckBetScore(DBR_GR_AllocGameRoundAndCheckBetScore * pAllocGameRoundAndCheckBetScore);
	//������Ϸ�ּ�¼�洢����
	ULONGLONG SPAllocGameRound(DBR_GR_AllocGameRound * pAllocGameRound);
	//��λͶע�洢����
	LONG SPResetBetScore(DBR_GR_ResetBetScore * pResetBetScore);
	//����Ͷע�洢����
	LONG SPInsertBetScore(DBR_GR_InsertBetScore * pInsertBetScore);
	//д�������¼�洢����
	LONG SPInsertOperationInfo(tagOperationData& OperationData);
	//��ȡ��Ϸ�ڵ����ִ洢����
	LONG SPReadGameTreeNodeName(WORD wTypeID,
		WORD wKindID,
		WORD wServerID);
	//��ȡ�������ʺ�
	LONG SPReadAIUser(WORD wAIUserCount);

	//��������
private:
	//��ȡ�ʺ���Ϣ
	bool helper_DBGetFieldValue_UserData(CDataBaseHelper &WorkDataBase, 
		tagUserData& UserData,
		DECIMAL& decOccupancyRate,
		DECIMAL& decTaxOccupancyRate,
		DECIMAL& decRollbackRate,
		DWORD& dwGameRight,
		DWORD* pdwRealUserID = NULL,
		BYTE* pcbUserType = NULL);
	//��ȡ������Ϸ�ּ�¼-������ö�Ƚ����Ϣ
	bool helper_DBGetFieldValue_AllocGameRoundAndCheckBetScoreResult(CDataBaseHelper &WorkDataBase, 
		DBR_GR_AllocGameRoundAndCheckBetScoreResult& AllocGameRoundAndCheckBetScoreResult);
	//��ʼ��������¼����
	inline void helper_InitOperationData(tagOperationData& OperationData,DWORD dwOPType, DWORD dwIPAddress);
	//��������¼��ͷ
	inline void helper_FillOperationHeader(tagOperationData& OperationData,
		DWORD			dwUserID,
		BYTE			cbUserType,
		LPCTSTR			szAccount,
		DWORD			dwOperationUserID,
		BYTE			cbOperationUserType,
		LPCTSTR			szOperationAccount);
	//��ȡ��ǰʱ��
	inline DOUBLE helper_GetNowTime();

	//��ȡ��Ϸ�ڵ�����
	bool helper_DBGetFieldValue_ReadGameTreeNodeName(CDataBaseHelper &WorkDataBase, 
		TCHAR *szTypeName,
		TCHAR *szKindName,
		TCHAR *szServerName);
	//��ȡ���ö��
	bool helper_DBGetFieldValue_UserScore(CDataBaseHelper &WorkDataBase, 
		DBR_GR_UserScore *pUserScore);
	//��ȡAI�ʺ���Ϣ
	bool helper_DBGetFieldValue_AIUser(CDataBaseHelper &WorkDataBase,
		Array_DBR_GR_ReadUserSuccess &arrUserAgent,
		Array_DBR_GR_ReadUserSuccess &arrUserGame);
};

//////////////////////////////////////////////////////////////////////////

#endif