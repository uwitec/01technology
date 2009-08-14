#include "StdAfx.h"
#include "CenterSocket.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//�����ʶ
#define IREQ_CONNECT					1								//��������
#define IREQ_SEND_DATA					2								//��������
#define IREQ_CLOSE_SOCKET				3								//�ر�����

//////////////////////////////////////////////////////////////////////////
//�ṹ�嶨��

//��������
struct tagReqConnect
{
	WORD								wPort;							//���Ӷ˿�
	DWORD								dwServerIP;						//���ӵ�ַ
};

//��������
struct tagReqSendData
{
	WORD								wMainCmdID;						//��������
	WORD								wSubCmdID;						//��������
	WORD								wDataSize;						//���ݴ�С
	BYTE								cbDataBuffer[SOCKET_PACKAGE];	//��������
};

//�ر�����
struct tagReqCloseSocket
{
	bool								bNotify;						//�Ƿ�֪ͨ
};

//////////////////////////////////////////////////////////////////////////

//���캯��
CCenterSocket::CCenterSocket(void)
{
	//�ӿڱ���
	m_pIEventService=NULL;
	m_pIQueueService=NULL;
	m_pIAsynchronismEngine=NULL;

	return;
}

//��������
CCenterSocket::~CCenterSocket(void)
{
}

//�ӿڲ�ѯ
void * __cdecl CCenterSocket::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IClientSocketSink,Guid,dwQueryVer);
	QUERYINTERFACE(IAsynchronismEngineSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAsynchronismEngineSink,Guid,dwQueryVer);
	return NULL;
}

//�����¼�
bool __cdecl CCenterSocket::OnAsynchronismEngineStart()
{
	//Ч�����
	GT_ASSERT(m_pIEventService!=NULL);
	GT_ASSERT(m_pIQueueService!=NULL);
	GT_ASSERT(m_pIAsynchronismEngine!=NULL);

	//���Ӷ���
	if (m_ClientSocketHelper.GetInterface()==NULL)
	{
		//��������
		bool bSuccess=m_ClientSocketHelper.CreateInstance();
		if (bSuccess==false)
		{
			GT_ASSERT(m_pIEventService!=NULL);
			m_pIEventService->ShowEventNotify(TEXT("CenterServer���Ӷ��󴴽�ʧ��"),Level_Exception);
			return false;
		}

		//���ýӿ�
		IClientSocketSink * pIClientSocketSink=GET_MYSELF_INTERFACE(IClientSocketSink);
		GT_ASSERT(pIClientSocketSink!=NULL);
		if (m_ClientSocketHelper->SetSocketSink(pIClientSocketSink)==false)
		{
			GT_ASSERT(m_pIEventService!=NULL);
			m_pIEventService->ShowEventNotify(TEXT("CenterServer���Ӷ���ص��ӿ�����ʧ��"),Level_Exception);
			return false;
		}
	}

	return true;
}

//ֹͣ�¼�
bool __cdecl CCenterSocket::OnAsynchronismEngineStop()
{
	//�ر�����
	if (m_ClientSocketHelper.GetInterface()!=NULL) 
	{
		m_ClientSocketHelper->CloseSocket(false);
		m_ClientSocketHelper.CloseInstance();
	}

	return true;
}

//�첽����
bool __cdecl CCenterSocket::OnAsynchronismRequest(WORD wRequestID, void * pBuffer, WORD wDataSize)
{
	switch (wRequestID)
	{
	case IREQ_CONNECT:		//��������
		{
			//Ч������
			GT_ASSERT(wDataSize==sizeof(tagReqConnect));
			tagReqConnect * pConnect=(tagReqConnect *)pBuffer;

			//������
			GT_ASSERT(m_ClientSocketHelper.GetInterface()!=NULL);
			m_ClientSocketHelper->Connect(pConnect->dwServerIP,pConnect->wPort);

			return true;
		}
	case IREQ_SEND_DATA:	//��������
		{
			//Ч������
			tagReqSendData * pSendData=(tagReqSendData *)pBuffer;
			GT_ASSERT(wDataSize>=(sizeof(tagReqSendData)-sizeof(pSendData->cbDataBuffer)));
			GT_ASSERT(wDataSize==(sizeof(tagReqSendData)-sizeof(pSendData->cbDataBuffer)+pSendData->wDataSize));

			//������
			GT_ASSERT(m_ClientSocketHelper.GetInterface()!=NULL);
			m_ClientSocketHelper->SendData(pSendData->wMainCmdID,pSendData->wSubCmdID,pSendData->cbDataBuffer,pSendData->wDataSize);

			return true;
		}
	case IREQ_CLOSE_SOCKET:	//�ر�����
		{
			//Ч������
			GT_ASSERT(wDataSize==sizeof(tagReqCloseSocket));
			tagReqCloseSocket * pCloseCocket=(tagReqCloseSocket *)pBuffer;

			//������
			GT_ASSERT(m_ClientSocketHelper.GetInterface()!=NULL);
			m_ClientSocketHelper->CloseSocket(pCloseCocket->bNotify);

			return true;
		};
	default: 
		{
			GT_ASSERT(FALSE);
		}
	}

	return false;
}

//����������Ϣ
bool __cdecl CCenterSocket::OnSocketConnect(int iErrorCode, LPCTSTR pszErrorDesc, IClientSocket * pIClientSocke)
{
	//Ч�����
	GT_ASSERT(m_pIQueueService!=NULL);

	//��������
	NTY_CenterConnect CenterSocketConnect;
	CenterSocketConnect.iErrorCode=iErrorCode;

	//Ͷ����Ϣ
	m_pIQueueService->AddToQueue(EVENT_CENTER_CONNECT,&CenterSocketConnect,sizeof(CenterSocketConnect));

	return true;
}

