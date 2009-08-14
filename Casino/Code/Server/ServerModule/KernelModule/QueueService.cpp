#include "StdAfx.h"
#include "QueueService.h"

//////////////////////////////////////////////////////////////////////////

//构造函数
CQueueServiceThread::CQueueServiceThread(void)
{
	m_hCompletionPort=NULL;
	memset(m_cbBuffer,0,sizeof(m_cbBuffer));
	m_cbThreadIndex = 0;
}

//析构函数
CQueueServiceThread::~CQueueServiceThread(void)
{
}

//配置函数
bool CQueueServiceThread::InitThread(HANDLE hCompletionPort,BYTE cbThreadIndex)
{
	//效验参数
	GT_ASSERT(IsRuning()==false);
	GT_ASSERT(m_hCompletionPort==NULL);

	//设置变量
	m_hCompletionPort=hCompletionPort;
	memset(m_cbBuffer,0,sizeof(m_cbBuffer));
	m_cbThreadIndex = cbThreadIndex;

	return true;
}

//取消配置
bool CQueueServiceThread::UnInitThread()
{
	//效验参数
	GT_ASSERT(IsRuning()==false);

	//设置变量
	m_hCompletionPort=NULL;
	memset(m_cbBuffer,0,sizeof(m_cbBuffer));

	return true;
}

//Run函数
bool CQueueServiceThread::RepetitionRun()
{
	//效验参数
	GT_ASSERT(m_hCompletionPort!=NULL);

	//变量定义
	DWORD dwThancferred=0;
	OVERLAPPED * pOverLapped=NULL;
	CQueueService * pQueueService=NULL;

	//等待完成端口
	if (GetQueuedCompletionStatus(m_hCompletionPort,&dwThancferred,(PULONG_PTR)&pQueueService,&pOverLapped,INFINITE))
	{
		//判断退出
		if (pQueueService==NULL) return false;

		//获取数据
		tagDataHead DataHead;
		bool bSuccess=pQueueService->GetData(DataHead,m_cbBuffer,sizeof(m_cbBuffer));
		GT_ASSERT(bSuccess==true);

		//处理数据
		if (bSuccess==true) pQueueService->OnQueueServiceThread(m_cbThreadIndex, DataHead,m_cbBuffer,DataHead.wDataSize);

		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////

//构造函数
CQueueService::CQueueService(void)
{
	m_bService=false;
	m_hCompletionPort=NULL;
	m_QueueServiceThread=NULL;
	m_pIQueueServiceSink=NULL;
	m_cbThreadCount=1;
}

//析构函数
CQueueService::~CQueueService(void)
{
	//停止服务
	StopService();

	return;
}

//接口查询
void * __cdecl CQueueService::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IQueueService,Guid,dwQueryVer);
	QUERYINTERFACE(IQueueServiceEngine,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IQueueServiceEngine,Guid,dwQueryVer);
	return NULL;
}

//设置接口
bool __cdecl CQueueService::SetQueueServiceSink(IUnknownEx * pIUnknownEx)
{
	GT_ASSERT(pIUnknownEx!=NULL);
	m_pIQueueServiceSink=GET_OBJECTPTR_INTERFACE(pIUnknownEx,IQueueServiceSink);
	GT_ASSERT(m_pIQueueServiceSink!=NULL);
	return (m_pIQueueServiceSink!=NULL);
}

//负荷信息
bool __cdecl CQueueService::GetBurthenInfo(tagBurthenInfo & BurthenInfo)
{
	CThreadLockHandle LockHandle(&m_ThreadLock);
	return m_DataStorage.GetBurthenInfo(BurthenInfo);
}

//加入数据
bool __cdecl CQueueService::AddToQueue(WORD wIdentifier, void * const pBuffer, WORD wDataSize)
{
	GT_ASSERT(m_cbThreadCount);
	CThreadLockHandle LockHandle(&m_ThreadLock);
	m_DataStorage.AddData(wIdentifier,pBuffer,wDataSize);
	BYTE n = rand() % m_cbThreadCount;
	PostQueuedCompletionStatus(m_hCompletionPort[n],wDataSize,(ULONG_PTR)this,NULL);
	return true;
}

