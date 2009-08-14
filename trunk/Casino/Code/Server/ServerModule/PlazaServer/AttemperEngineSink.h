#ifndef ATTEMPER_ENGINE_SINK_HEAD_FILE
#define ATTEMPER_ENGINE_SINK_HEAD_FILE

#pragma once

#include "Commonality.h"
#include "ServerList.h"
#include "InitParamter.h"
#include "CenterSocket.h"

//////////////////////////////////////////////////////////////////////////

//������Ϣ�ṹ
struct tagConnectItemInfo
{
	BYTE							bConnect;							//�Ƿ�����
	WORD							wRoundID;							//ѭ������
	DWORD							dwClientIP;							//���ӵ�ַ
	tagUserData						UserData;							//�û�����
	BYTE							cbUserType;							//�û�����
	DWORD							dwRealUserID;						//�û�ID
};
#define INVALID_USERTYPE (BYTE)-1

//////////////////////////////////////////////////////////////////////////

//�������湳��
class CAttemperEngineSink : public IAttemperEngineSink
{
	//��������
protected:
	CInitParamter					* m_pInitParamter;					//���ò���
	tagConnectItemInfo				* m_pConnectItemInfo;				//������Ϣ
	WORD							m_wLogonUserCount;					//��½����

	//��������
protected:
	WORD							m_wOnLineInfoSize;					//��Ϣ����
	tagOnLineCountInfo				m_OnLineCountInfo[256];				//������Ϣ

	//CenterServer
protected:
	enSocketState					m_SocketState;						//����״̬
	CCenterSocket					* m_pCenterSocket;					//��������

	//�������
protected:
	CServerList						m_ServerList;						//�������б�
	CQueueServiceEvent				m_DataBaseEvent;					//���ݿ�֪ͨ

	//�ӿڱ���
protected:
	ITimerEngine					* m_pITimerEngine;					//��ʱ���ӿ�
	IEventService					* m_pIEventService;					//�¼�����
	ITCPSocketEngine				* m_pITCPSocketEngine;				//����ӿ�
	IUpdateEventSink				* m_pIUpdateEventSink;				//�����¼��ӿ�

	//��Ϸ����
protected:
	CMD_GP_GameInfo					m_GameInfo;							//��Ϸ��Ϣ
	CMD_GP_Notice					m_ManagementNotice;					//������
	CMD_GP_Notice					m_GameNotice;						//��Ϸ����
	//��������
public:
	//���캯��
	CAttemperEngineSink();
	//��������
	virtual ~CAttemperEngineSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual bool __cdecl Release() { if (IsValid()) delete this; return true; }
	//�Ƿ���Ч
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CAttemperEngineSink))?true:false; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//���Ƚӿ�
public:
	//����ģ������
	virtual bool __cdecl StartService(IUnknownEx * pIUnknownEx);
	//����ģ��ر�
	virtual bool __cdecl StopService(IUnknownEx * pIUnknownEx);
	//�¼�����ӿ�
	virtual bool __cdecl OnAttemperEvent(BYTE cbThreadIndex,WORD wIdentifier, void * pDataBuffer, WORD wDataSize, DWORD dwInsertTime);
	//�¼��ӿ�
public:
	//��ʱ���¼�
	virtual bool __cdecl OnEventTimer(BYTE cbThreadIndex,WORD wTimerID, WPARAM wBindParam);
	//���ݿ��¼�
	virtual bool __cdecl OnEventDataBase(BYTE cbThreadIndex,void * pDataBuffer, WORD wDataSize, NTY_DataBaseEvent * pDataBaseEvent);
	//����Ӧ���¼�
	virtual bool __cdecl OnEventSocketAccept(BYTE cbThreadIndex,NTY_SocketAcceptEvent * pSocketAcceptEvent);
	//�����ȡ�¼�
	virtual bool __cdecl OnEventSocketRead(BYTE cbThreadIndex,CMD_Command Command, void * pDataBuffer, WORD wDataSize, NTY_SocketReadEvent * pSocketReadEvent);
	//����ر��¼�
	virtual bool __cdecl OnEventSocketClose(BYTE cbThreadIndex,NTY_SocketCloseEvent * pSocketCloseEvent);

	//���ú��� 
public:
	//�����¼�
	bool SetEventService(IUnknownEx * pIUnknownEx);
	//���ø����¼�����
	bool SetUpdateEventSink(IUnknownEx * pIUnknownEx);
	//���ú���
	bool InitAttemperSink(CInitParamter * pInitParamter, CCenterSocket * pCenterSocket, IUnknownEx * pIUnknownEx);

	//��������
public:
	//ö���û�
	void EnumConnectUser(IUnknownEx * pIUnknownEx);
	//�ж��Ƿ�����������
	bool IsConnectedCenter();

	//���������¼�
