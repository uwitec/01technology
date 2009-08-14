#include "StdAfx.h"
#include "CenterService.h"

//////////////////////////////////////////////////////////////////////////

//���캯��
CCenterService::CCenterService()
{
	m_pIEventService=NULL;
}

//��������
CCenterService::~CCenterService()
{
}

//��ʼ����
bool CCenterService::StartService()
{
	try
	{
		//��������
		bool bSuccess=false;

		//�����������
		if (m_ServiceEngine.IsValid()==false)
		{
			if (m_ServiceEngine.CreateInstance()==false) 
			{
				throw m_ServiceEngine.GetErrorMessage();
			}
			if (m_pIEventService!=NULL) 
			{
				m_ServiceEngine->SetEventService(m_pIEventService);
			}

			//ע�������ҷ���
			IUnknownEx * pIAttemperEngine=GET_OBJECT_INTERFACE(m_AttemperSink,IUnknownEx);
			if (m_ServiceEngine->SetAttemperEngineSink(pIAttemperEngine)==false) 
			{
				throw 0;
			}

			//ע�����ݿ���ҷ���
			IUnknownEx * pIDataBaseSink=GET_OBJECT_INTERFACE(m_DataBaseSink,IUnknownEx);
			if (m_ServiceEngine->SetDataBaseSink(pIDataBaseSink)==false) 
			{
				throw 0;
			}
		}

		//�ж�״̬
		if (m_ServiceEngine->IsService()==true)
		{
			TraceEventString(TEXT("���ķ����Ѿ���������"),Level_Warning);
			return true;
		}

		//���÷���
		m_InitParamter.LoadInitParamter();
		if (m_DataBaseSink.InitDataBaseSink(&m_InitParamter,m_ServiceEngine.GetInterface())==false) 
		{
			throw 0;
		}
		if (m_AttemperSink.InitAttemperSink(&m_InitParamter,m_ServiceEngine.GetInterface())==false) 
		{
			throw 0;
		}
		if (m_ServiceEngine->InitServiceEngine(m_InitParamter.m_wListenPort,m_InitParamter.m_wMaxConnect)==false) 
		{
			throw 0;
		}

		//��������
		if (m_ServiceEngine->StartService(1)==false) 
		{
			throw 0;
		}

		return true;
	}
	catch (LPCTSTR pszErrorMsg)
	{
		TraceEventString(pszErrorMsg,Level_Exception); 
		TraceEventString(TEXT("���ķ�������ʧ��"),Level_Exception); 
		StopService();
	}
	catch (...)
	{ 
		TraceEventString(TEXT("���ķ�������ʧ��"),Level_Exception); 
		StopService();
	}

	return false;
}

//ֹͣ����
bool CCenterService::StopService()
{
	try
	{
		if (m_ServiceEngine.GetInterface()!=NULL) 
		{
			m_ServiceEngine->StopService();
		}
		return true;
	}
	catch (LPCTSTR pszErrorMsg)
	{
		TraceEventString(pszErrorMsg,Level_Exception); 
		TraceEventString(TEXT("���ķ���ֹͣʧ��"),Level_Exception); 
	}
	catch (...)
	{ 
		TraceEventString(TEXT("���ķ���ֹͣʧ��"),Level_Exception); 
	}

	return false;
}

//����״̬
bool CCenterService::IsService()
{
	if (m_ServiceEngine.IsValid()==false) 
	{
		return false;
	}
	return m_ServiceEngine->IsService();
}

//�����¼�
bool CCenterService::SetEventService(IUnknownEx * pIEventService)
{
	//��ȡ�ӿ�
	GT_ASSERT(pIEventService!=NULL);
	m_pIEventService=(IEventService *)pIEventService->QueryInterface(IID_IEventService,VER_IEventService);
	GT_ASSERT(m_pIEventService!=NULL);

	//���ýӿ�
	m_DataBaseSink.SetEventService(pIEventService);
	m_AttemperSink.SetEventService(pIEventService);
	if (m_ServiceEngine.IsValid()) 
	{
		m_ServiceEngine->SetEventService(pIEventService);
	}

	return (m_pIEventService!=NULL);
}

//��ʾ����
void CCenterService::TraceEventString(LPCTSTR pszString, enTraceLevel TraceLevel)
{
	if (m_pIEventService!=NULL)
	{
		m_pIEventService->ShowEventNotify(pszString,TraceLevel);
	}
	
	return ;
}

//////////////////////////////////////////////////////////////////////////
