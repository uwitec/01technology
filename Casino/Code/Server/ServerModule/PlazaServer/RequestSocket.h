#pragma once
#include "Stdafx.h"

#define WM_REQUESTINFOOK	WM_USER+100

class CRequestSocket: public IClientSocketSink
{
public:
	CRequestSocket(void);
	~CRequestSocket(void);
	//变量定义
protected:
	IEventService						* m_pIEventService;				//事件服务
	CWnd								* m_pParentWnd;
	//组件变量
protected:
	CClientSocketHelper					m_ClientSocketHelper;			//网络连接

	//基础接口
public:
	//释放对象
	virtual bool __cdecl Release() { if (IsValid()) delete this; return true; }
	//是否有效
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CRequestSocket))?true:false; }
	//接口查询
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//网络接口
public:
	//网络连接消息
	virtual bool __cdecl OnSocketConnect(int iErrorCode, LPCTSTR pszErrorDesc, IClientSocket * pIClientSocke);
	//网络读取消息
	virtual bool __cdecl OnSocketRead(CMD_Command Command, void * pBuffer, WORD wDataSize, IClientSocket * pIClientSocke);
	//网络关闭消息
	virtual bool __cdecl OnSocketClose(IClientSocket * pIClientSocke, bool bCloseByServer);

	//网络读取消息
	virtual bool __cdecl OnSocketServerRequest(CMD_Command Command, void * pBuffer, WORD wDataSize);

	//配置函数
public:
	//设置事件
	bool SetEventService(IUnknownEx * pIUnknownEx);
	//设置主窗口
	bool SetParentWnd(CWnd *pParent);

	//功能函数
public:
	//连接服务器
	bool Connect(DWORD dwServerIP, WORD wPort);
	//发送函数
	bool SendData(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize);
	//关闭连接
	bool CloseSocket(bool bNotify);
};
