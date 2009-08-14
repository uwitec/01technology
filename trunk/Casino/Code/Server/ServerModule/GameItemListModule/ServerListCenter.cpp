#include "StdAfx.h"
#include "ServerListCenter.h"

//////////////////////////////////////////////////////////////////////////

//���캯��
CServerListCenter::CServerListCenter(void)
{
	//��ʼ��
	m_TypeItemMap.InitHashTable(GetMaxPrime(20));
	m_KindItemMap.InitHashTable(GetMaxPrime(50));
	m_ServerItemMap.InitHashTable(GetMaxPrime(500));
	m_StationItemMap.InitHashTable(GetMaxPrime(200));

	//�ӿ�ָ��
	m_pIEventService=NULL;

	return;
}

//��������
CServerListCenter::~CServerListCenter(void)
{
	//ɾ������
	tagGameTypeItem * pGameTypeItem=NULL;
	for (INT_PTR i=0;i<m_TypeItemStore.GetCount();i++)
	{
		pGameTypeItem=m_TypeItemStore[i];
		SafeDelete(pGameTypeItem);
	}
	m_TypeItemStore.RemoveAll();

	//ɾ������
	tagGameKindItem * pGameKindItem=NULL;
	for (INT_PTR i=0;i<m_KindItemStore.GetCount();i++)
	{
		pGameKindItem=m_KindItemStore[i];
		SafeDelete(pGameKindItem);
	}
	m_KindItemStore.RemoveAll();

	//ɾ������
	tagGameServerItem * pGameServerItem=NULL;
	for (INT_PTR i=0;i<m_ServerItemStore.GetCount();i++)
	{
		pGameServerItem=m_ServerItemStore[i];
		SafeDelete(pGameServerItem);
	}
	m_ServerItemStore.RemoveAll();

	//ɾ��վ��
	tagGameStationItem * pGameStationItem=NULL;
	for (INT_PTR i=0;i<m_StationItemStore.GetCount();i++)
	{
		pGameStationItem=m_StationItemStore[i];
		SafeDelete(pGameStationItem);
	}
	m_StationItemStore.RemoveAll();

	return;
}

//�ӿڲ�ѯ
void * __cdecl CServerListCenter::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IServerListCenter,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IServerListCenter,Guid,dwQueryVer);
	return NULL;
}

//�����¼�
bool __cdecl CServerListCenter::SetEventService(IUnknownEx * pIUnknownEx)
{
	GT_ASSERT(pIUnknownEx!=NULL);
	m_pIEventService=GET_OBJECTPTR_INTERFACE(pIUnknownEx,IEventService);
	GT_ASSERT(m_pIEventService!=NULL);
	return (m_pIEventService!=NULL);
}

//��������
bool __cdecl CServerListCenter::InsertGameType(tagGameType * pGameType)
{
	//Ч�����
	GT_ASSERT(pGameType!=NULL);
	if (pGameType==NULL) return false;

	//�����ִ�
	tagGameTypeItem * pGameTypeItem=NULL;
	if (m_TypeItemMap.Lookup(pGameType->wTypeID,pGameTypeItem)==TRUE) return false;

	_BEGIN_TRY
	{
		//��������
		INT_PTR nStroeCount=m_TypeItemStore.GetCount();
		if (nStroeCount>0)
		{
			pGameTypeItem=m_TypeItemStore[nStroeCount-1];
			m_TypeItemStore.RemoveAt(nStroeCount-1);
		}
		else
		{
			pGameTypeItem=new tagGameTypeItem;
			if (pGameTypeItem==NULL) return false;
		}

		//��������
		CopyMemory(&pGameTypeItem->GameType,pGameType,sizeof(tagGameType));
		m_TypeItemMap[pGameType->wTypeID]=pGameTypeItem;

		return true;
	}
	CATCH_COMMON_EXCEPTION(;)CATCH_UNKNOWN_EXCEPTION(;)_END_CATCH

	return false;
}