public:
	//���������¼�
	virtual bool __cdecl OnEventCenterSocketConnect(int iErrorCode);
	//���Ķ�ȡ�¼�
	virtual bool __cdecl OnEventCenterSocketRead(CMD_Command Command, void * pDataBuffer, WORD wDataSize);
	//���Ĺر��¼�
	virtual bool __cdecl OnEventCenterSocketClose(bool bCloseByServer);

	//���ݿ⺯��
private:
	//�û���¼�ɹ�
	bool OnDBLogonSuccess(void * pDataBuffer, WORD wDataSize, NTY_DataBaseEvent * pDataBaseEvent);
	//�û���¼ʧ��
	bool OnDBLogonError(void * pDataBuffer, WORD wDataSize, NTY_DataBaseEvent * pDataBaseEvent);
	//���������ʺ����
	bool OnDBGetLessUserListComplete(void * pDataBuffer, WORD wDataSize, NTY_DataBaseEvent * pDataBaseEvent);
	//������Ϸ����Ϣ
	bool OnDBUpdateGameMainInfoSuccess(void * pDataBuffer, WORD wDataSize, NTY_DataBaseEvent * pDataBaseEvent);

	//���纯��
private:
	//��¼��Ϣ����
	bool OnSocketMainLogon(CMD_Command Command, void * pDataBuffer, WORD wDataSize, NTY_SocketReadEvent * pSocketReadEvent);
	//�༭�ʺŴ���
	bool OnSocketMainEditUser(CMD_Command Command, void * pDataBuffer, WORD wDataSize, NTY_SocketReadEvent * pSocketReadEvent);
	//�༭���ʺŴ���
	bool OnSocketMainEditSubUser(CMD_Command Command, void * pDataBuffer, WORD wDataSize, NTY_SocketReadEvent * pSocketReadEvent);
	//��ȡ�ʺŴ���
	bool OnSocketMainGetUser(CMD_Command Command, void * pDataBuffer, WORD wDataSize, NTY_SocketReadEvent * pSocketReadEvent);
	//��ȡ��¼
	bool OnSocketMainGetRecord(CMD_Command Command, void * pDataBuffer, WORD wDataSize, NTY_SocketReadEvent * pSocketReadEvent);

	//��������
private:
	//�б���Ϣ����
	bool OnCenterMainServerList(CMD_Command Command, void * pDataBuffer, WORD wDataSize);
	//������Ϣ��Ϣ����
	bool OnCenterMainUpdateInfo(CMD_Command Command, void * pDataBuffer, WORD wDataSize);
	//������Ϣ����
	bool OnCenterMainManagement(CMD_Command Command, void * pDataBuffer, WORD wDataSize);

	//��������
private:
	//����ʺ���Ϣ
	int SimpleCheck_CMD_GP_RegisterUser(CMD_GP_RegisterUser * pRegisterUser);
	//����ʺŻ�����Ϣ
	int SimpleCheck_CMD_GP_BasicUserInfo(CMD_GP_BasicUserInfo * pBasicUserInfo);
	//����ʺ���ϸ��Ϣ
	int SimpleCheck_CMD_GP_DetailUserInfo(CMD_GP_DetailUserInfo * pDetailUserInfo);
	//������ʺ���Ϣ
	int SimpleCheck_CMD_GP_RegisterSubUser(CMD_GP_RegisterSubUser * pRegisterSubUserInfo);
	//������ʺŻ�����Ϣ
	int SimpleCheck_CMD_GP_BasicSubUserInfo(CMD_GP_BasicSubUserInfo * pBasicSubUserInfo);
	//��������ʺŻ�����Ϣ
	int SimpleCheck_CMD_GP_MySelfBasicUserInfo(CMD_GP_MySelfBasicUserInfo * pBasicUserInfo);
	//����ȡ������¼��Ϣ
	int SimpleCheck_CMD_GP_GetOperation(CMD_GP_GetOperation * pGetOperation);
	//����ȡ�����¼��ʽ-1��Ϣ
	int SimpleCheck_CMD_GP_GetChartViewOne(CMD_GP_GetChartViewOne * pGetChartViewOne);
	//����ȡ�����¼��ʽ-2��Ϣ
	int SimpleCheck_CMD_GP_GetChartViewTwo(CMD_GP_GetChartViewTwo * pGetChartViewTwo);
	//����ȡ�����¼��ʽ-3��Ϣ
	int SimpleCheck_CMD_GP_GetChartViewThree(CMD_GP_GetChartViewThree * pGetChartViewThree);
	//����ȡ��Ϸ��¼��Ϣ
	int SimpleCheck_CMD_GP_GetBetHistory(CMD_GP_GetBetHistory * pGetBetHistory);
	//����û�����Ȩ��
	bool SimpleCheck_UserRight(BYTE& cbUserType,tagUserData& UserData);
};

//////////////////////////////////////////////////////////////////////////

#endif