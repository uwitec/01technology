#include "StdAfx.h"
#include "Tableframesink.h"
#include "GameServerManager.h"
#include "..\CommandDefinition\CMD_ShowHand.h"

//////////////////////////////////////////////////////////////////////////

//全局变量
static CGameServiceManager			g_GameServiceManager;				//管理变量

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameServiceManager::CGameServiceManager(void)
{
	//设置属性
	m_GameServiceAttrib.wKindID=KIND_ID;
	m_GameServiceAttrib.dwServerVersion=0;
	m_GameServiceAttrib.dwMaxClientVersion=0;
	m_GameServiceAttrib.dwLessClientVersion=0;
	m_GameServiceAttrib.wSupporType=GAME_GENRE;
	m_GameServiceAttrib.wChairCount=GAME_PLAYER;
	lstrcpyn(m_GameServiceAttrib.szKindName,GAME_NAME,CountArray(m_GameServiceAttrib.szKindName));
	lstrcpyn(m_GameServiceAttrib.szDescription,TEXT("梭哈GameService组件"),CountArray(m_GameServiceAttrib.szDescription));
	lstrcpyn(m_GameServiceAttrib.szServerModuleName,TEXT("B_ShowHand.dll"),CountArray(m_GameServiceAttrib.szServerModuleName));
	m_GameServiceAttrib.wProcessType = ID_B_SHOWHAND;
	m_GameServiceAttrib.wBetRegionCount = 1;
	m_GameServiceAttrib.bCheckCellScoreBeforeStartGame = TRUE;
	m_GameServiceAttrib.bCheckSameIP = TRUE;
	return;
}

//析构函数
CGameServiceManager::~CGameServiceManager(void)
{
}

//接口查询
void * __cdecl CGameServiceManager::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IGameServiceManager,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IGameServiceManager,Guid,dwQueryVer);
	return NULL;
}

//创建游戏桌
void * __cdecl CGameServiceManager::CreateTableFrameSink(const IID & Guid, DWORD dwQueryVer)
{
	//建立对象
	CTableFrameSink * pTableFrameSink=NULL;
	try
	{
		pTableFrameSink=new CTableFrameSink();
		if (pTableFrameSink==NULL) throw TEXT("创建失败");
		void * pObject=pTableFrameSink->QueryInterface(Guid,dwQueryVer);
		if (pObject==NULL) throw TEXT("接口查询失败");
		return pObject;
	}
	catch (...) {}

	//清理对象
	SafeDelete(pTableFrameSink);

	return NULL;
}

//获取属性
void __cdecl CGameServiceManager::GetGameServiceAttrib(tagGameServiceAttrib & GameServiceAttrib)
{
	GameServiceAttrib=m_GameServiceAttrib;
	return;
}

//参数修改
bool __cdecl CGameServiceManager::RectifyServiceOption(tagGameServiceOption * pGameServiceOption)
{
	//效验参数
	GT_ASSERT(pGameServiceOption!=NULL);
	if (pGameServiceOption==NULL) return false;

	//信用额度调整
	pGameServiceOption->dwCellScore=__max(1L,pGameServiceOption->dwCellScore);
	if (pGameServiceOption->dwHighScore!=0L) pGameServiceOption->dwHighScore=__max(pGameServiceOption->dwHighScore,pGameServiceOption->dwLessScore);

	return true;
}

//////////////////////////////////////////////////////////////////////////

//建立对象函数
extern "C" __declspec(dllexport) void * __cdecl CreateGameServiceManager(const GUID & Guid, DWORD dwInterfaceVer)
{
	return g_GameServiceManager.QueryInterface(Guid,dwInterfaceVer);
}

//////////////////////////////////////////////////////////////////////////