//��������
bool __cdecl CServerListCenter::InsertGameKind(tagGameKind * pGameKind)
{
	//Ч�����
	GT_ASSERT(pGameKind!=NULL);
	if (pGameKind==NULL) return false;

	//�����ִ�
	tagGameKindItem * pGameKindItem=NULL;
	if (m_KindItemMap.Lookup(pGameKind->wKindID,pGameKindItem)==TRUE) return false;

	_BEGIN_TRY
	{
		//��������
		INT_PTR nStroeCount=m_KindItemStore.GetCount();
		if (nStroeCount>0)
		{
			pGameKindItem=m_KindItemStore[nStroeCount-1];
			m_KindItemStore.RemoveAt(nStroeCount-1);
		}
		else
		{
			pGameKindItem=new tagGameKindItem;
			if (pGameKindItem==NULL) return false;
		}

		//��������
		memset(pGameKindItem,0,sizeof(pGameKindItem));
		CopyMemory(&pGameKindItem->GameKind,pGameKind,sizeof(tagGameKind));
		m_KindItemMap[pGameKind->wKindID]=pGameKindItem;

		return true;
	}
	CATCH_COMMON_EXCEPTION(;)CATCH_UNKNOWN_EXCEPTION(;)_END_CATCH

	return false;
}

//���뷿��
bool __cdecl CServerListCenter::InsertGameServer(tagGameServer * pGameServer,
												 WORD wIndex,WORD wRoundID)
{
	//Ч�����
	GT_ASSERT(pGameServer!=NULL);
	if (pGameServer==NULL) return false;

	//�����ִ�
	tagGameServerItem * pGameServerItem=NULL;;
	DWORD dwKey=MAKELONG(pGameServer->wKindID,pGameServer->wServerID);
	if (m_ServerItemMap.Lookup(dwKey,pGameServerItem)==TRUE) return false;

	_BEGIN_TRY
	{
		//��������
		INT_PTR nStroeCount=m_ServerItemStore.GetCount();
		if (nStroeCount>0)
		{
			pGameServerItem=m_ServerItemStore[nStroeCount-1];
			m_ServerItemStore.RemoveAt(nStroeCount-1);
		}
		else
		{
			pGameServerItem=new tagGameServerItem;
			if (pGameServerItem==NULL) return false;
		}

		//��������
		pGameServerItem->dwUpdateTime=(DWORD)time(NULL);
		pGameServerItem->wIndex = wIndex;
		pGameServerItem->wRoundID = wRoundID;
		CopyMemory(&pGameServerItem->GameServer,pGameServer,sizeof(tagGameServer));
		m_ServerItemMap[dwKey]=pGameServerItem;

		if (m_pIEventService!=NULL) 
		{
			TCHAR szBuffer[256]=TEXT("");
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("��%s  KindID��%ld StationID��%ld  ServerID��%ld��ע��ɹ�"),
				pGameServer->szServerName,pGameServer->wKindID,pGameServer->wStationID,pGameServer->wServerID);
			m_pIEventService->ShowEventNotify(szBuffer,Level_Debug);
		}

		//��������
		UpdateKindOnLineCount(pGameServer->wKindID);

		return true;
	}
	CATCH_COMMON_EXCEPTION(;)CATCH_UNKNOWN_EXCEPTION(;)_END_CATCH

	return false;
}

