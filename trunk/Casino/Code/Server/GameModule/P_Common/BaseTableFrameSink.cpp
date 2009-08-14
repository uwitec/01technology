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
//�ӿڲ�ѯ
void * __cdecl CBaseTableFrameSink::QueryInterface(const IID & Guid,
											   DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink, Guid, dwQueryVer);

	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink, Guid, dwQueryVer);
	return NULL;
}
//��ʼ��
bool __cdecl CBaseTableFrameSink::InitTableFrameSink(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	GT_ASSERT(pIUnknownEx != NULL);

	m_pITableFrame = GET_OBJECTPTR_INTERFACE(pIUnknownEx, ITableFrame);

	if (m_pITableFrame == NULL)
	{
		return false;
	}

	//��ȡ����
	m_pGameServiceOption = m_pITableFrame->GetGameServiceOption();

	GT_ASSERT(m_pGameServiceOption != NULL);

	m_pITableFrame->SetGameStatus(GS_FREE);

	m_pGameServiceAttrib=m_pITableFrame->GetGameServiceAttrib();
	GT_ASSERT(m_pGameServiceAttrib!=NULL);

	return true;
}

//��λ����
void __cdecl CBaseTableFrameSink::RepositTableFrameSink()
{
	//����״̬
	m_pITableFrame->SetGameStatus(GS_FREE);

	return;
}

//��ʼģʽ
enStartMode __cdecl CBaseTableFrameSink::GetGameStartMode()
{
	return enStartMode_AllReady;	//��ʼģʽ
}


//��Ϸ״̬
bool __cdecl CBaseTableFrameSink::IsUserPlaying(WORD wChairID)
{	
	return  true;
}