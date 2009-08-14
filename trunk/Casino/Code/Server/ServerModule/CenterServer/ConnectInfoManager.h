#ifndef CONNECT_INFO_MANAGER_HEAD_FILE
#define CONNECT_INFO_MANAGER_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//��������
enum enConnectKind
{
	ConnectKind_Unknow,				//δ֪����
	ConnectKind_GamePlaza,			//�㳡����
	ConnectKind_GameServer,			//��������
};

//////////////////////////////////////////////////////////////////////////

//������Ϣ�ṹ
class CConnectItem
{
	//��������
public:
	WORD							m_wRoundID;							//ѭ������
	DWORD							m_dwClientIP;						//���ӵ�ַ
	const enConnectKind				m_ConnectKind;						//��������

	//��������
public:
	CConnectItem(enConnectKind ConnectKind);
	virtual ~CConnectItem();
};

//////////////////////////////////////////////////////////////////////////

//�㳡������Ϣ
class CConnectItem_Plaza : public CConnectItem
{
	//��������
public:
	CConnectItem_Plaza();
	virtual ~CConnectItem_Plaza();
};

//////////////////////////////////////////////////////////////////////////

//����������Ϣ
class CConnectItem_GameServer : public CConnectItem
{
	//��������
public:
	WORD							m_wKindID;							//���ͱ�ʶ
	WORD							m_wServerID;						//�����ʶ

	//��������
public:
	CConnectItem_GameServer();
	virtual ~CConnectItem_GameServer();
};

//////////////////////////////////////////////////////////////////////////

//������Ϣ����
class CConnectInfoManager
{
	//��������
public:
	CConnectInfoManager();
	virtual ~CConnectInfoManager();
};

//////////////////////////////////////////////////////////////////////////

#endif