//����վ��
bool __cdecl CServerListCenter::InsertGameStation(tagGameStation * pGameStation)
{
	//Ч�����
	GT_ASSERT(pGameStation!=NULL);
	if (pGameStation==NULL) return false;

	//�����ִ�
	tagGameStationItem * pGameStationItem=NULL;
	DWORD dwKey=MAKELONG(pGameStation->wKindID,pGameStation->wStationID);
	if (m_StationItemMap.Lookup(dwKey,pGameStationItem)==TRUE) return false;

	_BEGIN_TRY
	{
		//��������
		INT_PTR nStroeCount=m_StationItemStore.GetCount();
		if (nStroeCount>0)
		{
			pGameStationItem=m_StationItemStore[nStroeCount-1];
			m_StationItemStore.RemoveAt(nStroeCount-1);
		}
		else
		{
			pGameStationItem=new tagGameStationItem;
			if (pGameStationItem==NULL) return false;
		}

		//��������
		CopyMemory(&pGameStationItem->GameStation,pGameStation,sizeof(tagGameStation));
		m_StationItemMap[dwKey]=pGameStationItem;

		return true;
	}
	CATCH_COMMON_EXCEPTION(;)CATCH_UNKNOWN_EXCEPTION(;)_END_CATCH

	return false;
}

//ɾ������
bool __cdecl CServerListCenter::DeleteGameType(WORD wTypeID)
{
	//��������
	WORD wKey=0;
	tagGameTypeItem * pGameTypeItem=NULL;
	if (m_TypeItemMap.Lookup(wKey,pGameTypeItem)==FALSE) return false;

	//ɾ������
	m_TypeItemMap.RemoveKey(wKey);
	m_TypeItemStore.Add(pGameTypeItem);

	return true;
}

//ɾ������
bool __cdecl CServerListCenter::DeleteGameKind(WORD wKindID)
{
	//��������
	WORD wKey=0;
	tagGameKindItem * pGameKindItem=NULL;
	if (m_KindItemMap.Lookup(wKey,pGameKindItem)==FALSE) return false;

	//ɾ������
	m_KindItemMap.RemoveKey(wKey);
	m_KindItemStore.Add(pGameKindItem);

	return true;
}

//ɾ������
bool __cdecl CServerListCenter::DeleteGameServer(WORD wKindID, WORD wServerID)
{
	//���ҷ���
	tagGameServerItem * pGameServerItem=NULL;
	DWORD dwKey=MAKELONG(wKindID,wServerID);
	if (m_ServerItemMap.Lookup(dwKey,pGameServerItem)==FALSE) return false;

	//ɾ������
	m_ServerItemMap.RemoveKey(dwKey);
	m_ServerItemStore.Add(pGameServerItem);

	//��������
	UpdateKindOnLineCount(wKindID);

	return false;
}

//ɾ��վ��
bool __cdecl CServerListCenter::DeleteGameStation(WORD wKindID, WORD wStationID)
{
	//����վ��
	tagGameStationItem * pGameStationItem=NULL;
	DWORD dwKey=MAKELONG(wKindID,wStationID);
	if (m_StationItemMap.Lookup(dwKey,pGameStationItem)==FALSE) return false;

	//ɾ������
	m_StationItemMap.RemoveKey(dwKey);
	m_StationItemStore.Add(pGameStationItem);

	return false;
}

//ö������
tagGameTypeItem * const __cdecl CServerListCenter::EmunGameTypeItem(POSITION & Pos)
{
	//��������
	if (Pos==NULL) Pos=m_TypeItemMap.GetStartPosition();
	if (Pos==NULL) return NULL;

	//���ҷ���
	WORD wKey=0;
	tagGameTypeItem * pGameTypeItem=NULL;
	m_TypeItemMap.GetNextAssoc(Pos,wKey,pGameTypeItem);

	return pGameTypeItem;
}

//ö������
tagGameKindItem * const __cdecl CServerListCenter::EmunGameKindItem(POSITION & Pos)
{
	//��������
	if (Pos==NULL) Pos=m_KindItemMap.GetStartPosition();
	if (Pos==NULL) return NULL;

	//���ҷ���
	WORD wKey=0;
	tagGameKindItem * pGameKindItem=NULL;
	m_KindItemMap.GetNextAssoc(Pos,wKey,pGameKindItem);

	return pGameKindItem;
}

