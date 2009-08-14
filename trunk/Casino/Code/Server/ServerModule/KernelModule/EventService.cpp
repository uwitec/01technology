#include "StdAfx.h"
#include "EventService.h"

//////////////////////////////////////////////////////////////////////////

//��ɫ����
#define COLOR_TIME						RGB(250,250,250)
#define COLOR_NORMAL					RGB(250,250,250)
#define COLOR_WARN						RGB(255,128,0)
#define COLOR_EXCEPTION					RGB(200,0,0)
#define COLOR_DEBUG						RGB(0,128,128)

//��̬����
IEventService		* CEventTrace::m_pIEventService=NULL;				//�¼��ӿ�

//////////////////////////////////////////////////////////////////////////

//���캯��
CEventService::CEventService(void)
{
	//���ñ���
	m_hRichEdit=NULL;
	m_nMaxLineCount=300;
	m_nReserveLineCount=20;
	for (int i=0;i<CountArray(m_bEventService);i++) m_bEventService[i]=true;
}

//��������
CEventService::~CEventService(void)
{
}

//�ӿڲ�ѯ
void * __cdecl CEventService::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IEventService,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IEventService,Guid,dwQueryVer);
	return NULL;
}

//���þ��
bool CEventService::SetRichEditHwnd(HWND hRichEdit)
{
	GT_ASSERT(IsWindow(hRichEdit));
	if (IsWindow(hRichEdit))
	{
		m_hRichEdit=hRichEdit;
		return true;
	}
	return false;
}

//���ü���
void __cdecl CEventService::ConfigEventService(enTraceLevel TraceLevel, bool bShow)
{
	GT_ASSERT(TraceLevel<EVENT_LEVEL_COUNT);
	if (TraceLevel<EVENT_LEVEL_COUNT) m_bEventService[TraceLevel]=bShow;
	return;
}

//�����ַ�
void __cdecl CEventService::ShowEventNotify(LPCTSTR pszString, enTraceLevel TraceLevel)
{
	//�ж���ʾ
	GT_ASSERT(TraceLevel<EVENT_LEVEL_COUNT);
	if ((TraceLevel<EVENT_LEVEL_COUNT)&&(m_bEventService[TraceLevel]==false)) return;

	//���ø�ʽ
	CHARFORMAT2 CharFormat;
	memset(&CharFormat,0,sizeof(CharFormat));
	CharFormat.cbSize=sizeof(CharFormat); 
	CharFormat.dwMask=CFM_COLOR|CFM_FACE|CFM_SIZE;
	CharFormat.yHeight=180;
	lstrcpyn(CharFormat.szFaceName,TEXT("����"),sizeof(CharFormat.szFaceName));

	//��ȡ��ɫ
	COLORREF crColor=COLOR_NORMAL;
	switch (TraceLevel)
	{
	case Level_Normal: { crColor=COLOR_NORMAL; break; }
	case Level_Warning: { crColor=COLOR_WARN; break; }
	case Level_Exception: { crColor=COLOR_EXCEPTION; break; }
	case Level_Debug: { crColor=COLOR_DEBUG; break; }
	}

	//������Դ
	CThreadLockHandle LockHandle(&m_ThreadLock);

	//��ȡָ��
	if (IsWindow(m_hRichEdit)==FALSE) return;
	CRichEditCtrl * pRichEdit=(CRichEditCtrl *)CRichEditCtrl::FromHandle(m_hRichEdit);
	if (pRichEdit==NULL) return;

	//������Ϣ
	int nLineCount=pRichEdit->GetLineCount();
	if (nLineCount>=m_nMaxLineCount)
	{
		GT_ASSERT(m_nMaxLineCount>m_nReserveLineCount);
		pRichEdit->SetSel(0,pRichEdit->LineIndex(nLineCount-m_nReserveLineCount));
		pRichEdit->ReplaceSel(TEXT(""));
	}

	//��ȡʱ��
	SYSTEMTIME SystemTime;
	TCHAR szTimeBuffer[64];
	GetLocalTime(&SystemTime);
	_snprintf(szTimeBuffer,sizeof(szTimeBuffer),TEXT("�� %02d:%02d:%02d ��"),SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond);

	//������Ϣ
	CharFormat.crTextColor=COLOR_TIME;
	long lTextLength=pRichEdit->GetWindowTextLength();
	pRichEdit->SetSel(lTextLength,lTextLength);
	pRichEdit->SetSelectionCharFormat(CharFormat);
	pRichEdit->ReplaceSel(szTimeBuffer);

	//������Ϣ
	CharFormat.crTextColor=crColor;
	lTextLength=pRichEdit->GetWindowTextLength();
	pRichEdit->SetSel(lTextLength,lTextLength);
	pRichEdit->SetSelectionCharFormat(CharFormat);
	pRichEdit->ReplaceSel(pszString);

	//������Ϣ
	lTextLength=pRichEdit->GetWindowTextLength();
	pRichEdit->SetSel(lTextLength,lTextLength);
	pRichEdit->SetSelectionCharFormat(CharFormat);
	pRichEdit->ReplaceSel(TEXT("\n"));

	return;
}

//////////////////////////////////////////////////////////////////////////

//���ýӿ�
bool CEventTrace::SetEventService(IUnknownEx * pIUnknownEx)
{
	if (pIUnknownEx!=NULL)
	{
		IEventService * pIEventService=GET_OBJECTPTR_INTERFACE(pIUnknownEx,IEventService);
		GT_ASSERT(pIEventService!=NULL);
		if (pIEventService==NULL) return false;
		m_pIEventService=pIEventService;
	}
	else m_pIEventService=NULL;

	return true;
}

//��ȡ�ӿ�
void * CEventTrace::GetEventService(const IID & Guid, DWORD dwQueryVer)
{
	if (m_pIEventService==NULL) return NULL;
	return m_pIEventService->QueryInterface(Guid,dwQueryVer);
}

//���ü���
void CEventTrace::ConfigEventService(enTraceLevel TraceLevel, bool bShow)
{
	if (m_pIEventService!=NULL) m_pIEventService->ConfigEventService(TraceLevel,bShow);
}

//�¼�֪ͨ
void CEventTrace::ShowEventNotify(LPCTSTR pszString, enTraceLevel TraceLevel)
{
	if (m_pIEventService!=NULL) m_pIEventService->ShowEventNotify(pszString,TraceLevel);
}

//////////////////////////////////////////////////////////////////////////

//����������
extern "C" __declspec(dllexport) void * __cdecl CreateEventService(const GUID & Guid, DWORD dwInterfaceVer)
{
	//��������
	CEventService * pEventService=NULL;
	_BEGIN_TRY
	{
		pEventService=new CEventService();
		if (pEventService==NULL) throw TEXT("����ʧ��");
		void * pObject=pEventService->QueryInterface(Guid,dwInterfaceVer);
		if (pObject==NULL) throw TEXT("�ӿڲ�ѯʧ��");
		return pObject;
	}
	CATCH_COMMON_EXCEPTION(;)CATCH_UNKNOWN_EXCEPTION(;)_END_CATCH

	//�������
	SafeDelete(pEventService);
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
