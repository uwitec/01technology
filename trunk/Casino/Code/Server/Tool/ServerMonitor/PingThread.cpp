#include "StdAfx.h"
#include ".\pingthread.h"

CPingThread::CPingThread(void)
{
	m_hWnd = 0;
	m_dwContext = 0;
	m_bContinue = NULL;
	m_nCurTaskIndex = 0;
}

CPingThread::~CPingThread(void)
{
}
bool CPingThread::RepetitionRun()
{
	m_TaskLock.Lock();
	if(m_tagPingTask_Array.GetCount())
	{
		m_nCurTaskIndex = m_nCurTaskIndex % m_tagPingTask_Array.GetCount();
		tagPingTask task = m_tagPingTask_Array.GetAt(m_nCurTaskIndex ++);
		m_TaskLock.UnLock();

		UpdateContext(task.dwContext);
		Ping(1, task.szHost);
		return *m_bContinue;
	}
	m_TaskLock.UnLock();
	return *m_bContinue;
}

void CPingThread::InitPing(HWND hWnd,BOOL *bContinue)
{
	nId = 1;
	nSeq = 1;

	ASSERT(hWnd);
	ASSERT(::IsWindow(hWnd));
	ASSERT(bContinue);
	m_hWnd = hWnd;
	m_bContinue = bContinue;

	m_nCurTaskIndex = 0;
	m_tagPingTask_Array.RemoveAll();
}
void CPingThread::AddTask(LPCSTR lpstrHost,
						  DWORD dwContext)
{
	CThreadLockHandle lock(&m_TaskLock);
	tagPingTask task;
	task.dwContext = dwContext;
	_tcscpy(task.szHost, lpstrHost);
	m_tagPingTask_Array.Add(task);
}
void CPingThread::UpdateContext(DWORD dwContext)
{
	m_dwContext = dwContext;
}
void CPingThread::Ping(UINT nRetries,LPCSTR pstrHost)
{
	SOCKET	  rawSocket;
	LPHOSTENT lpHost;
	UINT	  nLoop;
	int       nRet;
	struct    sockaddr_in saDest;
	struct    sockaddr_in saSrc;
	DWORD	  dwTimeSent;
	DWORD	  dwElapsed;
	u_char    cTTL;

	ASSERT(m_bContinue);

	lpHost = gethostbyname(pstrHost);
	if (lpHost == NULL)
	{
		WSAError(TEXT("找不到主机"));
		return;
	}
	saDest.sin_addr.s_addr = *((u_long FAR *) (lpHost->h_addr));
	saDest.sin_family = AF_INET;
	saDest.sin_port = 0;

	UpdateNetAddr(inet_ntoa(saDest.sin_addr));

	CString str;
	rawSocket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (rawSocket == SOCKET_ERROR) 
	{
		WSAError(TEXT("创建Socket异常"));
		return;
	}

	for (nLoop = 0; nLoop < nRetries && *m_bContinue == TRUE; nLoop++)
	{
		SendEchoRequest(rawSocket, &saDest);

		nRet = WaitForEchoReply(rawSocket);
		if (nRet == SOCKET_ERROR)
		{
			WSAError(TEXT("监听网络异常"));
			break;
		}
		if (!nRet)
		{
			RequestTimedOut();
		}
		else
		{
			DWORD dwContext;
			dwTimeSent = RecvEchoReply(rawSocket, &saSrc, &cTTL,&dwContext);
			if(dwTimeSent != (DWORD)(-1))
			{
				dwElapsed = GetTickCount() - dwTimeSent;
#ifdef _DEBUG
				str.Format("Thread[0x%x]Reply[%d] from: %s: bytes=%d time=%ldms TTL=%d\n", 
					GetCurrentThreadId(),
					nLoop+1,
					inet_ntoa(saSrc.sin_addr), 
					REQ_DATASIZE,
					dwElapsed,
					cTTL);
				TRACE(str);
#endif
				UpdateNetDelay(dwElapsed, dwContext);

				Sleep(1000);
			}
		}
	}

	nRet = closesocket(rawSocket);
	if (nRet == SOCKET_ERROR)
		WSAError(TEXT("关闭Socket异常"));

}