//ö�ٷ���
tagGameServerItem * const __cdecl CServerListCenter::EmunGameServerItem(POSITION & Pos)
{
	//��������
	if (Pos==NULL) Pos=m_ServerItemMap.GetStartPosition();
	if (Pos==NULL) return NULL;

	//���ҷ���
	DWORD dwKey=0;
	tagGameServerItem * pGameServerItem=NULL;
	m_ServerItemMap.GetNextAssoc(Pos,dwKey,pGameServerItem);

	return pGameServerItem;
}

//ö��վ��
tagGameStationItem * const __cdecl CServerListCenter::EmunGameStationItem(POSITION & Pos)
{
	//��������
	if (Pos==NULL) Pos=m_StationItemMap.GetStartPosition();
	if (Pos==NULL) return NULL;

	//����վ��
	DWORD dwKey=0;
	tagGameStationItem * pGameStationItem=NULL;
	m_StationItemMap.GetNextAssoc(Pos,dwKey,pGameStationItem);

	return pGameStationItem;
}

//��������
tagGameTypeItem * const __cdecl CServerListCenter::SearchGameType(WORD wTypeID)
{
	tagGameTypeItem * pGameTypeItem=NULL;
	m_TypeItemMap.Lookup(wTypeID,pGameTypeItem);
	return pGameTypeItem;
}

//��������
tagGameKindItem * const __cdecl CServerListCenter::SearchGameKind(WORD wKindID)
{
	tagGameKindItem * pGameKindItem=NULL;
	m_KindItemMap.Lookup(wKindID,pGameKindItem);
	return pGameKindItem;
}

//���ҷ���
tagGameServerItem * const __cdecl CServerListCenter::SearchGameServer(WORD wKindID, WORD wServerID)
{
	tagGameServerItem * pGameServerItem=NULL;
	m_ServerItemMap.Lookup(MAKELONG(wKindID,wServerID),pGameServerItem);
	return pGameServerItem;
}

//����վ��
tagGameStationItem * const __cdecl CServerListCenter::SearchGameStation(WORD wKindID, WORD wStationID)
{
	tagGameStationItem * pGameStationItem=NULL;
	m_StationItemMap.Lookup(MAKELONG(wKindID,wStationID),pGameStationItem);
	return pGameStationItem;
}

//�����б�
void __cdecl CServerListCenter::ResetServerList()
{
	//ɾ������
	tagGameTypeItem * pGameTypeItem=NULL;
	POSITION Pos=m_TypeItemMap.GetStartPosition();
	while (Pos!=NULL)
	{
		WORD wKey=0;
		m_TypeItemMap.GetNextAssoc(Pos,wKey,pGameTypeItem);
		m_TypeItemStore.Add(pGameTypeItem);
	}
	m_TypeItemMap.RemoveAll();

	//ɾ������
	tagGameKindItem * pGameKindItem=NULL;
	Pos=m_KindItemMap.GetStartPosition();
	while (Pos!=NULL)
	{
		WORD wKey=0;
		m_KindItemMap.GetNextAssoc(Pos,wKey,pGameKindItem);
		m_KindItemStore.Add(pGameKindItem);
	}
	m_KindItemMap.RemoveAll();

	//ɾ������
	Pos=m_ServerItemMap.GetStartPosition();
	tagGameServerItem * pGameServerItem=NULL;
	while (Pos!=NULL)
	{
		DWORD dwKey=0;
		m_ServerItemMap.GetNextAssoc(Pos,dwKey,pGameServerItem);
		m_ServerItemStore.Add(pGameServerItem);
	}
	m_ServerItemMap.RemoveAll();

	//ɾ��վ��
	Pos=m_StationItemMap.GetStartPosition();
	tagGameStationItem * pGameStationItem=NULL;
	while (Pos!=NULL)
	{
		DWORD dwKey=0;
		m_StationItemMap.GetNextAssoc(Pos,dwKey,pGameStationItem);
		m_StationItemStore.Add(pGameStationItem);
	}
	m_StationItemMap.RemoveAll();

	return;
}

