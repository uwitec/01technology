#ifndef CENTER_SOCKET_HEAD_FILE
#define CENTER_SOCKET_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameServiceExport.h"

//////////////////////////////////////////////////////////////////////////

#define EVENT_CENTER_CONNECT		0x0007								//��������
#define EVENT_CENTER_READ			0x0008								//���Ķ�ȡ
#define EVENT_CENTER_CLOSE			0x0009								//���Ĺر�

//����֪ͨ
struct NTY_CenterConnect
{
	int									iErrorCode;						//�������
};

//��ȡ֪ͨ
struct NTY_CenterRead
{
	CMD_Command							Command;						//������Ϣ
	WORD								wDataSize;						//���ݴ�С
	BYTE								cbReadBuffer[SOCKET_PACKAGE];	//��������
};

//�ر�֪ͨ
struct NTY_CenterClose
{
	bool								bCloseByServer;					//�ر�ģʽ
};

//////////////////////////////////////////////////////////////////////////

//����������
class CCenterSocket : public IAsynchronismEngineSink, public IClientSocketSink
{
	//��������
protected:
	IEventService						* m_pIEventService;				//�¼�����
	IQueueService						* m_pIQueueService;				//���з���
	IAsynchronismEngine					* m_pIAsynchronismEngine;		//�첽����

	//�������
protected:
	CClientSocketHelper					m_ClientSocketHelper;			//��������

	//��������
public:
	//���캯��
	CCenterSocket();
	//��������
	virtual ~CCenterSocket();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual bool __cdecl Release() { if (IsValid()) delete this; return true; }
	//�Ƿ���Ч
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CCenterSocket))?true:false; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//�첽�ӿ�
public:
	//�����¼�
	virtual bool __cdecl OnAsynchronismEngineStart();
	//ֹͣ�¼�
	virtual bool __cdecl OnAsynchronismEngineStop();
	//�첽����
	virtual bool __cdecl OnAsynchronismRequest(WORD wRequestID, void * pBuffer, WORD wDataSize);

	//����ӿ�
public:
	//����������Ϣ
	virtual bool __cdecl OnSocketConnect(int iErrorCode, LPCTSTR pszErrorDesc, IClientSocket * pIClientSocke);
	//�����ȡ��Ϣ
	virtual bool __cdecl OnSocketRead(CMD_Command Command, void * pBuffer, WORD wDataSize, IClientSocket * pIClientSocke);
	//����ر���Ϣ
	virtual bool __cdecl OnSocketClose(IClientSocket * pIClientSocke, bool bCloseByServer);

	//���ú���
public:
	//�����¼�
	bool SetEventService(IUnknownEx * pIUnknownEx);
	//���ö���
	bool SetQueueService(IUnknownEx * pIUnknownEx);
	//�����첽
	bool SetAsynchronismEngine(IUnknownEx * pIUnknownEx);

	//���ܺ���
public:
	//���ӷ�����
	bool Connect(DWORD dwServerIP, WORD wPort);
	//���ͺ���
	bool SendData(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize);
	//�ر�����
	bool CloseSocket(bool bNotify);
};

//////////////////////////////////////////////////////////////////////////

#endif