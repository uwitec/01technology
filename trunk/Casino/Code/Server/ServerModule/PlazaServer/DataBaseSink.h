#ifndef DATA_BASE_SINK_HEAD_FILE
#define DATA_BASE_SINK_HEAD_FILE

#pragma once
#include <afxtempl.h>
#include "Afxinet.h"
#include "Commonality.h"
#include "InitParamter.h"

//////////////////////////////////////////////////////////////////////////
typedef CArray<tagUserData,tagUserData> tagUserDataArray;
typedef CArray<tagSubUserData,tagSubUserData> tagSubUserDataArray;
typedef CArray<tagOperationData,tagOperationData> tagOperationDataArray;
typedef CArray<tagChartViewOneData,tagChartViewOneData> tagChartViewOneDataArray;
typedef CArray<tagChartViewTwoData,tagChartViewTwoData> tagChartViewTwoDataArray;
typedef CArray<tagBetHistoryData,tagBetHistoryData> tagBetHistoryDataArray;

//���ݿ����湳��
class CDataBaseSink : public IDataBaseSink
{
	//��������
protected:
	CInitParamter					* m_pInitParamter;					//���ò���
	IEventService					* m_pIEventService;					//�¼�����
	DWORD							  m_dwLogonServerAddr;
	//�������
protected:
	CDataBaseHelper					m_WorkDataBase;						//�û����ݿ�
	CQueueServiceEvent				m_AttemperEvent;					//����֪ͨ
	ITCPSocketEngine				* m_pITCPSocketEngine;				//��������

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
	//ģ������
	virtual bool __cdecl StartService(IUnknownEx * pIUnknownEx);
	//ģ��ر�
	virtual bool __cdecl StopService(IUnknownEx * pIUnknownEx);
	//���ݲ�������
	virtual bool __cdecl OnDataBaseRequest(BYTE cbThreadIndex,const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);

	//���ú��� 
public:
	//�����¼�
	bool SetEventService(IUnknownEx * pIUnknownEx);
	//���ýӿ�
	bool SetSocketEngine(IUnknownEx * pIUnknownEx);
	//���ú���
	bool InitDataBaseSink(CInitParamter * pInitParamter, IUnknownEx * pIUnknownEx);