//�����б�
bool __cdecl CServerListCenter::LoadServerList(tagDataBaseInfo & DataBaseInfo)
{
	//�����б�
	ResetServerList();

	//�������ݿ�
	CDataBaseHelper ServerInfoDB;
	if (ServerInfoDB.CreateInstance()==false) return false;

	try
	{
		//ת����ַ
		TCHAR szDataBaseAdr[16]=TEXT("");
		BYTE * pcbDataBaseAdr=(BYTE *)&DataBaseInfo.dwDataBaseAddr;
		_snprintf(szDataBaseAdr,sizeof(szDataBaseAdr),TEXT("%d.%d.%d.%d"),pcbDataBaseAdr[0],pcbDataBaseAdr[1],pcbDataBaseAdr[2],pcbDataBaseAdr[3]);

		//�������ݿ�
		ServerInfoDB->SetConnectionInfo(szDataBaseAdr,DataBaseInfo.szDataBasePipeName,
			DataBaseInfo.wDataBasePort,DataBaseInfo.szDataBaseName,DataBaseInfo.szDataBaseUser,DataBaseInfo.szDataBasePass);
		if (ServerInfoDB->OpenConnection()==false) return false;

		//������Ϣ
		LoadGameType(ServerInfoDB.GetInterface());
		LoadGameKind(ServerInfoDB.GetInterface());
		LoadGameStation(ServerInfoDB.GetInterface());

		return true;
	}
	catch (IADOError * pIADOError)
	{ 
		GT_ASSERT(m_pIEventService!=NULL);
		m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
	}

	return false;
}
//����ȫ���б�
bool __cdecl CServerListCenter::LoadAllServerList(tagDataBaseInfo & DataBaseInfo)
{
	//�����б�
	ResetServerList();

	//�������ݿ�
	CDataBaseHelper ServerInfoDB;
	if (ServerInfoDB.CreateInstance()==false) return false;

	try
	{
		//ת����ַ
		TCHAR szDataBaseAdr[16]=TEXT("");
		BYTE * pcbDataBaseAdr=(BYTE *)&DataBaseInfo.dwDataBaseAddr;
		_snprintf(szDataBaseAdr,sizeof(szDataBaseAdr),TEXT("%d.%d.%d.%d"),pcbDataBaseAdr[0],pcbDataBaseAdr[1],pcbDataBaseAdr[2],pcbDataBaseAdr[3]);

		//�������ݿ�
		ServerInfoDB->SetConnectionInfo(szDataBaseAdr,DataBaseInfo.szDataBasePipeName,
			DataBaseInfo.wDataBasePort,
			DataBaseInfo.szDataBaseName,
			DataBaseInfo.szDataBaseUser,
			DataBaseInfo.szDataBasePass);
		if (ServerInfoDB->OpenConnection()==false) return false;

		//������Ϣ
		LoadGameType(ServerInfoDB.GetInterface());
		LoadGameKind(ServerInfoDB.GetInterface());
		LoadGameStation(ServerInfoDB.GetInterface());
		LoadGameServer(ServerInfoDB.GetInterface());

		return true;
	}
	catch (IADOError * pIADOError)
	{ 
		GT_ASSERT(m_pIEventService!=NULL);
		m_pIEventService->ShowEventNotify(pIADOError->GetErrorDescribe(),Level_Exception);
	}

	return false;
}
//��������
bool __cdecl CServerListCenter::UpdateServerOnLineCount(WORD wKindID, WORD wServerID, DWORD dwOnLineCount)
{
	tagGameServerItem * pGameServerItem=SearchGameServer(wKindID,wServerID);
	if (pGameServerItem!=NULL)
	{
		//���·���
		pGameServerItem->dwUpdateTime=(DWORD)time(NULL);
		pGameServerItem->GameServer.dwOnLineCount=dwOnLineCount;

		//��������
		UpdateKindOnLineCount(wKindID);

		return true;
	}

	return false;
}

