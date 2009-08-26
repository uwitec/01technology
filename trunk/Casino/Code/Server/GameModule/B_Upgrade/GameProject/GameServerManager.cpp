#include "StdAfx.h"
#include "Tableframesink.h"
#include "GameServerManager.h"

//////////////////////////////////////////////////////////////////////////

//ȫ�ֱ���
static CGameServiceManager			g_GameServiceManager;				//��������

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameServiceManager::CGameServiceManager()
{
	//��������
	m_GameServiceAttrib.wKindID=KIND_ID;
	m_GameServiceAttrib.dwServerVersion=0;
	m_GameServiceAttrib.dwMaxClientVersion=0;
	m_GameServiceAttrib.dwLessClientVersion=0;
	m_GameServiceAttrib.wSupporType=GAME_GENRE;
	m_GameServiceAttrib.wChairCount=GAME_PLAYER;
	lstrcpyn(m_GameServiceAttrib.szKindName,GAME_NAME,CountArray(m_GameServiceAttrib.szKindName));
	lstrcpyn(m_GameServiceAttrib.szDescription,TEXT("����GameService���"),CountArray(m_GameServiceAttrib.szDescription));
	m_GameServiceAttrib.wProcessType = ID_B_UPGRADE;
	lstrcpyn(m_GameServiceAttrib.szServerModuleName,TEXT("B_UpGrade.dll"),CountArray(m_GameServiceAttrib.szServerModuleName));
	m_GameServiceAttrib.wBetRegionCount = 1;
	m_GameServiceAttrib.bCheckCellScoreBeforeStartGame = TRUE;
	m_GameServiceAttrib.bCheckSameIP = TRUE;
	return;
}

//��������
CGameServiceManager::~CGameServiceManager(void)
{
}

//�ӿڲ�ѯ
void * __cdecl CGameServiceManager::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IGameServiceManager,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IGameServiceManager,Guid,dwQueryVer);
	return NULL;
}

//������Ϸ��
void * __cdecl CGameServiceManager::CreateTableFrameSink(const IID & Guid, DWORD dwQueryVer)
{
	//��������
	CTableFrameSink * pTableFrameSink=NULL;
	try
	{
		pTableFrameSink=new CTableFrameSink();
		if (pTableFrameSink==NULL) throw TEXT("����ʧ��");
		void * pObject=pTableFrameSink->QueryInterface(Guid,dwQueryVer);
		if (pObject==NULL) throw TEXT("�ӿڲ�ѯʧ��");
		return pObject;
	}
	catch (...) {}

	//��������
	SafeDelete(pTableFrameSink);

	return NULL;
}

//��ȡ����
void __cdecl CGameServiceManager::GetGameServiceAttrib(tagGameServiceAttrib & GameServiceAttrib)
{
	GameServiceAttrib=m_GameServiceAttrib;
	return;
}

//�����޸�
bool __cdecl CGameServiceManager::RectifyServiceOption(tagGameServiceOption * pGameServiceOption)
{
	//Ч�����
	GT_ASSERT(pGameServiceOption!=NULL);
	if (pGameServiceOption==NULL) return false;

	//��ȵ���
	pGameServiceOption->dwCellScore=__max(1L,pGameServiceOption->dwCellScore);

	//��ҷ���
	if (pGameServiceOption->wServerType&GAME_GENRE_GOLD)
	{
		if (pGameServiceOption->dwHighScore!=0L) pGameServiceOption->dwHighScore=__max(pGameServiceOption->dwHighScore,pGameServiceOption->dwLessScore);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////

//����������
extern "C" __declspec(dllexport) void * __cdecl CreateGameServiceManager(const GUID & Guid, DWORD dwInterfaceVer)
{
	return g_GameServiceManager.QueryInterface(Guid,dwInterfaceVer);
}

//////////////////////////////////////////////////////////////////////////