int CPingThread::SendEchoRequest(SOCKET s,LPSOCKADDR_IN lpstToAddr) 
{
	int nRet;
	memset(&echoReq, 0, sizeof(echoReq));
	echoReq.icmpHdr.Type		= ICMP_ECHOREQ;
	echoReq.icmpHdr.Code		= 0;
	echoReq.icmpHdr.Checksum	= 0;
	echoReq.icmpHdr.ID			= m_dwContext;
	echoReq.icmpHdr.Seq			= nSeq++;

	for (nRet = 0; nRet < REQ_DATASIZE; nRet++)
		echoReq.cData[nRet] = ' '+nRet;

	echoReq.dwTime				= GetTickCount();
	echoReq.icmpHdr.Checksum = in_cksum((u_short *)&echoReq, sizeof(ECHOREQUEST));

	nRet = sendto(s,				
		(LPSTR)&echoReq,		
		sizeof(ECHOREQUEST),
		0,						
		(LPSOCKADDR)lpstToAddr,
		sizeof(SOCKADDR_IN));  

	if (nRet == SOCKET_ERROR) 
		WSAError(TEXT("发送数据异常"));
	return (nRet);
}


DWORD CPingThread::RecvEchoReply(SOCKET s, LPSOCKADDR_IN lpsaFrom, u_char *pTTL, DWORD * pdwContext) 
{
	ECHOREPLY echoReply;
	memset(&echoReply, 0, sizeof(echoReply));
	int nRet;
	int nAddrLen = sizeof(struct sockaddr_in);

	nRet = recvfrom(s,			
		(LPSTR)&echoReply,
		sizeof(ECHOREPLY),	
		0,					
		(LPSOCKADDR)lpsaFrom,
		&nAddrLen);		

	if (nRet == SOCKET_ERROR) 
		WSAError(TEXT("接收数据异常"));

	//if(m_dwContext == echoReply.echoRequest.icmpHdr.ID &&
	//	nSeq == echoReply.echoRequest.icmpHdr.Seq+1)
	//{
		*pTTL = echoReply.ipHdr.TTL;
		*pdwContext = echoReply.echoRequest.icmpHdr.ID;
		return(echoReply.echoRequest.dwTime);   		
	//}
	//	else
	//{
	//	return (DWORD)(-1);
	//}
}


int CPingThread::WaitForEchoReply(SOCKET s)
{
	struct timeval Timeout;
	fd_set readfds;

	readfds.fd_count = 1;
	readfds.fd_array[0] = s;
	Timeout.tv_sec = 1;
	Timeout.tv_usec = 0;

	return(select(1, &readfds, NULL, NULL, &Timeout));
}

u_short CPingThread::in_cksum(u_short *addr, int len)
{
	register int nleft = len;
	register u_short *w = addr;
	register u_short answer;
	register int sum = 0;

	while( nleft > 1 )  
	{
		sum += *w++;
		nleft -= 2;
	}

	if( nleft == 1 )
	{
		u_short	u = 0;
		*(u_char *)(&u) = *(u_char *)w ;
		sum += u;
	}

	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);	
	answer = ~sum;
	return (answer);
}
void CPingThread::UpdateNetDelay(DWORD dwElapsed, DWORD dwContext)
{
	tagNoticeUpdateNetDelay *pNotice = new tagNoticeUpdateNetDelay;
	pNotice->Header.dwContext  = dwContext;
	pNotice->Header.wNoticeCode = wNoticeCode_UpdateNetDelay;
	pNotice->dwElapsed = dwElapsed;
	::PostMessage(m_hWnd,WM_MSG_STATUS, sizeof(tagNoticeUpdateNetDelay), (LPARAM)pNotice);
}
void CPingThread::RequestTimedOut()
{
	tagNoticeUpdateNetDelay *pNotice = new tagNoticeUpdateNetDelay;
	pNotice->Header.dwContext = m_dwContext;
	pNotice->Header.wNoticeCode = wNoticeCode_RequestTimedOut;
	::PostMessage(m_hWnd,WM_MSG_STATUS, sizeof(tagNoticeUpdateNetDelay), (LPARAM)pNotice);
}
void CPingThread::UpdateNetAddr(LPCSTR pstrAddr)
{
	tagNoticeUpdateNetAddr *pNotice = new tagNoticeUpdateNetAddr;
	pNotice->Header.dwContext = m_dwContext;
	pNotice->Header.wNoticeCode = wNoticeCode_UpdateNetAddr;
	_tcscpy(pNotice->szNetAddr, pstrAddr);
	::PostMessage(m_hWnd,WM_MSG_STATUS, sizeof(tagNoticeUpdateNetAddr), (LPARAM)pNotice);
}
void CPingThread::WSAError(LPCSTR lpMsg)
{
	tagNoticeError *pNotice = new tagNoticeError;
	pNotice->Header.dwContext = m_dwContext;
	pNotice->Header.wNoticeCode = wNoticeCode_Error;
	_tcscpy(pNotice->szError, lpMsg);
	pNotice->dwSocketError = WSAGetLastError();
	::PostMessage(m_hWnd,WM_MSG_STATUS, sizeof(tagNoticeError), (LPARAM)pNotice);
}
