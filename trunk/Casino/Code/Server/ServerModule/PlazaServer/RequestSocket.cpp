#include ".\requestsocket.h"
#include ".\InitParamter.h"

CRequestSocket::CRequestSocket(void)
{
	m_pIEventService = NULL;
	m_pParentWnd	 = NULL;

	
}

CRequestSocket::~CRequestSocket(void)
{
	if (m_ClientSocketHelper.GetInterface()!=NULL) 
	{
		m_ClientSocketHelper->CloseSocket(false);
		m_ClientSocketHelper.CloseInstance();
	}

}
//�ӿڲ�ѯ
void * __cdecl CRequestSocket::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IClientSocketSink,Guid,dwQueryVer);
	return NULL;
}
//����������Ϣ
bool __cdecl CRequestSocket::OnSocketConnect(int iErrorCode, LPCTSTR pszErrorDesc, IClientSocket * pIClientSocke)
{
	if(iErrorCode == 0)
	{
		GT_ASSERT(m_pIEventService!=NULL);
		m_pIEventService->ShowEventNotify(TEXT("���ķ��������ӳɹ�"),Level_Normal);
		m_pIEventService->ShowEventNotify(TEXT("���������½��Ϣ"),Level_Normal);

		CMD_CS_GetLogonServerInfo sendData;
		//���ز���
		CInitParamter InitParamter;
		InitParamter.LoadInitParamter();

		//��������
		TCHAR szWriteBuffer[256]=TEXT("");
		CXOREncrypt::EncryptData(InitParamter.m_szLogonServerPass,szWriteBuffer,sizeof(szWriteBuffer));
		lstrcpyn(sendData.szLogonServerPass,szWriteBuffer,CountArray(sendData.szLogonServerPass));

		return SendData(MDM_CS_SERVER_REQUEST, SUB_CS_GET_LOGONSERVERINFO, &sendData, sizeof(sendData));
	}
	else
	{
		GT_ASSERT(m_pIEventService!=NULL);
		m_pIEventService->ShowEventNotify(TEXT("���ķ���������ʧ��"),Level_Exception);
		return false;
	}
}

//�����ȡ��Ϣ
bool __cdecl CRequestSocket::OnSocketRead(CMD_Command Command, void * pBuffer, WORD wDataSize, IClientSocket * pIClientSocke)
{
	switch(Command.wMainCmdID) 
	{
	case MDM_CS_SERVER_REQUEST:
		return OnSocketServerRequest(Command, pBuffer, wDataSize);
	default:
		break;
	}
	return false;
}

//����ر���Ϣ
bool __cdecl CRequestSocket::OnSocketClose(IClientSocket * pIClientSocke, bool bCloseByServer)
{
	GT_ASSERT(m_pIEventService!=NULL);
	m_pIEventService->ShowEventNotify(TEXT("���ķ��������ӶϿ�"),Level_Exception);

	return true;
}
//�����¼�
bool CRequestSocket::SetEventService(IUnknownEx * pIUnknownEx)
{
	GT_ASSERT(pIUnknownEx!=NULL);
	m_pIEventService=GET_OBJECTPTR_INTERFACE(pIUnknownEx,IEventService);
	GT_ASSERT(m_pIEventService!=NULL);
	return (m_pIEventService!=NULL);
}
//����������
bool CRequestSocket::SetParentWnd(CWnd *pParent)
{
	m_pParentWnd = pParent;
	return (m_pParentWnd!=NULL);
}
//���ӷ�����
bool CRequestSocket::Connect(DWORD dwServerIP, WORD wPort)
{
	CloseSocket(false);

	//���Ӷ���
	if (m_ClientSocketHelper.GetInterface()==NULL)
	{
		//��������
		bool bSuccess=m_ClientSocketHelper.CreateInstance();
		if (bSuccess==false)
		{
			GT_ASSERT(m_pIEventService!=NULL);
			m_pIEventService->ShowEventNotify(TEXT("���ķ��������Ӷ��󴴽�ʧ��"),Level_Exception);
			return false;
		}

		//���ýӿ�
		IClientSocketSink * pIClientSocketSink=GET_MYSELF_INTERFACE(IClientSocketSink);
		GT_ASSERT(pIClientSocketSink!=NULL);
		if (m_ClientSocketHelper->SetSocketSink(pIClientSocketSink)==false)
		{
			GT_ASSERT(m_pIEventService!=NULL);
			m_pIEventService->ShowEventNotify(TEXT("���ķ��������Ӷ���ص��ӿ�����ʧ��"),Level_Exception);
			return false;

		}
	}

	//������
	GT_ASSERT(m_ClientSocketHelper.GetInterface()!=NULL);
	return m_ClientSocketHelper->Connect(dwServerIP,wPort);
}

//���ͺ���
bool CRequestSocket::SendData(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize)
{
	//������
	GT_ASSERT(m_ClientSocketHelper.GetInterface()!=NULL);
	return m_ClientSocketHelper->SendData(wMainCmdID,wSubCmdID,pData,wDataSize);
}

//�ر�����
bool CRequestSocket::CloseSocket(bool bNotify)
{
	//�ر�����
	if (m_ClientSocketHelper.GetInterface()!=NULL) 
	{
		m_ClientSocketHelper->CloseSocket(false);
	}
	return true;
}
bool __cdecl CRequestSocket::OnSocketServerRequest(CMD_Command Command, void * pBuffer, WORD wDataSize)
{
	switch(Command.wSubCmdID) 
	{
	case SUB_CS_GET_LOGONSERVERINFO_SUCCESS:
		{

			GT_ASSERT(wDataSize >= sizeof(CMD_CS_LogonServerInfo));

			CMD_CS_LogonServerInfo *pInfo = (CMD_CS_LogonServerInfo*)pBuffer;
			GT_ASSERT(m_pIEventService!=NULL);
			m_pIEventService->ShowEventNotify(TEXT("�����½��Ϣ�ɹ�"),Level_Normal);

			//��ȡ����
			CInitParamter InitParamter;
			InitParamter.LoadInitParamter();

			CXOREncrypt::CrevasseData(pInfo->szDataBaseUser,InitParamter.m_szUserDataBaseUser,CountArray(InitParamter.m_szUserDataBaseUser));
			CXOREncrypt::CrevasseData(pInfo->szDataBasePass,InitParamter.m_szUserDataBasePass,CountArray(InitParamter.m_szUserDataBasePass));
			CXOREncrypt::CrevasseData(pInfo->szDataBasePipeName,InitParamter.m_szUserDataBasePipeName,CountArray(InitParamter.m_szUserDataBasePipeName));

			InitParamter.m_wUserDataBasePort = pInfo->wDataBasePort;
			struct in_addr in;
			in.S_un.S_addr	= pInfo->dwDataBaseAddr;
			strcpy(InitParamter.m_szUserDataBaseAddr,inet_ntoa(in));
			strcpy(InitParamter.m_szUserDataBaseName, szGameUserDB);
			lstrcpyn(InitParamter.m_szMainPage,pInfo->szMainPage,CountArray(InitParamter.m_szMainPage));

			//��������
			InitParamter.SaveInitParamter(false);

			GT_ASSERT(m_pParentWnd!=NULL);
			m_pParentWnd->SendMessage(WM_REQUESTINFOOK);


			return true;
		}
	case SUB_CS_GET_LOGONSERVERINFO_FAILD:
		{
			GT_ASSERT(m_pIEventService!=NULL);
			m_pIEventService->ShowEventNotify(TEXT("���������������"),Level_Exception);

			return true;
		}
	default:
		break;
	}
	return false;
}