//开始服务
bool __cdecl CQueueService::StartService(BYTE cbThreadCount)
{
	//效验参数
	GT_ASSERT(m_bService==false);
	GT_ASSERT(m_hCompletionPort==NULL);
	GT_ASSERT(m_pIQueueServiceSink!=NULL);

	m_cbThreadCount = cbThreadCount;

	m_hCompletionPort	 = new HANDLE[m_cbThreadCount];
	memset(m_hCompletionPort, 0, sizeof(HANDLE) * m_cbThreadCount);
	m_QueueServiceThread = new CQueueServiceThread[m_cbThreadCount];

	for(BYTE i = 0; i < m_cbThreadCount; i ++)
	{
		//建立完成端口
		m_hCompletionPort[i]=CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,NULL,1);
		if (m_hCompletionPort[i]==NULL) throw TEXT("队列对象完成端口创建失败");

		//启动线程
		if (m_QueueServiceThread[i].InitThread(m_hCompletionPort[i], i)==false) throw TEXT("队列对象线程初始化失败");
		if (m_QueueServiceThread[i].StartThead()==false) throw TEXT("队列对象线程启动失败");

	}

	//设置参数
	m_bService=true;

	return true;
}

//停止服务
bool __cdecl CQueueService::StopService()
{
	//设置变量
	m_bService=false;

	if(m_hCompletionPort)
	{
		//停止线程
		for(BYTE i = 0; i < m_cbThreadCount; i ++)
		{
			if (m_hCompletionPort[i]!=NULL) PostQueuedCompletionStatus(m_hCompletionPort[i],0,NULL,NULL);
			m_QueueServiceThread[i].StopThread();
			m_QueueServiceThread[i].UnInitThread();
		}

		for( i = 0; i < m_cbThreadCount; i ++)
		{
			//关闭完成端口
			if (m_hCompletionPort[i]!=NULL)
			{
				CloseHandle(m_hCompletionPort[i]);
				m_hCompletionPort[i]=NULL;
			}
		}
		delete[] m_hCompletionPort;
		m_hCompletionPort = NULL;
		delete[] m_QueueServiceThread;
		m_QueueServiceThread = NULL;
	}

	//设置数据
	m_DataStorage.RemoveData(false);

	return true;
}

//提取数据
bool CQueueService::GetData(tagDataHead & DataHead, void * pBuffer, WORD wBufferSize)
{
	CThreadLockHandle LockHandle(&m_ThreadLock);
	return m_DataStorage.GetData(DataHead,pBuffer,wBufferSize);
}

//数据消息
void CQueueService::OnQueueServiceThread(BYTE cbThreadIndex, const tagDataHead & DataHead, void * pBuffer, WORD wDataSize)
{
	GT_ASSERT(m_pIQueueServiceSink!=NULL);
	_BEGIN_TRY	
	{ 
		m_pIQueueServiceSink->OnQueueServiceSink(cbThreadIndex,DataHead.wIdentifier,pBuffer,DataHead.wDataSize,DataHead.dwInsertTime); 
	}
	CATCH_COMMON_EXCEPTION(;)
		CATCH_UNKNOWN_EXCEPTION(;)_END_CATCH
	return;
}

//////////////////////////////////////////////////////////////////////////

//建立对象函数
extern "C" __declspec(dllexport) void * __cdecl CreateQueueService(const GUID & Guid, DWORD dwInterfaceVer)
{
	//建立对象
	CQueueService * pQueueService=NULL;
	_BEGIN_TRY
	{
		pQueueService=new CQueueService();
		if (pQueueService==NULL) throw TEXT("创建失败");
		void * pObject=pQueueService->QueryInterface(Guid,dwInterfaceVer);
		if (pObject==NULL) throw TEXT("接口查询失败");
		return pObject;
	}
	CATCH_COMMON_EXCEPTION(;)CATCH_UNKNOWN_EXCEPTION(;)_END_CATCH

	//清理对象
	SafeDelete(pQueueService);
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
