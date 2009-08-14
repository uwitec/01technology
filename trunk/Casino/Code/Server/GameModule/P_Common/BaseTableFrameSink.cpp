#include "StdAfx.h"
#include ".\basetableframesink.h"

CBaseTableFrameSink::CBaseTableFrameSink(void)
{
	m_pITableFrame = NULL;
	m_pGameServiceOption = NULL;
	m_pGameServiceAttrib = NULL;
}

CBaseTableFrameSink::~CBaseTableFrameSink(void)
{
}
//接口查询
void * __cdecl CBaseTableFrameSink::QueryInterface(const IID & Guid,
											   DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink, Guid, dwQueryVer);

	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink, Guid, dwQueryVer);
	return NULL;
}
//初始化
bool __cdecl CBaseTableFrameSink::InitTableFrameSink(IUnknownEx * pIUnknownEx)
{
	//查询接口
	GT_ASSERT(pIUnknownEx != NULL);

	m_pITableFrame = GET_OBJECTPTR_INTERFACE(pIUnknownEx, ITableFrame);

	if (m_pITableFrame == NULL)
	{
		return false;
	}

	//获取参数
	m_pGameServiceOption = m_pITableFrame->GetGameServiceOption();

	GT_ASSERT(m_pGameServiceOption != NULL);

	m_pITableFrame->SetGameStatus(GS_FREE);

	m_pGameServiceAttrib=m_pITableFrame->GetGameServiceAttrib();
	GT_ASSERT(m_pGameServiceAttrib!=NULL);

	return true;
}

//复位桌子
void __cdecl CBaseTableFrameSink::RepositTableFrameSink()
{
	//设置状态
	m_pITableFrame->SetGameStatus(GS_FREE);

	return;
}

//开始模式
enStartMode __cdecl CBaseTableFrameSink::GetGameStartMode()
{
	return enStartMode_AllReady;	//开始模式
}


//游戏状态
bool __cdecl CBaseTableFrameSink::IsUserPlaying(WORD wChairID)
{	
	return  true;
}