//�����ȡ��Ϣ
bool __cdecl CCenterSocket::OnSocketRead(CMD_Command Command, void * pBuffer, WORD wDataSize, IClientSocket * pIClientSocke)
{
	//Ч�����
	GT_ASSERT(m_pIQueueService!=NULL);

	//��������
	NTY_CenterRead CenterSocketRead;
	CenterSocketRead.Command=Command;
	CenterSocketRead.wDataSize=wDataSize;
	if (wDataSize>0)
	{
		GT_ASSERT(pBuffer!=NULL);
		GT_ASSERT(wDataSize<=sizeof(CenterSocketRead.cbReadBuffer));
		CopyMemory(CenterSocketRead.cbReadBuffer,pBuffer,wDataSize);
	}

	//Ͷ����Ϣ
	WORD wQueueSize=sizeof(CenterSocketRead)-sizeof(CenterSocketRead.cbReadBuffer)+wDataSize;
	m_pIQueueService->AddToQueue(EVENT_CENTER_READ,&CenterSocketRead,wQueueSize);

	return true;
}

//����ر���Ϣ
bool __cdecl CCenterSocket::OnSocketClose(IClientSocket * pIClientSocke, bool bCloseByServer)
{
	//Ч�����
	GT_ASSERT(m_pIQueueService!=NULL);

	//��������
	NTY_CenterClose CenterSocketClose;
	CenterSocketClose.bCloseByServer=bCloseByServer;

	//Ͷ����Ϣ
	m_pIQueueService->AddToQueue(EVENT_CENTER_CLOSE,&CenterSocketClose,sizeof(CenterSocketClose));

	return true;
}

//�����¼�
bool CCenterSocket::SetEventService(IUnknownEx * pIUnknownEx)
{
	GT_ASSERT(pIUnknownEx!=NULL);
	m_pIEventService=GET_OBJECTPTR_INTERFACE(pIUnknownEx,IEventService);
	GT_ASSERT(m_pIEventService!=NULL);
	return (m_pIEventService!=NULL);
}

//���ö���
bool CCenterSocket::SetQueueService(IUnknownEx * pIUnknownEx)
{
	GT_ASSERT(pIUnknownEx!=NULL);
	m_pIQueueService=GET_OBJECTPTR_INTERFACE(pIUnknownEx,IQueueService);
	GT_ASSERT(m_pIQueueService!=NULL);
	return (m_pIQueueService!=NULL);
}

//�����첽
bool CCenterSocket::SetAsynchronismEngine(IUnknownEx * pIUnknownEx)
{
	GT_ASSERT(pIUnknownEx!=NULL);
	m_pIAsynchronismEngine=GET_OBJECTPTR_INTERFACE(pIUnknownEx,IAsynchronismEngine);
	GT_ASSERT(m_pIAsynchronismEngine!=NULL);
	return (m_pIAsynchronismEngine!=NULL);
}

//���ӷ�����
bool CCenterSocket::Connect(DWORD dwServerIP, WORD wPort)
{
	//��������
	tagReqConnect ReqConnect;
	ReqConnect.wPort=wPort;
	ReqConnect.dwServerIP=dwServerIP;

	//Ͷ������
	GT_ASSERT(m_pIAsynchronismEngine!=NULL);
	IUnknownEx * pIUnknownEx=GET_MYSELF_INTERFACE(IUnknownEx);
	m_pIAsynchronismEngine->InsertRequest(IREQ_CONNECT,&ReqConnect,sizeof(ReqConnect),pIUnknownEx);

	return true;
}

//���ͺ���
bool CCenterSocket::SendData(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize)
{
	//��������
	tagReqSendData ReqSendData;
	ReqSendData.wMainCmdID=wMainCmdID;
	ReqSendData.wSubCmdID=wSubCmdID;
	ReqSendData.wDataSize=wDataSize;
	if (wDataSize>0)
	{
		GT_ASSERT(wDataSize<=sizeof(ReqSendData.cbDataBuffer));
		if (wDataSize>sizeof(ReqSendData.cbDataBuffer)) return false;
		CopyMemory(ReqSendData.cbDataBuffer,pData,wDataSize);
	}

	//Ͷ������
	GT_ASSERT(m_pIAsynchronismEngine!=NULL);
	IUnknownEx * pIUnknownEx=GET_MYSELF_INTERFACE(IUnknownEx);
	WORD wSendSize=sizeof(ReqSendData)-sizeof(ReqSendData.cbDataBuffer)+ReqSendData.wDataSize;
	m_pIAsynchronismEngine->InsertRequest(IREQ_SEND_DATA,&ReqSendData,wSendSize,pIUnknownEx);

	return true;
}

//�ر�����
bool CCenterSocket::CloseSocket(bool bNotify)
{
	//��������
	tagReqCloseSocket ReqCloseSocket;
	ReqCloseSocket.bNotify=bNotify;

	//Ͷ������
	GT_ASSERT(m_pIAsynchronismEngine!=NULL);
	IUnknownEx * pIUnknownEx=GET_MYSELF_INTERFACE(IUnknownEx);
	m_pIAsynchronismEngine->InsertRequest(IREQ_CLOSE_SOCKET,&ReqCloseSocket,sizeof(ReqCloseSocket),pIUnknownEx);

	return true;
}

//////////////////////////////////////////////////////////////////////////
