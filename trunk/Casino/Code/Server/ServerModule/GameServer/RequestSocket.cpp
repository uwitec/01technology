#include "stdafx.h"
#include ".\requestsocket.h"
#include ".\InitParamter.h"
#include "GameServiceHelperContainer.h"

CRequestSocket::CRequestSocket(void)
{
	m_pIEventService = NULL;
	m_pParentContainer	 = NULL;
	m_pGameServiceParameter = NULL;
	m_pInitParamter = NULL;
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
	GT_ASSERT(m_pIEventService!=NULL);
	TCHAR szDescribe[128]=TEXT("");
	if(iErrorCode == 0)
	{
		_snprintf(szDescribe,sizeof(szDescribe),TEXT("������ID %ld�����ķ��������ӳɹ�"),(LONG)m_pInitParamter->m_wServerID);
		m_pIEventService->ShowEventNotify(szDescribe,Level_Normal);
		_snprintf(szDescribe,sizeof(szDescribe),TEXT("������ID %ld�����������½��Ϣ"),(LONG)m_pInitParamter->m_wServerID);
		m_pIEventService->ShowEventNotify(szDescribe,Level_Normal);

		CMD_CS_GetLoaderServerInfo sendData;


		//��������
		TCHAR szWriteBuffer[256]=TEXT("");
		CXOREncrypt::EncryptData(m_pInitParamter->m_szLoaderServerPass,szWriteBuffer,sizeof(szWriteBuffer));
		lstrcpyn(sendData.szLoaderServerPass,szWriteBuffer,CountArray(sendData.szLoaderServerPass));
		sendData.wServerID	= m_pInitParamter->m_wServerID;

		return SendData(MDM_CS_SERVER_REQUEST, SUB_CS_GET_LOADERSERVERINFO, &sendData, sizeof(sendData));
	}
	else
	{
		_snprintf(szDescribe,sizeof(szDescribe),TEXT("������ID %ld��CenterServer����ʧ��"),(LONG)m_pInitParamter->m_wServerID);
		m_pIEventService->ShowEventNotify(szDescribe,Level_Warning);
		return false;
	}
}

