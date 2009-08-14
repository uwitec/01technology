#pragma once

const DWORD WM_MSG_STATUS  = WM_USER + 100;
const WORD wNoticeCode_Error = 0;
const WORD wNoticeCode_UpdateNetAddr = 1;
const WORD wNoticeCode_RequestTimedOut = 2;
const WORD wNoticeCode_UpdateNetDelay  = 3;

struct tagNoticeHeader 
{
	WORD	wNoticeCode;
	DWORD	dwContext;
};

struct tagNoticeError
{
	tagNoticeHeader Header;
	DWORD			dwSocketError;
	TCHAR			szError[64];
};
struct tagNoticeUpdateNetAddr
{
	tagNoticeHeader Header;
	TCHAR			szNetAddr[64];
};
struct tagNoticeUpdateNetDelay
{
	tagNoticeHeader Header;
	DWORD			dwElapsed;
};
//////////////////////////////////////////////////////////////////////////
#pragma pack(1)

#define ICMP_ECHOREPLY	0
#define ICMP_ECHOREQ	8

typedef struct tagIPHDR
{
	u_char  VIHL;			
	u_char	TOS;			
	short	TotLen;			
	short	ID;				
	short	FlagOff;		
	u_char	TTL;			
	u_char	Protocol;		
	u_short	Checksum;		
	struct	in_addr iaSrc;	
	struct	in_addr iaDst;	
}IPHDR, *PIPHDR;

typedef struct tagICMPHDR
{
	u_char	Type;			
	u_char	Code;			
	u_short	Checksum;		
	u_short	ID;				
	u_short	Seq;			
	char	Data;			
}ICMPHDR, *PICMPHDR;

#define REQ_DATASIZE 32		
typedef struct tagECHOREQUEST
{
	ICMPHDR icmpHdr;
	DWORD	dwTime;
	char	cData[REQ_DATASIZE];
}ECHOREQUEST, *PECHOREQUEST;

typedef struct tagECHOREPLY
{
	IPHDR	ipHdr;
	ECHOREQUEST	echoRequest;
	char    cFiller[256];
}ECHOREPLY, *PECHOREPLY;
#pragma pack()
//////////////////////////////////////////////////////////////////////////
struct tagPingTask
{
	TCHAR szHost[128];
	DWORD dwContext;
};
typedef CArray<tagPingTask, tagPingTask> tagPingTask_Array;

class CPingThread: public CServiceThread
{
public:
	CPingThread(void);
	~CPingThread(void);

	void InitPing(HWND hWnd,BOOL *bContinue);
	void AddTask(LPCSTR lpstrHost,
	DWORD dwContext);
private:
	//Runº¯Êý
	virtual bool RepetitionRun();
protected:	
	void	UpdateContext(DWORD dwContext);
	void	Ping(UINT nRetries,LPCSTR pstrHost);

	int		WaitForEchoReply(SOCKET s);
	int		SendEchoRequest(SOCKET, LPSOCKADDR_IN);
	DWORD	RecvEchoReply(SOCKET, LPSOCKADDR_IN, u_char *, DWORD*);
	u_short in_cksum(u_short *addr, int len);

	void UpdateNetDelay(DWORD dwElapsed, DWORD dwContext);
	void RequestTimedOut();
	void UpdateNetAddr(LPCSTR pstrAddr);
	void WSAError(LPCSTR pstrFrom);
protected:
	HWND	m_hWnd;
	DWORD	m_dwContext;
	BOOL*	m_bContinue;
	ECHOREQUEST echoReq;
	u_short nId;
	u_short nSeq;
	tagPingTask_Array	m_tagPingTask_Array;
	int					m_nCurTaskIndex;
	CThreadLock			m_TaskLock;
};