//��������
void CServerListCenter::LoadGameType(IDataBase * pIDataBase)
{
	//ִ������
	pIDataBase->ClearAllParameters();
	pIDataBase->SetSPName(TEXT("GSP_GP_LoadTypeInfo"));
	pIDataBase->ExecuteCommand(true);

	//��ȡ��Ϣ
	tagGameType GameTypeRead;
	memset(&GameTypeRead,0,sizeof(GameTypeRead));
	while (pIDataBase->IsEndRecordset()==false)
	{
		//��ȡ��Ϣ
		pIDataBase->GetFieldValue(TEXT("TypeID"),GameTypeRead.wTypeID);
		pIDataBase->GetFieldValue(TEXT("SortID"),GameTypeRead.wSortID);
		pIDataBase->GetFieldValue(TEXT("ImageID"),(BYTE)GameTypeRead.nbImageID);
		pIDataBase->GetFieldValue(TEXT("TypeName"),GameTypeRead.szTypeName,sizeof(GameTypeRead.szTypeName));
		pIDataBase->MoveToNext();

		//������Ϣ
		InsertGameType(&GameTypeRead);
	}
	pIDataBase->CloseRecordset();

	return;
}

//��������
void CServerListCenter::LoadGameKind(IDataBase * pIDataBase)
{
	//ִ������
	pIDataBase->ClearAllParameters();
	pIDataBase->SetSPName(TEXT("GSP_GP_LoadKindInfo"));
	pIDataBase->ExecuteCommand(true);

	//��ȡ��Ϣ
	tagGameKind GameKindRead;
	memset(&GameKindRead,0,sizeof(GameKindRead));
	while (pIDataBase->IsEndRecordset()==false)
	{
		//��ȡ��Ϣ
		pIDataBase->GetFieldValue(TEXT("TypeID"),GameKindRead.wTypeID);
		pIDataBase->GetFieldValue(TEXT("KindID"),GameKindRead.wKindID);
		pIDataBase->GetFieldValue(TEXT("SortID"),GameKindRead.wSortID);
		pIDataBase->GetFieldValue(TEXT("KindName"),GameKindRead.szKindName,sizeof(GameKindRead.szKindName));
		pIDataBase->GetFieldValue(TEXT("ProcessType"),GameKindRead.wProcessType);
		pIDataBase->GetFieldValue(TEXT("TableCount"),GameKindRead.wTableCount);
		pIDataBase->GetFieldValue(TEXT("CellScore"),GameKindRead.dwCellScore);
		pIDataBase->GetFieldValue(TEXT("HighScore"),GameKindRead.dwHighScore);
		pIDataBase->GetFieldValue(TEXT("LessScore"),GameKindRead.dwLessScore);
		pIDataBase->GetFieldValue(TEXT("TaxRate"),GameKindRead.fTaxRate);
		pIDataBase->GetFieldValue(TEXT("AIUserCount"),GameKindRead.wAIUserCount);
		pIDataBase->GetFieldValue(TEXT("AILevel"),GameKindRead.cbAILevel);

		pIDataBase->MoveToNext();

		//������Ϣ
		InsertGameKind(&GameKindRead);
	}
	pIDataBase->CloseRecordset();

	return;
}

//����վ��
void CServerListCenter::LoadGameStation(IDataBase * pIDataBase)
{
	//ִ������
	pIDataBase->ClearAllParameters();
	pIDataBase->SetSPName(TEXT("GSP_GP_LoadStationInfo"));
	pIDataBase->ExecuteCommand(true);

	//��ȡ��Ϣ
	tagGameStation GameStationRead;
	memset(&GameStationRead,0,sizeof(GameStationRead));
	while (pIDataBase->IsEndRecordset()==false)
	{
		//��ȡ��Ϣ
		pIDataBase->GetFieldValue(TEXT("SortID"),GameStationRead.wSortID);
		pIDataBase->GetFieldValue(TEXT("KindID"),GameStationRead.wKindID);
		pIDataBase->GetFieldValue(TEXT("JoinID"),GameStationRead.wJoinID);
		pIDataBase->GetFieldValue(TEXT("StationID"),GameStationRead.wStationID);
		pIDataBase->GetFieldValue(TEXT("StationName"),GameStationRead.szStationName,sizeof(GameStationRead.szStationName));
		pIDataBase->MoveToNext();

		//������Ϣ
		InsertGameStation(&GameStationRead);
	}
	pIDataBase->CloseRecordset();

	return;
}

