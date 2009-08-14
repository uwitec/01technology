#ifndef SERVER_LIST_HEAD_FILE
#define SERVER_LIST_HEAD_FILE

#pragma once

#include "Commonality.h"

//////////////////////////////////////////////////////////////////////////

//���鶨��
typedef CArrayTemplate<tagGameType *> CGameTypePtrArray;
typedef CArrayTemplate<tagGameKind *> CGameKindPtrArray;
typedef CArrayTemplate<tagGameServer *> CGameServerPtrArray;
typedef CArrayTemplate<tagGameStation *> CGameStationPtrArray;

//////////////////////////////////////////////////////////////////////////

//�б���Ϣ
class CServerListInfo
{
	//��������
public:
	CGameTypePtrArray				m_GameTypePtrArrayActive;			//��������
	CGameTypePtrArray				m_GameTypePtrArrayBuffer;			//��������
	CGameKindPtrArray				m_GameKindPtrArrayActive;			//��������
	CGameKindPtrArray				m_GameKindPtrArrayBuffer;			//��������
	CGameServerPtrArray				m_GameServerPtrArrayActive;			//��������
	CGameServerPtrArray				m_GameServerPtrArrayBuffer;			//��������
	CGameStationPtrArray			m_GameStationPtrArrayActive;		//վ������
	CGameStationPtrArray			m_GameStationPtrArrayBuffer;		//վ������

	//��������
public:
	//���캯��
	CServerListInfo();
	//��������
	virtual ~CServerListInfo();

	//ά������
public:
	//������Ϣ
	bool ResetInfo();
	//��������
	bool InsertGameType(tagGameType * pGameType);
	//��������
	bool InsertGameKind(tagGameKind * pGameKind);
	//����վ��
	bool InsertGameStation(tagGameStation * pGameStation);
	//���뷿��
	bool InsertGameServer(tagGameServer * pGameServer);
	//������Ϣ
	bool CopyServerListInfo(CServerListInfo & ServerListInfoScr);
};

//////////////////////////////////////////////////////////////////////////

//�б����
class CServerList
{
	//��������
protected:
	CServerListInfo					m_ServerListInfoBuffer;				//�б���Ϣ
	CServerListInfo					m_ServerListInfoActive;				//�б���Ϣ
	BYTE							m_cbSendBuffer[SOCKET_PACKAGE];		//���ͻ���

	//�ӿ�ָ��
protected:
	IEventService					* m_pIEventService;					//�¼��ӿ�
	ITCPSocketEngine				* m_pITCPSocketEngine;				//��������

public:
	//���캯��
	CServerList();
	//��������
	virtual ~CServerList();

	//���ýӿ�
public:
	//�����¼�
	virtual bool __cdecl SetEventService(IUnknownEx * pIUnknownEx);
	//���ýӿ�
	virtual bool __cdecl SetSocketEngine(IUnknownEx * pIUnknownEx);

	//���ͽӿ�
public:
	//��������
	virtual bool __cdecl SendGameTypeList(WORD wIndex, WORD wRountID);
	//��������
	virtual bool __cdecl SendGameKindList(WORD wIndex, WORD wRountID);
	//����վ��
	virtual bool __cdecl SendGameStationList(WORD wIndex, WORD wRountID);
	//����վ��
	virtual bool __cdecl SendGameStationList(WORD wIndex, WORD wRountID, WORD wKindID);
	//���ͷ���
	virtual bool __cdecl SendGameServerList(WORD wIndex, WORD wRountID);
	//���ͷ���
	virtual bool __cdecl SendGameServerList(WORD wIndex, WORD wRountID, WORD wKindID);

	//���½ӿ�
public:
	//����׼��
	virtual bool __cdecl ResetServerListBuffer();
	//�����
	virtual bool __cdecl ActiveServerListBuffer();
	//׷������
	virtual bool __cdecl AppendGameTypeBuffer(tagGameType * pGameType, DWORD dwCount);
	//׷������
	virtual bool __cdecl AppendGameKindBuffer(tagGameKind * pGameKind, DWORD dwCount);
	//׷��վ��
	virtual bool __cdecl AppendGameStationBuffer(tagGameStation * pGameStation, DWORD dwCount);
	//׷�ӷ���
	virtual bool __cdecl AppendGameServerBuffer(tagGameServer * pGameServer, DWORD dwCount);
	//���ܽӿ�
public:
	//��ȡ����
	virtual WORD __cdecl GetOnLineCountInfo(tagOnLineCountInfo OnLineCountInfo[], WORD wBufferCount);
};

//////////////////////////////////////////////////////////////////////////

#endif