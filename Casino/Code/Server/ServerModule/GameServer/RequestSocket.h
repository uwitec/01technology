#pragma once
#include "Stdafx.h"

class CGameServiceHelperContainer;
class CInitParamter;

class CRequestSocket: public IClientSocketSink
{
public:
	CRequestSocket(void);
	~CRequestSocket(void);
	//��������
protected:
	IEventService						* m_pIEventService;				//�¼�����
	CGameServiceHelperContainer			* m_pParentContainer;
	//�������
protected:
	CClientSocketHelper					m_ClientSocketHelper;			//��������
	tagGameServiceParameter*			m_pGameServiceParameter;
	CInitParamter*						m_pInitParamter;
	//�����ӿ�
public:
	//�ͷŶ���
	virtual bool __cdecl Release() { if (IsValid()) delete this; return true; }
	//�Ƿ���Ч
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CRequestSocket))?true:false; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//����������Ϣ
	virtual bool __cdecl OnSocketConnect(int iErrorCode, LPCTSTR pszErrorDesc, IClientSocket * pIClientSocke);
	//�����ȡ��Ϣ
	virtual bool __cdecl OnSocketRead(CMD_Command Command, void * pBuffer, WORD wDataSize, IClientSocket * pIClientSocke);
	//����ر���Ϣ
	virtual bool __cdecl OnSocketClose(IClientSocket * pIClientSocke, bool bCloseByServer);

	//�����ȡ��Ϣ
	virtual bool __cdecl OnSocketServerRequest(CMD_Command Command, void * pBuffer, WORD wDataSize);

	//���ú���
public:
	//�����¼�
	bool SetEventService(IUnknownEx * pIUnknownEx);
	//����������
	bool SetParentContainer(CGameServiceHelperContainer *pParent, 
						tagGameServiceParameter*	pGameServiceParameter,
						CInitParamter*	pInitParamter);

	//���ܺ���
public:
	//���ӷ�����
	bool Connect(DWORD dwServerIP, WORD wPort);
	//���ͺ���
	bool SendData(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize);
	//�ر�����
	bool CloseSocket(bool bNotify);
};