//�����ȡ��Ϣ
bool __cdecl CRequestSocket::OnSocketRead(CMD_Command Command, void * pBuffer, WORD wDataSize, IClientSocket * pIClientSocke)
{
	switch(Command.wMainCmdID) {
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
	TCHAR szDescribe[128]=TEXT("");
	_snprintf(szDescribe,sizeof(szDescribe),TEXT("������ID %ld��CenterServer���ӶϿ�"),(LONG)m_pInitParamter->m_wServerID);
	m_pIEventService->ShowEventNotify(szDescribe,Level_Exception);

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
//����
bool CRequestSocket::SetParentContainer(CGameServiceHelperContainer *pParent, 
										tagGameServiceParameter*	pGameServiceParameter,
										CInitParamter*	pInitParamter)
{
	m_pParentContainer = pParent;
	m_pGameServiceParameter = pGameServiceParameter;
	m_pInitParamter = pInitParamter;
	return (m_pParentContainer!=NULL);
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
			TCHAR szDescribe[128]=TEXT("");
			_snprintf(szDescribe,sizeof(szDescribe),TEXT("������ID %ld��CenterServer���Ӷ��󴴽�ʧ��"),(LONG)m_pInitParamter->m_wServerID);
			m_pIEventService->ShowEventNotify(szDescribe,Level_Exception);
			return false;
		}

		//���ýӿ�
		IClientSocketSink * pIClientSocketSink=GET_MYSELF_INTERFACE(IClientSocketSink);
		GT_ASSERT(pIClientSocketSink!=NULL);
		if (m_ClientSocketHelper->SetSocketSink(pIClientSocketSink)==false)
		{
			GT_ASSERT(m_pIEventService!=NULL);
			TCHAR szDescribe[128]=TEXT("");
			_snprintf(szDescribe,sizeof(szDescribe),TEXT("������ID %ld��CenterServer���Ӷ���ص��ӿ�����ʧ��"),(LONG)m_pInitParamter->m_wServerID);
			m_pIEventService->ShowEventNotify(szDescribe,Level_Exception);
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
	case SUB_CS_GET_LOADERSERVERINFO_SUCCESS:
		{

			GT_ASSERT(wDataSize >= sizeof(CMD_CS_LoaderServerInfo));

			CMD_CS_LoaderServerInfo *pInfo = (CMD_CS_LoaderServerInfo*)pBuffer;
			GT_ASSERT(m_pIEventService!=NULL);
			TCHAR szDescribe[128]=TEXT("");
			_snprintf(szDescribe,sizeof(szDescribe),TEXT("������ID %ld�������½��Ϣ�ɹ�"),(LONG)m_pInitParamter->m_wServerID);
			m_pIEventService->ShowEventNotify(szDescribe,Level_Normal);

			GT_ASSERT(m_pGameServiceParameter);
			memcpy(m_pGameServiceParameter, pInfo, sizeof(CMD_CS_LoaderServerInfo));


			CXOREncrypt::CrevasseData(pInfo->GameUserDBInfo.szDataBasePipeName,m_pInitParamter->m_szGameUserDBPipeName,CountArray(m_pInitParamter->m_szGameUserDBPipeName));
			CXOREncrypt::CrevasseData(pInfo->GameUserDBInfo.szDataBaseUser,m_pInitParamter->m_szGameUserDBUser,CountArray(m_pInitParamter->m_szGameUserDBUser));
			CXOREncrypt::CrevasseData(pInfo->GameUserDBInfo.szDataBasePass,m_pInitParamter->m_szGameUserDBPass,CountArray(m_pInitParamter->m_szGameUserDBPass));
			m_pInitParamter->m_wGameUserDBPort = pInfo->GameUserDBInfo.wDataBasePort;
			struct in_addr in;
			in.S_un.S_addr	= pInfo->GameUserDBInfo.dwDataBaseAddr;
			strcpy(m_pInitParamter->m_szGameUserDBAddr,inet_ntoa(in));
			strcpy(m_pInitParamter->m_szGameUserDBName, szGameUserDB);

			CXOREncrypt::CrevasseData(pInfo->GameScoreDBInfo.szDataBasePipeName,m_pInitParamter->m_szServerInfoDBPipeName,CountArray(m_pInitParamter->m_szServerInfoDBPipeName));
			CXOREncrypt::CrevasseData(pInfo->GameScoreDBInfo.szDataBaseUser,m_pInitParamter->m_szServerInfoDBUser,CountArray(m_pInitParamter->m_szServerInfoDBUser));
			CXOREncrypt::CrevasseData(pInfo->GameScoreDBInfo.szDataBasePass,m_pInitParamter->m_szServerInfoDBPass,CountArray(m_pInitParamter->m_szServerInfoDBPass));
			m_pInitParamter->m_wServerInfoDBPort = pInfo->GameScoreDBInfo.wDataBasePort;
			in.S_un.S_addr	= pInfo->GameScoreDBInfo.dwDataBaseAddr;
			strcpy(m_pInitParamter->m_szServerInfoDBAddr,inet_ntoa(in));
			strcpy(m_pInitParamter->m_szServerInfoDBName, szGameUserDB);


			GT_ASSERT(m_pParentContainer!=NULL);
			m_pParentContainer->OnRequestInfoOk(0,0);


			return true;
		}
	case SUB_CS_GET_LOADERSERVERINFO_FAILD:
		{
			GT_ASSERT(m_pIEventService!=NULL);
			TCHAR szDescribe[128]=TEXT("");
			_snprintf(szDescribe,sizeof(szDescribe),TEXT("������ID %ld����������ʧ��"),(LONG)m_pInitParamter->m_wServerID);
			m_pIEventService->ShowEventNotify(szDescribe,Level_Exception);

			return true;
		}
	default:
		break;
	}
	return false;
}