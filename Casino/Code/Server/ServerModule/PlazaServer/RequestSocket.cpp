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
//接口查询
void * __cdecl CRequestSocket::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IClientSocketSink,Guid,dwQueryVer);
	return NULL;
}
//网络连接消息
bool __cdecl CRequestSocket::OnSocketConnect(int iErrorCode, LPCTSTR pszErrorDesc, IClientSocket * pIClientSocke)
{
	if(iErrorCode == 0)
	{
		GT_ASSERT(m_pIEventService!=NULL);
		m_pIEventService->ShowEventNotify(TEXT("中心服务器连接成功"),Level_Normal);
		m_pIEventService->ShowEventNotify(TEXT("正在请求登陆信息"),Level_Normal);

		CMD_CS_GetLogonServerInfo sendData;
		//加载参数
		CInitParamter InitParamter;
		InitParamter.LoadInitParamter();

		//变量定义
		TCHAR szWriteBuffer[256]=TEXT("");
		CXOREncrypt::EncryptData(InitParamter.m_szLogonServerPass,szWriteBuffer,sizeof(szWriteBuffer));
		lstrcpyn(sendData.szLogonServerPass,szWriteBuffer,CountArray(sendData.szLogonServerPass));

		return SendData(MDM_CS_SERVER_REQUEST, SUB_CS_GET_LOGONSERVERINFO, &sendData, sizeof(sendData));
	}
	else
	{
		GT_ASSERT(m_pIEventService!=NULL);
		m_pIEventService->ShowEventNotify(TEXT("中心服务器连接失败"),Level_Exception);
		return false;
	}
}

//网络读取消息
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

//网络关闭消息
bool __cdecl CRequestSocket::OnSocketClose(IClientSocket * pIClientSocke, bool bCloseByServer)
{
	GT_ASSERT(m_pIEventService!=NULL);
	m_pIEventService->ShowEventNotify(TEXT("中心服务器连接断开"),Level_Exception);

	return true;
}
//设置事件
bool CRequestSocket::SetEventService(IUnknownEx * pIUnknownEx)
{
	GT_ASSERT(pIUnknownEx!=NULL);
	m_pIEventService=GET_OBJECTPTR_INTERFACE(pIUnknownEx,IEventService);
	GT_ASSERT(m_pIEventService!=NULL);
	return (m_pIEventService!=NULL);
}
//设置主窗口
bool CRequestSocket::SetParentWnd(CWnd *pParent)
{
	m_pParentWnd = pParent;
	return (m_pParentWnd!=NULL);
}
//连接服务器
bool CRequestSocket::Connect(DWORD dwServerIP, WORD wPort)
{
	CloseSocket(false);

	//连接对象
	if (m_ClientSocketHelper.GetInterface()==NULL)
	{
		//创建对象
		bool bSuccess=m_ClientSocketHelper.CreateInstance();
		if (bSuccess==false)
		{
			GT_ASSERT(m_pIEventService!=NULL);
			m_pIEventService->ShowEventNotify(TEXT("中心服务器连接对象创建失败"),Level_Exception);
			return false;
		}

		//设置接口
		IClientSocketSink * pIClientSocketSink=GET_MYSELF_INTERFACE(IClientSocketSink);
		GT_ASSERT(pIClientSocketSink!=NULL);
		if (m_ClientSocketHelper->SetSocketSink(pIClientSocketSink)==false)
		{
			GT_ASSERT(m_pIEventService!=NULL);
			m_pIEventService->ShowEventNotify(TEXT("中心服务器连接对象回调接口设置失败"),Level_Exception);
			return false;

		}
	}

	//请求处理
	GT_ASSERT(m_ClientSocketHelper.GetInterface()!=NULL);
	return m_ClientSocketHelper->Connect(dwServerIP,wPort);
}

//发送函数
bool CRequestSocket::SendData(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize)
{
	//请求处理
	GT_ASSERT(m_ClientSocketHelper.GetInterface()!=NULL);
	return m_ClientSocketHelper->SendData(wMainCmdID,wSubCmdID,pData,wDataSize);
}

//关闭连接
bool CRequestSocket::CloseSocket(bool bNotify)
{
	//关闭连接
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
			m_pIEventService->ShowEventNotify(TEXT("请求登陆信息成功"),Level_Normal);

			//获取输入
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

			//保存设置
			InitParamter.SaveInitParamter(false);

			GT_ASSERT(m_pParentWnd!=NULL);
			m_pParentWnd->SendMessage(WM_REQUESTINFOOK);


			return true;
		}
	case SUB_CS_GET_LOGONSERVERINFO_FAILD:
		{
			GT_ASSERT(m_pIEventService!=NULL);
			m_pIEventService->ShowEventNotify(TEXT("启动服务密码错误"),Level_Exception);

			return true;
		}
	default:
		break;
	}
	return false;
}