//���ط���
void CServerListCenter::LoadGameServer(IDataBase * pIDataBase)
{
	//ִ������
	pIDataBase->ClearAllParameters();
	pIDataBase->SetSPName(TEXT("GSP_GP_LoadServerInfo"));
	pIDataBase->ExecuteCommand(true);

	//��ȡ��Ϣ
	tagGameServer GameServerRead;
	memset(&GameServerRead,0,sizeof(GameServerRead));
	while (pIDataBase->IsEndRecordset()==false)
	{
		//��ȡ��Ϣ
		pIDataBase->GetFieldValue(TEXT("SortID"),GameServerRead.wSortID);
		pIDataBase->GetFieldValue(TEXT("KindID"),GameServerRead.wKindID);
		pIDataBase->GetFieldValue(TEXT("TypeID"),GameServerRead.wTypeID);
		pIDataBase->GetFieldValue(TEXT("ServerID"),GameServerRead.wServerID);
		pIDataBase->GetFieldValue(TEXT("ServerName"),GameServerRead.szServerName,sizeof(GameServerRead.szServerName));
		pIDataBase->GetFieldValue(TEXT("MaxUserCount"),GameServerRead.wMaxConnect);
		pIDataBase->MoveToNext();

		//������Ϣ
		InsertGameServer(&GameServerRead);
	}
	pIDataBase->CloseRecordset();

	return;
}
//��ȡ����
UINT CServerListCenter::GetMaxPrime(UINT uLessNum)
{
	UINT u2= 0;
	UINT uPrime = 0;
	for (uPrime=uLessNum;uPrime>2;uPrime--)
	{
		for(u2=2;u2<=uPrime/2;u2++) if (uPrime%u2==0) break;
		if (u2>uPrime/2) break;
	}

	return uPrime;
}

//��������
void CServerListCenter::UpdateKindOnLineCount(WORD wKindID)
{
	tagGameKindItem * pGameKindItem=SearchGameKind(wKindID);
	if (pGameKindItem!=NULL)
	{
		POSITION Pos=NULL;
		DWORD dwKindAllLineCount=0L;
		tagGameServerItem * pGameServerItem=NULL;
		do
		{
			pGameServerItem=EmunGameServerItem(Pos);
			if (pGameServerItem==NULL) break;
			if (pGameServerItem->GameServer.wKindID==wKindID) dwKindAllLineCount+=pGameServerItem->GameServer.dwOnLineCount;
		} while (Pos!=NULL);
		pGameKindItem->GameKind.dwOnLineCount=dwKindAllLineCount;
	}

	return;
}

//////////////////////////////////////////////////////////////////////////

//����������
extern "C" __declspec(dllexport) void * __cdecl CreateServerListCenter(const GUID & Guid, DWORD dwInterfaceVer)
{
	//��������
	CServerListCenter * pServerListCenter=NULL;
	_BEGIN_TRY
	{
		pServerListCenter=new CServerListCenter();
		if (pServerListCenter==NULL) throw TEXT("����ʧ��");
		void * pObject=pServerListCenter->QueryInterface(Guid,dwInterfaceVer);
		if (pObject==NULL) throw TEXT("�ӿڲ�ѯʧ��");
		return pObject;
	}
	CATCH_COMMON_EXCEPTION(;)CATCH_UNKNOWN_EXCEPTION(;)_END_CATCH

	//�������
	SafeDelete(pServerListCenter);
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