	//������
private:
	//��¼������
	bool OnRequestLogon(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//ע��������
	bool OnRequestRegister(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//��ȡ���������
	bool OnRequestSetUserScore(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//״̬�趨������
	bool OnRequestSetUserCongealState(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//������Ϣ�趨������
	bool OnRequestSetBasicUserInfo(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//��ϸ��Ϣ�趨������
	bool OnRequestSetDetailUserInfo(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//Ȩ���趨������
	bool OnRequestSetUserRight(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//���������Ϣ�趨������
	bool OnRequestSetMySelfBasicUserInfo(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//���ʺ�״̬�趨������
	bool OnRequestSetSubUserCongealState(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//ע�����ʺ�������
	bool OnRequestRegisterSubUser(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//���ʺŻ�����Ϣ�趨������
	bool OnRequestSetBasicSubUserInfo(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//��ȡ���ߴ���
	bool OnRequestGetLessUserList(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//��ȡ���ʺŴ���
	bool OnRequestGetSubUserList(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//��ȡ������¼
	bool OnRequestGetOperationList(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//��ȡ�����¼��ʽ-1
	bool OnRequestGetChartViewOneList(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//��ȡ�����¼��ʽ-2
	bool OnRequestGetChartViewTwoList(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//��ȡ��Ϸ�ּ�¼
	bool OnRequestGetBetHistoryList(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//��ȡ��Ϸ�ּ�¼��ϸͶע
	bool OnRequestGetDetailBetScore(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//��ȡ�����¼��ʽ-3
	bool OnRequestGetChartViewThree(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	
	//������Ϸ��Ϣ
	bool OnRequestUpdateGameMainInfo(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);
	//ע���ʺŵ�½
	bool OnRequestLogout(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);

	//�洢����
private:
	//�ʺŴ洢����
	LONG SPLogonByAccount(LPCTSTR pszAccounts,
		LPCTSTR pszPassword, DWORD dwClientIP,
		DWORD dwLogonServerAddr, 
		WORD wListenPort);

	//ע��洢����
	LONG SPRegisterUser(tagUserData* pUserData, 
		LPCTSTR pszPassword, DWORD dwClientIP);

	//��ȡ���ߴ洢����
	LONG SPGetLessUserList(DWORD	dwParentUserID);

	//��ȡ���ʺŴ洢����
	LONG SPGetSubUserList(DWORD	dwParentUserID);

	//��ȡ��ȴ洢����
	LONG SPSetUserScore(DWORD dwParentUserID,DWORD dwUserID,
		DOUBLE fScore);

	//״̬�趨�洢����
	LONG SPSetUserCongealState(DWORD dwUserID,
		BYTE cbLevel, BYTE cbState, DWORD dwStateMark);

	//������Ϣ�趨�洢����
	LONG SPSetBaiscUserInfo(DWORD dwUserID,
		LPCTSTR pszName,LPCTSTR pszPassword,BYTE cbFaceID);

	//��ϸ��Ϣ�趨�洢����
	LONG SPSetDetailUserInfo(DBR_GP_SetDetailUserInfo * pSetDetailUserInfo);

	//Ȩ���趨�洢����
	LONG SPSetUserRight(DBR_GP_SetUserRight * pSetUserRight);

	//���������Ϣ�趨�洢����
	LONG SPSetMySelfBaiscUserInfo(
		BYTE  cbUserType,
		DWORD dwUserID,
		LPCTSTR pszName,
		LPCTSTR pszPassword,
		LPCTSTR pszNewPassword,
		BYTE cbFaceID);

	//���ʺ�״̬�趨�洢����
	LONG SPSetSubUserCongealState(DWORD dwUserID,
		BYTE cbState, DWORD dwStateMark);

	//ע�����ʺŴ洢����
	LONG SPRegisterSubUser(DWORD dwParentUserID,
		LPCTSTR pszAccount,LPCTSTR pszName,LPCTSTR pszPassword);

	//���ʺŻ�����Ϣ�趨�洢����
	LONG SPSetBaiscSubUserInfo(DWORD dwUserID,
		LPCTSTR pszName,LPCTSTR pszPassword);

	//д�������¼�洢����
	LONG SPInsertOperationInfo(tagOperationData& OperationData);

	//��ѯ������¼�洢����
	LONG SPGetOperationList(DBR_GP_GetOperation * pGetOperation);

	//��ѯ�����¼��ʽ-1�洢����
	LONG SPGetChartViewOneList(DBR_GP_GetChartViewOne * pGetChartViewOne);

	//��ѯ�����¼��ʽ-2�洢����
	LONG SPGetChartViewTwoList(DBR_GP_GetChartViewTwo * pGetChartViewTwo);

	//��ѯ��Ϸ�ּ�¼�洢����
	LONG SPGetBetHistoryList(DBR_GP_GetBetHistory * pGetBetHistory);

	//��ѯ��Ϸ�ּ�¼��ϸͶע�洢����
	LONG SPGetDetailBetScore(DBR_GP_GetDetailBetScore * pGetDetailBetScore);

	//��ѯ�����¼��ʽ-3�洢����
	LONG SPGetChartViewThree(DBR_GP_GetChartViewThree * pGetChartViewThree);

	//��ѯ��Ϸ����Ϣ�洢����
	LONG SPLoadGameMainInfo();

	//ע���ʺŵ�½�洢����
	LONG SPLogoutUser(DWORD dwLogonServerAddr,
		WORD wListenPort,DBR_GP_LogoutUser * pLogoutUser);

	//��������
private:
	//��ȡ�ʺ���Ϣ
	inline bool helper_DBGetFieldValue_UserData(CDataBaseHelper &WorkDataBase, 
		tagUserData& UserData,DWORD* pdwRealUserID = NULL,BYTE* pcbUserType = NULL);
	//��ȡ���ʺ���Ϣ
	inline bool helper_DBGetFieldValue_SubUserData(CDataBaseHelper &WorkDataBase, tagSubUserData& UserData);
	//��ȡ�ʺ��б���Ϣ
	inline bool helper_DBGetFieldValue_UserDataArray(CDataBaseHelper &WorkDataBase, tagUserDataArray& UserDataArray);
	//��ȡ���ʺ��б���Ϣ
	inline bool helper_DBGetFieldValue_SubUserDataArray(CDataBaseHelper &WorkDataBase, tagSubUserDataArray& UserDataArray);
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
	//ת���ַ�����
	inline void helper_ConvertACPToUtf8_OperationData(tagOperationData* pOperationData);
	//ת���ַ�����
	inline void helper_ConvertACPToUtf8_BetHistoryData(tagBetHistoryData* pBetHistoryData);
	//��ȡ������¼��Ϣ
	inline bool helper_DBGetFieldValue_OperationData(CDataBaseHelper &WorkDataBase, tagOperationData& OperationData);
	//��ȡ������¼�б���Ϣ
	inline bool helper_DBGetFieldValue_OperationDataArray(CDataBaseHelper &WorkDataBase, tagOperationDataArray& OperationDataArray);
	//��ȡ�����¼��ʽ-1��Ϣ
	inline bool helper_DBGetFieldValue_ChartViewOneData(CDataBaseHelper &WorkDataBase, tagChartViewOneData& ChartViewOneData);
	//��ȡ�����¼��ʽ-1�б���Ϣ
	inline bool helper_DBGetFieldValue_ChartViewOneDataArray(CDataBaseHelper &WorkDataBase, tagChartViewOneDataArray& ChartViewOneDataArray);
	//��ȡ�����¼��ʽ-1�б�������Ϣ
	inline bool helper_DBGetFieldValue_ChartViewOneListConfig(CDataBaseHelper &WorkDataBase, CMD_GP_ChartViewOneListConfig* pConfig);
	//��ȡ�����¼��ʽ-2��Ϣ
	inline bool helper_DBGetFieldValue_ChartViewTwoData(CDataBaseHelper &WorkDataBase, tagChartViewTwoData& ChartViewTwoData);
	//��ȡ�����¼��ʽ-2�б���Ϣ
	inline bool helper_DBGetFieldValue_ChartViewTwoDataArray(CDataBaseHelper &WorkDataBase, tagChartViewTwoDataArray& ChartViewTwoDataArray);
	//��ȡ�ּ�¼��Ϣ
	inline bool helper_DBGetFieldValue_BetHistoryData(CDataBaseHelper &WorkDataBase, tagBetHistoryData& BetHistoryData);
	//��ȡ��Ϸ�ּ�¼��ϸͶע
	inline bool helper_DBGetFieldValue_DetailBetScoreSuccess(CDataBaseHelper &WorkDataBase, CMD_GP_GetDetailBetScoreSuccess *pGetDetailBetScoreSuccess);
	//��ȡ�ּ�¼�б���Ϣ
	inline bool helper_DBGetFieldValue_BetHistoryDataArray(CDataBaseHelper &WorkDataBase, tagBetHistoryDataArray& BetHistoryDataArray);
	//��ȡ��Ϸ����Ϣ
	inline bool helper_DBGetFieldValue_GameMainInfo(CDataBaseHelper &WorkDataBase,DBR_Update_GameMainInfo &GameMainInfo);
	//��ȡ�����¼��ʽ-3��Ϣ
	inline bool helper_DBGetFieldValue_ChartViewThreeSucess(CDataBaseHelper &WorkDataBase, CMD_GP_ChartViewThreeSucess* pChartViewThreeSucess);
	//���ͻ��˵�½
	inline LONG helper_AllowClientLogon(BYTE cbClientType,BYTE cbUserType,BYTE cbUserLevel);

};

//////////////////////////////////////////////////////////////////////////

#endif