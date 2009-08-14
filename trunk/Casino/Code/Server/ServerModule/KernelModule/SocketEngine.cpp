#include "StdAfx.h"
#include "EventService.h"
#include "SocketEngine.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

#define TIME_BREAK_READY			45000L								//�ж�ʱ��
#define TIME_BREAK_CONNECT			45000L								//�ж�ʱ��
#define TIME_DETECT_SOCKET			20000L								//���ʱ��
const char* POLICYFILEREQUESET	= TEXT("<policy-file-request/>");
const char* POLICYFILERESPONSE  = TEXT("<?xml version=\"1.0\"?><cross-domain-policy> <allow-access-from domain=\"*\" to-ports=\"*\" />  </cross-domain-policy> ");
static bool bInitHookSpecialCMD = false;
static BYTE cbHookSpecialCMD_PolicyfileRequset[32];
static BYTE cbHookSpecialCMD_PolicyfileResponse[256];
static WORD wHookSpecialCMD_PolicyfileRequset_Size;
static WORD wHookSpecialCMD_PolicyfileResponse_Size;

//////////////////////////////////////////////////////////////////////////

//��������
#define QUEUE_SEND_REQUEST			1									//���ͱ�ʶ
#define QUEUE_SAFE_CLOSE			2									//��ȫ�ر�
#define QUEUE_ALLOW_BATCH			3									//����Ⱥ��
#define QUEUE_DETECT_SOCKET			4									//�������

//��������ṹ
struct tagSendDataRequest
{
	WORD							wMainCmdID;							//��������
	WORD							wSubCmdID;							//��������
	WORD							wIndex;								//��������
	WORD							wRountID;							//ѭ������
	WORD							wDataSize;							//���ݴ�С
	BYTE							cbSendBuf[SOCKET_PACKAGE];			//���ͻ���
};

//����Ⱥ��
struct tagAllowBatchSend
{
	WORD							wIndex;								//��������
	WORD							wRountID;							//ѭ������
	BYTE							cbAllow;							//�����־
};

//��ȫ�ر�
struct tagSafeCloseSocket
{
	WORD							wIndex;								//��������
	WORD							wRountID;							//ѭ������
};

//////////////////////////////////////////////////////////////////////////

//�����ֽ�ӳ���
const BYTE g_SendByteMap[256]=				
{
	0x70,0x2F,0x40,0x5F,0x44,0x8E,0x6E,0x45,0x7E,0xAB,0x2C,0x1F,0xB4,0xAC,0x9D,0x91,
	0x0D,0x36,0x9B,0x0B,0xD4,0xC4,0x39,0x74,0xBF,0x23,0x16,0x14,0x06,0xEB,0x04,0x3E,
	0x12,0x5C,0x8B,0xBC,0x61,0x63,0xF6,0xA5,0xE1,0x65,0xD8,0xF5,0x5A,0x07,0xF0,0x13,
	0xF2,0x20,0x6B,0x4A,0x24,0x59,0x89,0x64,0xD7,0x42,0x6A,0x5E,0x3D,0x0A,0x77,0xE0,
	0x80,0x27,0xB8,0xC5,0x8C,0x0E,0xFA,0x8A,0xD5,0x29,0x56,0x57,0x6C,0x53,0x67,0x41,
	0xE8,0x00,0x1A,0xCE,0x86,0x83,0xB0,0x22,0x28,0x4D,0x3F,0x26,0x46,0x4F,0x6F,0x2B,
	0x72,0x3A,0xF1,0x8D,0x97,0x95,0x49,0x84,0xE5,0xE3,0x79,0x8F,0x51,0x10,0xA8,0x82,
	0xC6,0xDD,0xFF,0xFC,0xE4,0xCF,0xB3,0x09,0x5D,0xEA,0x9C,0x34,0xF9,0x17,0x9F,0xDA,
	0x87,0xF8,0x15,0x05,0x3C,0xD3,0xA4,0x85,0x2E,0xFB,0xEE,0x47,0x3B,0xEF,0x37,0x7F,
	0x93,0xAF,0x69,0x0C,0x71,0x31,0xDE,0x21,0x75,0xA0,0xAA,0xBA,0x7C,0x38,0x02,0xB7,
	0x81,0x01,0xFD,0xE7,0x1D,0xCC,0xCD,0xBD,0x1B,0x7A,0x2A,0xAD,0x66,0xBE,0x55,0x33,
	0x03,0xDB,0x88,0xB2,0x1E,0x4E,0xB9,0xE6,0xC2,0xF7,0xCB,0x7D,0xC9,0x62,0xC3,0xA6,
	0xDC,0xA7,0x50,0xB5,0x4B,0x94,0xC0,0x92,0x4C,0x11,0x5B,0x78,0xD9,0xB1,0xED,0x19,
	0xE9,0xA1,0x1C,0xB6,0x32,0x99,0xA3,0x76,0x9E,0x7B,0x6D,0x9A,0x30,0xD6,0xA9,0x25,
	0xC7,0xAE,0x96,0x35,0xD0,0xBB,0xD2,0xC8,0xA2,0x08,0xF3,0xD1,0x73,0xF4,0x48,0x2D,
	0x90,0xCA,0xE2,0x58,0xC1,0x18,0x52,0xFE,0xDF,0x68,0x98,0x54,0xEC,0x60,0x43,0x0F
};

//�����ֽ�ӳ���
const BYTE g_RecvByteMap[256]=				
{
	0x51,0xA1,0x9E,0xB0,0x1E,0x83,0x1C,0x2D,0xE9,0x77,0x3D,0x13,0x93,0x10,0x45,0xFF,
	0x6D,0xC9,0x20,0x2F,0x1B,0x82,0x1A,0x7D,0xF5,0xCF,0x52,0xA8,0xD2,0xA4,0xB4,0x0B,
	0x31,0x97,0x57,0x19,0x34,0xDF,0x5B,0x41,0x58,0x49,0xAA,0x5F,0x0A,0xEF,0x88,0x01,
	0xDC,0x95,0xD4,0xAF,0x7B,0xE3,0x11,0x8E,0x9D,0x16,0x61,0x8C,0x84,0x3C,0x1F,0x5A,
	0x02,0x4F,0x39,0xFE,0x04,0x07,0x5C,0x8B,0xEE,0x66,0x33,0xC4,0xC8,0x59,0xB5,0x5D,
	0xC2,0x6C,0xF6,0x4D,0xFB,0xAE,0x4A,0x4B,0xF3,0x35,0x2C,0xCA,0x21,0x78,0x3B,0x03,
	0xFD,0x24,0xBD,0x25,0x37,0x29,0xAC,0x4E,0xF9,0x92,0x3A,0x32,0x4C,0xDA,0x06,0x5E,
	0x00,0x94,0x60,0xEC,0x17,0x98,0xD7,0x3E,0xCB,0x6A,0xA9,0xD9,0x9C,0xBB,0x08,0x8F,
	0x40,0xA0,0x6F,0x55,0x67,0x87,0x54,0x80,0xB2,0x36,0x47,0x22,0x44,0x63,0x05,0x6B,
	0xF0,0x0F,0xC7,0x90,0xC5,0x65,0xE2,0x64,0xFA,0xD5,0xDB,0x12,0x7A,0x0E,0xD8,0x7E,
	0x99,0xD1,0xE8,0xD6,0x86,0x27,0xBF,0xC1,0x6E,0xDE,0x9A,0x09,0x0D,0xAB,0xE1,0x91,
	0x56,0xCD,0xB3,0x76,0x0C,0xC3,0xD3,0x9F,0x42,0xB6,0x9B,0xE5,0x23,0xA7,0xAD,0x18,
	0xC6,0xF4,0xB8,0xBE,0x15,0x43,0x70,0xE0,0xE7,0xBC,0xF1,0xBA,0xA5,0xA6,0x53,0x75,
	0xE4,0xEB,0xE6,0x85,0x14,0x48,0xDD,0x38,0x2A,0xCC,0x7F,0xB1,0xC0,0x71,0x96,0xF8,
	0x3F,0x28,0xF2,0x69,0x74,0x68,0xB7,0xA3,0x50,0xD0,0x79,0x1D,0xFC,0xCE,0x8A,0x8D,
	0x2E,0x62,0x30,0xEA,0xED,0x2B,0x26,0xB9,0x81,0x7C,0x46,0x89,0x73,0xA2,0xF7,0x72
};

//���ݼ�����Կ
const static DWORD g_dwPacketKey=0xA55AA55A;

//////////////////////////////////////////////////////////////////////////

//���캯��
COverLapped::COverLapped(enOperationType OperationType) : m_OperationType(OperationType)
{
	memset(&m_WSABuffer,0,sizeof(m_WSABuffer));
	memset(&m_OverLapped,0,sizeof(m_OverLapped));
}

//��������
COverLapped::~COverLapped()
{
}

//////////////////////////////////////////////////////////////////////////

//���캯��
COverLappedSend::COverLappedSend() : COverLapped(OperationType_Send)
{
	m_WSABuffer.len=0;
	m_WSABuffer.buf=(char *)m_cbBuffer;
}

//��������
COverLappedSend::~COverLappedSend()
{
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CServerSocketItem::CServerSocketItem(WORD wIndex, IServerSocketItemSink * pIServerSocketItemSink) 
	: m_wIndex(wIndex), m_pIServerSocketItemSink(pIServerSocketItemSink)
{
	m_wRountID=0;
	m_wRecvSize=0;
	m_cbSendRound=0;
	m_cbRecvRound=0;
	m_bNotify=true;
	m_bRecvIng=false;
	m_bCloseIng=false;
	m_bAllowBatch=false;
	m_dwSendXorKey=0;
	m_dwRecvXorKey=0;
	m_dwClientAddr=0;
	m_dwConnectTime=0;
	m_dwSendTickCount=0;
	m_dwRecvTickCount=0;
	m_dwSendPacketCount=0;
	m_dwRecvPacketCount=0;
	m_hSocket=INVALID_SOCKET;
	if(bInitHookSpecialCMD == false)
	{
		memset(cbHookSpecialCMD_PolicyfileRequset, 0, sizeof(cbHookSpecialCMD_PolicyfileRequset));
		memcpy(cbHookSpecialCMD_PolicyfileRequset, POLICYFILEREQUESET, _tcslen(POLICYFILEREQUESET));
		wHookSpecialCMD_PolicyfileRequset_Size = _tcslen(POLICYFILEREQUESET) + 1;
		memset(cbHookSpecialCMD_PolicyfileResponse, 0, sizeof(cbHookSpecialCMD_PolicyfileResponse));
		memcpy(cbHookSpecialCMD_PolicyfileResponse, POLICYFILERESPONSE, _tcslen(POLICYFILERESPONSE));
		wHookSpecialCMD_PolicyfileResponse_Size = _tcslen(POLICYFILERESPONSE) + 1;
		bInitHookSpecialCMD = true;
	}
}

//��������
CServerSocketItem::~CServerSocketItem(void)
{
	//ɾ�������ص� IO
	INT_PTR iCount=m_OverLappedSendFree.GetCount();
	for (INT_PTR i=0;i<iCount;i++) delete m_OverLappedSendFree[i];
	m_OverLappedSendFree.RemoveAll();

	//ɾ����ص� IO
	iCount=m_OverLappedSendActive.GetCount();
	for (INT_PTR i=0;i<iCount;i++) delete m_OverLappedSendActive[i];
	m_OverLappedSendActive.RemoveAll();

	return;
}

//���ӳ��
WORD CServerSocketItem::SeedRandMap(WORD wSeed)
{
	DWORD dwHold=wSeed;
	return (WORD)((dwHold=dwHold*241103L+2533101L)>>16);
}

//ӳ�䷢������
BYTE CServerSocketItem::MapSendByte(BYTE const cbData)
{
	//BYTE cbMap=g_SendByteMap[(BYTE)(cbData+m_cbSendRound)];
	//m_cbSendRound+=3;
	BYTE cbMap=g_SendByteMap[(BYTE)(cbData)];
	return cbMap;
}

//ӳ���������
BYTE CServerSocketItem::MapRecvByte(BYTE const cbData)
{
	//BYTE cbMap=g_RecvByteMap[cbData]-m_cbRecvRound;
	//m_cbRecvRound+=3;
	BYTE cbMap=g_RecvByteMap[cbData];
	return cbMap;
}

//��ȡ���ͽṹ
COverLappedSend * CServerSocketItem::GetSendOverLapped()
{
	//Ѱ�ҿ��нṹ
	COverLappedSend * pOverLappedSend=NULL;
	INT_PTR nFreeCount=m_OverLappedSendFree.GetCount();
	if (nFreeCount>0)
	{
		pOverLappedSend=m_OverLappedSendFree[nFreeCount-1];
		GT_ASSERT(pOverLappedSend!=NULL);
		m_OverLappedSendFree.RemoveAt(nFreeCount-1);
		m_OverLappedSendActive.Add(pOverLappedSend);
		return pOverLappedSend;
	}

	//�½����ͽṹ
	_BEGIN_TRY
	{
		pOverLappedSend=new COverLappedSend;
		GT_ASSERT(pOverLappedSend!=NULL);
		m_OverLappedSendActive.Add(pOverLappedSend);
		return pOverLappedSend;
	}
	CATCH_COMMON_EXCEPTION(;)CATCH_UNKNOWN_EXCEPTION(;)_END_CATCH
	return NULL;
}
bool CServerSocketItem::HookSpecialCMD(BYTE* pcbDataBuffer, WORD wDataSize)
{
	if(wDataSize == wHookSpecialCMD_PolicyfileRequset_Size)
	{
		if(_tcsicmp((char*)pcbDataBuffer, POLICYFILEREQUESET) == 0)
		{		
			return SendRawData((void*)cbHookSpecialCMD_PolicyfileResponse, 
				wHookSpecialCMD_PolicyfileResponse_Size,
				GetRountID());
		}
	}
	return false;
}
//�󶨶���
DWORD CServerSocketItem::Attach(SOCKET hSocket, DWORD dwClientAddr)
{
	//Ч������
	GT_ASSERT(dwClientAddr!=0);
	GT_ASSERT(m_bRecvIng==false);
	GT_ASSERT(IsValidSocket()==false);
	GT_ASSERT(hSocket!=INVALID_SOCKET);

	//���ñ���
	m_bNotify=false;
	m_bRecvIng=false;
	m_bCloseIng=false;
	m_hSocket=hSocket;
	m_dwClientAddr=dwClientAddr;
	m_dwRecvTickCount=GetTickCount();
	m_dwConnectTime=(DWORD)time(NULL);

	//����֪ͨ
	m_pIServerSocketItemSink->OnSocketAcceptEvent(this);

	return GetIdentifierID();
}

//���ͺ���
bool CServerSocketItem::SendData(WORD wMainCmdID, WORD wSubCmdID, WORD wRountID)
{
	//Ч��״̬
	if (m_bCloseIng==true) return false;
	if (m_wRountID!=wRountID) return false;
	if (m_dwRecvPacketCount==0) return false;
	if (IsValidSocket()==false) return false;

	//Ѱ�ҷ��ͽṹ
	COverLappedSend * pOverLappedSend=GetSendOverLapped();
	GT_ASSERT(pOverLappedSend!=NULL);
	if (pOverLappedSend==NULL) return false;

	//��������
	CMD_Head * pHead=(CMD_Head *)pOverLappedSend->m_cbBuffer;
	pHead->CommandInfo.wMainCmdID=wMainCmdID;
	pHead->CommandInfo.wSubCmdID=wSubCmdID;
	WORD wSendSize=EncryptBuffer(pOverLappedSend->m_cbBuffer,sizeof(CMD_Head),sizeof(pOverLappedSend->m_cbBuffer));
	pOverLappedSend->m_WSABuffer.len=wSendSize;

	//��������
	if (m_OverLappedSendActive.GetCount()==1)
	{
		DWORD dwThancferred=0;
		int iRetCode=WSASend(m_hSocket,&pOverLappedSend->m_WSABuffer,1,&dwThancferred,0,&pOverLappedSend->m_OverLapped,NULL);
		if ((iRetCode==SOCKET_ERROR)&&(WSAGetLastError()!=WSA_IO_PENDING))
		{
			OnSendCompleted(pOverLappedSend,0L);
			return false;
		}
	}

	return true;
}

//���ͺ���
bool CServerSocketItem::SendData(void * pData, WORD wDataSize, WORD wMainCmdID, WORD wSubCmdID, WORD wRountID)
{
	//Ч�����
	GT_ASSERT(wDataSize<=SOCKET_PACKAGE);

	//Ч��״̬
	if (m_bCloseIng==true) return false;
	if (m_wRountID!=wRountID) return false;
	if (m_dwRecvPacketCount==0) return false;
	if (IsValidSocket()==false) return false;
	if (wDataSize>SOCKET_PACKAGE) return false;

	//Ѱ�ҷ��ͽṹ
	COverLappedSend * pOverLappedSend=GetSendOverLapped();
	GT_ASSERT(pOverLappedSend!=NULL);
	if (pOverLappedSend==NULL) return false;

	//��������
	CMD_Head * pHead=(CMD_Head *)pOverLappedSend->m_cbBuffer;
	pHead->CommandInfo.wMainCmdID=wMainCmdID;
	pHead->CommandInfo.wSubCmdID=wSubCmdID;
	if (wDataSize>0)
	{
		GT_ASSERT(pData!=NULL);
		memcpy(pHead+1,pData,wDataSize);
	}
	WORD wSendSize=EncryptBuffer(pOverLappedSend->m_cbBuffer,sizeof(CMD_Head)+wDataSize,sizeof(pOverLappedSend->m_cbBuffer));
	pOverLappedSend->m_WSABuffer.len=wSendSize;

	//��������
	if (m_OverLappedSendActive.GetCount()==1)
	{
		DWORD dwThancferred=0;
		int iRetCode=WSASend(m_hSocket,&pOverLappedSend->m_WSABuffer,1,&dwThancferred,0,&pOverLappedSend->m_OverLapped,NULL);
		if ((iRetCode==SOCKET_ERROR)&&(WSAGetLastError()!=WSA_IO_PENDING))
		{
			OnSendCompleted(pOverLappedSend,0L);
			return false;
		}
	}

	return true;
}
//���ͺ���
bool CServerSocketItem::SendRawData(void * pData, WORD wDataSize, WORD wRountID)
{
	//Ч�����
	GT_ASSERT(wDataSize<=SOCKET_PACKAGE);

	//Ч��״̬
	if (m_bCloseIng==true) return false;
	if (m_wRountID!=wRountID) return false;
	if (IsValidSocket()==false) return false;
	if (wDataSize>SOCKET_PACKAGE) return false;

	//Ѱ�ҷ��ͽṹ
	COverLappedSend * pOverLappedSend=GetSendOverLapped();
	GT_ASSERT(pOverLappedSend!=NULL);
	if (pOverLappedSend==NULL) return false;

	//��������
	memcpy(pOverLappedSend->m_cbBuffer, pData, wDataSize);
	pOverLappedSend->m_WSABuffer.len=wDataSize;

	//��������
	if (m_OverLappedSendActive.GetCount()==1)
	{
		DWORD dwThancferred=0;
		int iRetCode=WSASend(m_hSocket,&pOverLappedSend->m_WSABuffer,1,&dwThancferred,0,&pOverLappedSend->m_OverLapped,NULL);
		if ((iRetCode==SOCKET_ERROR)&&(WSAGetLastError()!=WSA_IO_PENDING))
		{
			OnSendCompleted(pOverLappedSend,0L);
			return false;
		}
	}

	return true;
}

//Ͷ�ݽ���
bool CServerSocketItem::RecvData()
{
	//Ч�����
	GT_ASSERT(m_bRecvIng==false);
	GT_ASSERT(m_hSocket!=INVALID_SOCKET);

	//�жϹر�
	if (m_bCloseIng==true)
	{
		if (m_OverLappedSendActive.GetCount()==0) CloseSocket(m_wRountID);
		return false;
	}

	//��������
	m_bRecvIng=true;
	DWORD dwThancferred=0,dwFlags=0;
	int iRetCode=WSARecv(m_hSocket,&m_OverLappedRecv.m_WSABuffer,1,&dwThancferred,&dwFlags,&m_OverLappedRecv.m_OverLapped,NULL);
	if ((iRetCode==SOCKET_ERROR)&&(WSAGetLastError()!=WSA_IO_PENDING))
	{
		m_bRecvIng=false;
		CloseSocket(m_wRountID);
		return false;
	}

	return true;
}

//�ر�����
bool CServerSocketItem::CloseSocket(WORD wRountID)
{
	//״̬�ж�
	if (m_wRountID!=wRountID) return false;

	//�ر�����
	if (m_hSocket!=INVALID_SOCKET)
	{
		closesocket(m_hSocket);
		m_hSocket=INVALID_SOCKET;
	}

	//�жϹر�
	if ((m_bRecvIng==false)&&(m_OverLappedSendActive.GetCount()==0)) OnCloseCompleted();

	return true;
}

//���ùر�
bool CServerSocketItem::ShutDownSocket(WORD wRountID)
{
	return true;

	//״̬�ж�
	if (m_wRountID!=wRountID) return false;
	if (m_hSocket==INVALID_SOCKET) return false;

	//���ñ���
	if (m_bCloseIng==false)
	{
		m_bCloseIng=true;
		if (m_OverLappedSendActive.GetCount()==0) CloseSocket(wRountID);
	}

	return true;
}

//����Ⱥ��
bool CServerSocketItem::AllowBatchSend(WORD wRountID, bool bAllowBatch)
{
	//״̬�ж�
	if (m_wRountID!=wRountID) return false;
	if (m_hSocket==INVALID_SOCKET) return false;

	//���ñ���
	m_bAllowBatch=bAllowBatch;

	return true;
}

//���ñ���
void CServerSocketItem::ResetSocketData()
{
	//Ч��״̬
	GT_ASSERT(m_hSocket==INVALID_SOCKET);

	//��������
	m_wRountID++;
	m_wRecvSize=0;
	m_cbSendRound=0;
	m_cbRecvRound=0;
	m_dwSendXorKey=0;
	m_dwRecvXorKey=0;
	m_dwClientAddr=0;
	m_dwConnectTime=0;
	m_dwSendTickCount=0;
	m_dwRecvTickCount=0;
	m_dwSendPacketCount=0;
	m_dwRecvPacketCount=0;

	//״̬����
	m_bNotify=true;
	m_bRecvIng=false;
	m_bCloseIng=false;
	m_bAllowBatch=false;
	m_OverLappedSendFree.Append(m_OverLappedSendActive);
	m_OverLappedSendActive.RemoveAll();

	return;
}

//������ɺ���
bool CServerSocketItem::OnSendCompleted(COverLappedSend * pOverLappedSend, DWORD dwThancferred)
{
	//Ч�����
	GT_ASSERT(pOverLappedSend!=NULL);
	GT_ASSERT(m_OverLappedSendActive.GetCount()>0);
	GT_ASSERT(pOverLappedSend==m_OverLappedSendActive[0]);

	//�ͷŷ��ͽṹ
	m_OverLappedSendActive.RemoveAt(0);
	m_OverLappedSendFree.Add(pOverLappedSend);

	//���ñ���
	if (dwThancferred!=0) m_dwSendTickCount=GetTickCount();

	//�жϹر�
	if (m_hSocket==INVALID_SOCKET)
	{
		m_OverLappedSendFree.Append(m_OverLappedSendActive);
		m_OverLappedSendActive.RemoveAll();
		CloseSocket(m_wRountID);
		return true;
	}

	//������������
	if (m_OverLappedSendActive.GetCount()>0)
	{
		DWORD dwThancferred=0;
		pOverLappedSend=m_OverLappedSendActive[0];
		GT_ASSERT(pOverLappedSend!=NULL);
		int iRetCode=WSASend(m_hSocket,&pOverLappedSend->m_WSABuffer,1,&dwThancferred,0,&pOverLappedSend->m_OverLapped,NULL);
		if ((iRetCode==SOCKET_ERROR)&&(WSAGetLastError()!=WSA_IO_PENDING))
		{
			m_OverLappedSendFree.Append(m_OverLappedSendActive);
			m_OverLappedSendActive.RemoveAll();
			CloseSocket(m_wRountID);
			return false;
		}
		return true;
	}

	//�жϹر�
	if (m_bCloseIng==true) 
		CloseSocket(m_wRountID);

	return true;
}

//������ɺ���
bool CServerSocketItem::OnRecvCompleted(COverLappedRecv * pOverLappedRecv, DWORD dwThancferred)
{
	//Ч������
	GT_ASSERT(m_bRecvIng==true);

	//���ñ���
	m_bRecvIng=false;
	m_dwRecvTickCount=GetTickCount();

	//�жϹر�
	if (m_hSocket==INVALID_SOCKET)
	{
		CloseSocket(m_wRountID);
		return true;
	}

	//��������
	int iRetCode=recv(m_hSocket,(char *)m_cbRecvBuf+m_wRecvSize,sizeof(m_cbRecvBuf)-m_wRecvSize,0);
	if (iRetCode<=0)
	{
		CloseSocket(m_wRountID);
		return true;
	}
	//�ͻ������������
	if(HookSpecialCMD(m_cbRecvBuf+m_wRecvSize, iRetCode))
	{
		return true;
	}

	//�������
	m_wRecvSize+=iRetCode;
	BYTE cbBuffer[SOCKET_BUFFER];
	CMD_Head * pHead=(CMD_Head *)m_cbRecvBuf;

	//��������
	_BEGIN_TRY
	{
		while (m_wRecvSize>=sizeof(CMD_Head))
		{
			//Ч������
			WORD wPacketSize=pHead->CmdInfo.wDataSize;
			if (wPacketSize>SOCKET_BUFFER) throw TEXT("���ݰ�����");
			if (wPacketSize<sizeof(CMD_Head)) throw TEXT("���ݰ��Ƿ�");
			if (pHead->CmdInfo.cbMessageVer!=SOCKET_VER) throw TEXT("���ݰ��汾����");
			if (m_wRecvSize<wPacketSize) break;

			//��ȡ����
			CopyMemory(cbBuffer,m_cbRecvBuf,wPacketSize);
			WORD wRealySize=CrevasseBuffer(cbBuffer,wPacketSize);
			GT_ASSERT(wRealySize>=sizeof(CMD_Head));
			m_dwRecvPacketCount++;

			//��������
			WORD wDataSize=wRealySize-sizeof(CMD_Head);
			void * pDataBuffer=cbBuffer+sizeof(CMD_Head);
			CMD_Command Command=((CMD_Head *)cbBuffer)->CommandInfo;

			//�ں�����
			if (Command.wMainCmdID==MDM_KN_COMMAND)
			{
				switch (Command.wSubCmdID)
				{
				case SUB_KN_DETECT_SOCKET:	//������
					{
						break;
					}
				default: throw TEXT("�Ƿ�������");
				}
			}
			else 
			{
				//��Ϣ����
				m_pIServerSocketItemSink->OnSocketReadEvent(Command,pDataBuffer,wDataSize,this);			
			}

			//ɾ����������
			m_wRecvSize-=wPacketSize;
			MoveMemory(m_cbRecvBuf,m_cbRecvBuf+wPacketSize,m_wRecvSize);

		}
	}
	CATCH_COMMON_EXCEPTION(CloseSocket(m_wRountID);return false;)
		CATCH_UNKNOWN_EXCEPTION(CloseSocket(m_wRountID);return false;)
		_END_CATCH

	return RecvData();
}

//�ر����֪ͨ
bool CServerSocketItem::OnCloseCompleted()
{
	//Ч��״̬
	GT_ASSERT(m_hSocket==INVALID_SOCKET);
	GT_ASSERT(m_OverLappedSendActive.GetCount()==0);

	//�ر��¼�
	GT_ASSERT(m_bNotify==false);
	if (m_bNotify==false)
	{
		m_bNotify=true;
		m_pIServerSocketItemSink->OnSocketCloseEvent(this);
	}

	//״̬����
	ResetSocketData();

	return true;
}

//��������
WORD CServerSocketItem::EncryptBuffer(BYTE pcbDataBuffer[], WORD wDataSize, WORD wBufferSize)
{
	WORD i = 0;
	//Ч�����
	GT_ASSERT(wDataSize>=sizeof(CMD_Head));
	GT_ASSERT(wDataSize<=(sizeof(CMD_Head)+SOCKET_PACKAGE));
	GT_ASSERT(wBufferSize>=(wDataSize+2*sizeof(DWORD)));

	//��������
	WORD wEncryptSize=wDataSize-sizeof(CMD_Info),wSnapCount=0;
	if ((wEncryptSize%sizeof(DWORD))!=0)
	{
		wSnapCount=sizeof(DWORD)-wEncryptSize%sizeof(DWORD);
		memset(pcbDataBuffer+sizeof(CMD_Info)+wEncryptSize,0,wSnapCount);
	}

	//Ч�������ֽ�ӳ��
	BYTE cbCheckCode=0;
	for (i=sizeof(CMD_Info);i<wDataSize;i++) 
	{
		cbCheckCode+=pcbDataBuffer[i];
		pcbDataBuffer[i]=MapSendByte(pcbDataBuffer[i]);
	}

	//��д��Ϣͷ
	CMD_Head * pHead=(CMD_Head *)pcbDataBuffer;
	pHead->CmdInfo.cbCheckCode=~cbCheckCode+1;
	pHead->CmdInfo.wDataSize=wDataSize;
	pHead->CmdInfo.cbMessageVer=SOCKET_VER;


	//���ñ���
	m_dwSendPacketCount++;
	//m_dwSendXorKey=dwXorKey;

	return wDataSize;
}

//��������
WORD CServerSocketItem::CrevasseBuffer(BYTE pcbDataBuffer[], WORD wDataSize)
{
	WORD i = 0;
	//Ч�����
	GT_ASSERT(wDataSize>=sizeof(CMD_Head));
	GT_ASSERT(((CMD_Head *)pcbDataBuffer)->CmdInfo.wDataSize==wDataSize);

	//��������
	WORD wSnapCount=0;
	if ((wDataSize%sizeof(DWORD))!=0)
	{
		wSnapCount=sizeof(DWORD)-wDataSize%sizeof(DWORD);
		memset(pcbDataBuffer+wDataSize,0,wSnapCount);
	}

	
	//Ч�������ֽ�ӳ��
	CMD_Head * pHead=(CMD_Head *)pcbDataBuffer;
	BYTE cbCheckCode=pHead->CmdInfo.cbCheckCode;;
	for (i=sizeof(CMD_Info);i<wDataSize;i++)
	{
		pcbDataBuffer[i]=MapRecvByte(pcbDataBuffer[i]);
		cbCheckCode+=pcbDataBuffer[i];
	}
	if (cbCheckCode!=0) throw TEXT("���ݰ�Ч�������");

	return wDataSize;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CServerSocketRSThread::CServerSocketRSThread(void)
{
	m_hCompletionPort=NULL;
}

//��������
CServerSocketRSThread::~CServerSocketRSThread(void)
{
}

//���ú���
bool CServerSocketRSThread::InitThread(HANDLE hCompletionPort)
{
	GT_ASSERT(hCompletionPort!=NULL);
	m_hCompletionPort=hCompletionPort;
	return true;
}

//Run����
bool CServerSocketRSThread::RepetitionRun()
{
	//Ч�����
	GT_ASSERT(m_hCompletionPort!=NULL);

	//��������
	DWORD dwThancferred=0;					
	OVERLAPPED * pOverLapped=NULL;
	COverLapped * pSocketLapped=NULL;
	CServerSocketItem * pServerSocketItem=NULL;

	//�ȴ���ɶ˿�
	BOOL bSuccess=GetQueuedCompletionStatus(m_hCompletionPort,&dwThancferred,(PULONG_PTR)&pServerSocketItem,&pOverLapped,INFINITE);
	if ((bSuccess==FALSE)&&(GetLastError()!=ERROR_NETNAME_DELETED)) return false;
	if ((pServerSocketItem==NULL)&&(pOverLapped==NULL)) return false;

	//�������
	GT_ASSERT(pOverLapped!=NULL);
	GT_ASSERT(pServerSocketItem!=NULL);
	pSocketLapped=CONTAINING_RECORD(pOverLapped,COverLapped,m_OverLapped);
	switch (pSocketLapped->GetOperationType())
	{
	case OperationType_Recv:	//SOCKET ���ݶ�ȡ
		{
			COverLappedRecv * pOverLappedRecv=(COverLappedRecv *)pSocketLapped;
			CThreadLockHandle SocketLockHandle(pServerSocketItem->GetSignedLock());
			pServerSocketItem->OnRecvCompleted(pOverLappedRecv,dwThancferred);
			break;
		}
	case OperationType_Send:	//SOCKET ���ݷ���
		{
			COverLappedSend * pOverLappedSend=(COverLappedSend *)pSocketLapped;
			CThreadLockHandle SocketLockHandle(pServerSocketItem->GetSignedLock());
			pServerSocketItem->OnSendCompleted(pOverLappedSend,dwThancferred);
			break;
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CSocketAcceptThread::CSocketAcceptThread(void)
{
	m_hCompletionPort=NULL;
	m_pTCPSocketManager=NULL;
	m_hListenSocket=INVALID_SOCKET;
}

//��������
CSocketAcceptThread::~CSocketAcceptThread(void)
{
}

//���ú���
bool CSocketAcceptThread::InitThread(HANDLE hCompletionPort, SOCKET hListenSocket, CTCPSocketEngine * pTCPSocketManager)
{
	GT_ASSERT(hCompletionPort!=NULL);
	GT_ASSERT(pTCPSocketManager!=NULL);
	GT_ASSERT(hListenSocket!=INVALID_SOCKET);
	m_hListenSocket=hListenSocket;
	m_hCompletionPort=hCompletionPort;
	m_pTCPSocketManager=pTCPSocketManager;
	return true;
}

//Run����
bool CSocketAcceptThread::RepetitionRun()
{
	//Ч�����
	GT_ASSERT(m_hCompletionPort!=NULL);
	GT_ASSERT(m_pTCPSocketManager!=NULL);

	//���ñ���
	SOCKADDR_IN	SocketAddr;
	CServerSocketItem * pServerSocketItem=NULL;
	SOCKET hConnectSocket=INVALID_SOCKET;
	int nBufferSize=sizeof(SocketAddr);

	_BEGIN_TRY
	{
		//��������
		hConnectSocket=WSAAccept(m_hListenSocket,(SOCKADDR *)&SocketAddr,&nBufferSize,NULL,NULL);
		if (hConnectSocket==INVALID_SOCKET) return false;

		//��ȡ����
		pServerSocketItem=m_pTCPSocketManager->ActiveSocketItem();
		if (pServerSocketItem==NULL) throw TEXT("�������Ӷ���ʧ��");

		//�������
		CThreadLockHandle SocketLockHandle(pServerSocketItem->GetSignedLock());
		pServerSocketItem->Attach(hConnectSocket,SocketAddr.sin_addr.S_un.S_addr);
		CreateIoCompletionPort((HANDLE)hConnectSocket,m_hCompletionPort,(ULONG_PTR)pServerSocketItem,0);
		pServerSocketItem->RecvData();
	}
	CATCH_COMMON_EXCEPTION(if (hConnectSocket!=INVALID_SOCKET)	closesocket(hConnectSocket);)
		CATCH_UNKNOWN_EXCEPTION(if (hConnectSocket!=INVALID_SOCKET)	closesocket(hConnectSocket);)
		_END_CATCH

	return true;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CSocketDetectThread::CSocketDetectThread(void)
{
	m_dwTickCount=0;;
	m_pTCPSocketManager=NULL;
}

//��������
CSocketDetectThread::~CSocketDetectThread(void)
{
}

//���ú���
bool CSocketDetectThread::InitThread(CTCPSocketEngine * pTCPSocketManager)
{
	//Ч�����
	GT_ASSERT(pTCPSocketManager!=NULL);

	//���ñ���
	m_dwTickCount=0L;
	m_pTCPSocketManager=pTCPSocketManager;

	return true;
}

//Run����
bool CSocketDetectThread::RepetitionRun()
{
	//Ч�����
	GT_ASSERT(m_pTCPSocketManager!=NULL);

	//���ü��
	Sleep(500);
	m_dwTickCount+=500L;

	//�������
	if (m_dwTickCount>20000L)
	{
		m_dwTickCount=0L;
		m_pTCPSocketManager->DetectSocket();
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CTCPSocketEngine::CTCPSocketEngine(void)
{
	m_bService=false;
	m_wListenPort=0;
	m_dwLastDetect=0;
	m_wMaxSocketItem=0;
	m_hCompletionPort=NULL;
	m_hServerSocket=INVALID_SOCKET;

	return;
}

//��������
CTCPSocketEngine::~CTCPSocketEngine(void)
{
	//ֹͣ����
	StopService();

	//�ͷŴ洢����
	CServerSocketItem * pSocketItem=NULL;
	for (INT_PTR i=0;i<m_StorageSocketItem.GetCount();i++)
	{
		pSocketItem=m_StorageSocketItem[i];
		GT_ASSERT(pSocketItem!=NULL);
		SafeDelete(pSocketItem);
	}
	m_StorageSocketItem.RemoveAll();

	return;
}

//�ӿڲ�ѯ
void * __cdecl CTCPSocketEngine::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITCPSocketEngine,Guid,dwQueryVer);
	QUERYINTERFACE(ITCPSocketEngineManager,Guid,dwQueryVer);
	QUERYINTERFACE(IQueueServiceSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITCPSocketEngine,Guid,dwQueryVer);
	return NULL;
}

//���ýӿ�
bool __cdecl CTCPSocketEngine::SetSocketEngineSink(IUnknownEx * pIUnknownEx)
{
	//״̬�ж�
	if (m_bService==true) 
	{
		CEventTrace::ShowEventNotify(TEXT("�������洦�ڷ���״̬���󶨲�������"),Level_Exception);
		return false;
	}

	//���ýӿ�
	if (m_AttemperEvent.SetQueueService(pIUnknownEx)==false)
	{
		CEventTrace::ShowEventNotify(TEXT("���������봥�������ʧ��"),Level_Exception);
		return false;
	}
	
	return true;
}

//���ö˿�
bool __cdecl CTCPSocketEngine::SetServicePort(WORD wListenPort)
{
	//Ч��״̬
	if (m_bService==true) 
	{
		CEventTrace::ShowEventNotify(TEXT("�������洦�ڷ���״̬���˿ڰ󶨲�������"),Level_Exception);
		return false;
	}

	//�жϲ���
	if (wListenPort==0)
	{
		CEventTrace::ShowEventNotify(TEXT("����˿ڴ��󣬶˿ڰ󶨲���ʧ��"),Level_Exception);
		return false;
	}

	//���ñ���
	m_wListenPort=wListenPort;

	return true;
}

//������Ŀ
bool __cdecl CTCPSocketEngine::SetMaxSocketItem(WORD wMaxSocketItem)
{
	m_wMaxSocketItem=wMaxSocketItem;
	return true;
}

//��������
bool __cdecl CTCPSocketEngine::StartService()
{
	DWORD i = 0;
	//Ч��״̬
	if (m_bService==true) 
	{
		CEventTrace::ShowEventNotify(TEXT("���������ظ�������������������"),Level_Warning);
		return true;
	}

	//�ж϶˿�
	if (m_wListenPort==0)
	{
		CEventTrace::ShowEventNotify(TEXT("������������˿���Ч"),Level_Exception);
		return false;
	}

	//�󶨶���
	if (m_SendQueueService.SetQueueServiceSink(GET_MYSELF_INTERFACE(IUnknownEx))==false)
	{
		CEventTrace::ShowEventNotify(TEXT("���������봥�������ʧ��"),Level_Exception);
		return false;
	}

	try
	{
		//��ȡϵͳ��Ϣ
		SYSTEM_INFO SystemInfo;
		GetSystemInfo(&SystemInfo);
		DWORD dwThreadCount=SystemInfo.dwNumberOfProcessors;

		//������ɶ˿�
		m_hCompletionPort=CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,NULL,SystemInfo.dwNumberOfProcessors);
		if (m_hCompletionPort==NULL) throw TEXT("����������ɶ˿ڴ���ʧ��");

		//��������SOCKET
		struct sockaddr_in SocketAddr;
		memset(&SocketAddr,0,sizeof(SocketAddr));
		SocketAddr.sin_addr.s_addr=INADDR_ANY;
		SocketAddr.sin_family=AF_INET;
		SocketAddr.sin_port=htons(m_wListenPort);
		m_hServerSocket=WSASocket(AF_INET,SOCK_STREAM,IPPROTO_TCP,NULL,0,WSA_FLAG_OVERLAPPED);
		if (m_hServerSocket==INVALID_SOCKET) throw TEXT("����������� SOCKET ����ʧ��");
		int iErrorCode=bind(m_hServerSocket,(SOCKADDR*)&SocketAddr,sizeof(SocketAddr));
		if (iErrorCode==SOCKET_ERROR) throw TEXT("������������˿ڱ�ռ�ã��˿ڰ�ʧ��");
		iErrorCode=listen(m_hServerSocket,200);
		if (iErrorCode==SOCKET_ERROR) throw TEXT("������������˿ڱ�ռ�ã��˿ڼ���ʧ��");

		//�������Ͷ���
		bool bSuccess=m_SendQueueService.StartService(1);
		if (bSuccess==false) throw TEXT("�������淢�Ͷ��з�������ʧ��");

		//������д�߳�
		for (i=0;i<dwThreadCount;i++)
		{
			CServerSocketRSThread * pServerSocketRSThread=new CServerSocketRSThread();
			if (pServerSocketRSThread==NULL) throw TEXT("���������д�̷߳��񴴽�ʧ��");
			bSuccess=pServerSocketRSThread->InitThread(m_hCompletionPort);
			if (bSuccess==false) throw TEXT("���������д�̷߳�������ʧ��");
			m_SocketRSThreadArray.Add(pServerSocketRSThread);
		}

		//����Ӧ���߳�
		bSuccess=m_SocketAcceptThread.InitThread(m_hCompletionPort,m_hServerSocket,this);
		if (bSuccess==false) throw TEXT("����������������̷߳�������");

		//Run��д�߳�
		for (i=0;i<dwThreadCount;i++)
		{
			CServerSocketRSThread * pServerSocketRSThread=m_SocketRSThreadArray[i];
			GT_ASSERT(pServerSocketRSThread!=NULL);
			bSuccess=pServerSocketRSThread->StartThead();
			if (bSuccess==false) throw TEXT("���������д�̷߳�������ʧ��");
		}

		//�������߳�
		m_SocketDetectThread.InitThread(this);
		bSuccess=m_SocketDetectThread.StartThead();
		if (bSuccess==false) throw TEXT("����������̷߳�������ʧ��");

		//RunӦ���߳�
		bSuccess=m_SocketAcceptThread.StartThead();
		if (bSuccess==false) throw TEXT("������������̷߳�������ʧ��");

		//���ñ���
		m_bService=true;
	}
	catch (LPCTSTR pszError)
	{
		CEventTrace::ShowEventNotify(pszError,Level_Exception);
		return false;
	}

	return true;
}

//ֹͣ����
bool __cdecl CTCPSocketEngine::StopService()
{
	//���ñ���
	m_bService=false;
	m_dwLastDetect=0L;

	//ֹͣ����߳�
	m_SocketDetectThread.StopThread();

	//��ֹӦ���߳�
	if (m_hServerSocket!=INVALID_SOCKET)
	{
		closesocket(m_hServerSocket);
		m_hServerSocket=INVALID_SOCKET;
	}
	m_SocketAcceptThread.StopThread();

	//ֹͣ���Ͷ���
	m_SendQueueService.StopService();

	//�ͷŶ�д�߳�
	INT_PTR nCount=m_SocketRSThreadArray.GetCount(),i=0;
	if (m_hCompletionPort!=NULL)
	{
		for (i=0;i<nCount;i++) PostQueuedCompletionStatus(m_hCompletionPort,0,NULL,NULL);
	}
	for (i=0;i<nCount;i++)
	{
		CServerSocketRSThread * pSocketThread=m_SocketRSThreadArray[i];
		GT_ASSERT(pSocketThread!=NULL);
		pSocketThread->StopThread();
		SafeDelete(pSocketThread);
	}
	m_SocketRSThreadArray.RemoveAll();

	//�ر���ɶ˿�
	if (m_hCompletionPort!=NULL)
	{
		CloseHandle(m_hCompletionPort);
		m_hCompletionPort=NULL;
	}

	//�ر�����
	CServerSocketItem * pServerSocketItem=NULL;
	for (i=0;i<m_ActiveSocketItem.GetCount();i++)
	{
		pServerSocketItem=m_ActiveSocketItem[i];
		GT_ASSERT(pServerSocketItem!=NULL);
		pServerSocketItem->CloseSocket(pServerSocketItem->GetRountID());
		pServerSocketItem->ResetSocketData();
	}
	m_FreeSocketItem.Append(m_ActiveSocketItem);
	m_ActiveSocketItem.RemoveAll();

	return true;
}

//Ӧ����Ϣ
bool CTCPSocketEngine::OnSocketAcceptEvent(CServerSocketItem * pServerSocketItem)
{
	//Ч������
	GT_ASSERT(pServerSocketItem!=NULL);
	GT_ASSERT(m_AttemperEvent.IsValid()==true);

	//Ͷ����Ϣ
	if (m_bService==false) return false;
	m_AttemperEvent.PostSocketAcceptEvent(pServerSocketItem->GetIndex(),pServerSocketItem->GetRountID(),pServerSocketItem->GetClientAddr());

	return true;
}

//�����ȡ��Ϣ
bool CTCPSocketEngine::OnSocketReadEvent(CMD_Command Command, void * pBuffer, WORD wDataSize, CServerSocketItem * pServerSocketItem)
{
	//Ч������
	GT_ASSERT(pServerSocketItem!=NULL);
	GT_ASSERT(m_AttemperEvent.IsValid()==true);

	//Ч��״̬
	if (m_bService==false) return false;
	m_AttemperEvent.PostSocketReadEvent(pServerSocketItem->GetIndex(),pServerSocketItem->GetRountID(),Command,pBuffer,wDataSize);

	return true;
}

//����ر���Ϣ
bool CTCPSocketEngine::OnSocketCloseEvent(CServerSocketItem * pServerSocketItem)
{
	//Ч�����
	GT_ASSERT(pServerSocketItem!=NULL);
	GT_ASSERT(m_AttemperEvent.IsValid()==true);

	_BEGIN_TRY
	{
		//Ч��״̬
		if (m_bService==false) return false;

		//����ʱ��
		WORD wIndex=pServerSocketItem->GetIndex();
		WORD wRountID=pServerSocketItem->GetRountID();
		DWORD dwClientAddr=pServerSocketItem->GetClientAddr();
		DWORD dwConnectTime=pServerSocketItem->GetConnectTime();
		m_AttemperEvent.PostSocketCloseEvent(wIndex,wRountID,dwClientAddr,dwConnectTime);

		//�ͷ�����
		FreeSocketItem(pServerSocketItem);
	}
	CATCH_COMMON_EXCEPTION(;)
		CATCH_UNKNOWN_EXCEPTION(;)
		_END_CATCH
	return true;
}

//֪ͨ�ص����������Ͷ����̵߳��ã�
void __cdecl CTCPSocketEngine::OnQueueServiceSink(BYTE cbThreadIndex,WORD wIdentifier, void * pBuffer, WORD wDataSize, DWORD dwInsertTime)
{
	switch (wIdentifier)
	{
	case QUEUE_SEND_REQUEST:		//��������
		{
			//Ч������
			tagSendDataRequest * pSendDataRequest=(tagSendDataRequest *)pBuffer;
			GT_ASSERT(wDataSize>=(sizeof(tagSendDataRequest)-sizeof(pSendDataRequest->cbSendBuf)));
			GT_ASSERT(wDataSize==(pSendDataRequest->wDataSize+sizeof(tagSendDataRequest)-sizeof(pSendDataRequest->cbSendBuf)));

			//��������
			if (pSendDataRequest->wIndex==INDEX_ALL_SOCKET)
			{
				//��ȡ���
				CThreadLockHandle ItemLockedHandle(&m_ItemLocked);
				m_TempSocketItem.Copy(m_ActiveSocketItem);
				ItemLockedHandle.UnLock();

				//ѭ����������
				CServerSocketItem * pServerSocketItem=NULL;
				for (INT_PTR i=0;i<m_TempSocketItem.GetCount();i++)
				{
					pServerSocketItem=m_TempSocketItem[i];
					GT_ASSERT(pServerSocketItem!=NULL);
					CThreadLockHandle SocketLockHandle(pServerSocketItem->GetSignedLock());
					if (pServerSocketItem->IsAllowBatch())
					{
						pServerSocketItem->SendData(pSendDataRequest->cbSendBuf,pSendDataRequest->wDataSize,pSendDataRequest->wMainCmdID,
							pSendDataRequest->wSubCmdID,pServerSocketItem->GetRountID());
					}
				}
			}
			else
			{
				//�����
				CServerSocketItem * pServerSocketItem=EnumSocketItem(pSendDataRequest->wIndex);
				CThreadLockHandle SocketLockHandle(pServerSocketItem->GetSignedLock());
				pServerSocketItem->SendData(pSendDataRequest->cbSendBuf,pSendDataRequest->wDataSize,pSendDataRequest->wMainCmdID,
					pSendDataRequest->wSubCmdID,pSendDataRequest->wRountID);
			}

			break;
		}
	case QUEUE_SAFE_CLOSE:		//��ȫ�ر�
		{
			//Ч������
			GT_ASSERT(wDataSize==sizeof(tagSafeCloseSocket));
			tagSafeCloseSocket * pSafeCloseSocket=(tagSafeCloseSocket *)pBuffer;

			//��ȫ�ر�
			CServerSocketItem * pServerSocketItem=EnumSocketItem(pSafeCloseSocket->wIndex);
			CThreadLockHandle SocketLockHandle(pServerSocketItem->GetSignedLock());
			pServerSocketItem->ShutDownSocket(pSafeCloseSocket->wRountID);

			break;
		}
	case QUEUE_ALLOW_BATCH:		//����Ⱥ��
		{
			//Ч������
			GT_ASSERT(wDataSize==sizeof(tagAllowBatchSend));
			tagAllowBatchSend * pAllowBatchSend=(tagAllowBatchSend *)pBuffer;

			//����Ⱥ��
			CServerSocketItem * pServerSocketItem=EnumSocketItem(pAllowBatchSend->wIndex);
			CThreadLockHandle SocketLockHandle(pServerSocketItem->GetSignedLock());
			pServerSocketItem->AllowBatchSend(pAllowBatchSend->wRountID,pAllowBatchSend->cbAllow?true:false);

			break;
		}
	case QUEUE_DETECT_SOCKET:	//�������
		{
			//��ȡ���
			CThreadLockHandle ItemLockedHandle(&m_ItemLocked);
			m_TempSocketItem.Copy(m_ActiveSocketItem);
			ItemLockedHandle.UnLock();

			//��������
			CMD_KN_DetectSocket DetectSocket;
			ZeroMemory(&DetectSocket,sizeof(DetectSocket));

			//��������
			WORD wRountID=0;
			DWORD dwNowTickCount=GetTickCount();
			DWORD dwBreakTickCount=__max(dwNowTickCount-m_dwLastDetect,TIME_BREAK_READY);

			//���ñ���
			m_dwLastDetect=GetTickCount();

			//�������
			for (INT_PTR i=0;i<m_TempSocketItem.GetCount();i++)
			{
				//��������
				CServerSocketItem * pServerSocketItem=m_TempSocketItem[i];
				DWORD dwRecvTickCount=pServerSocketItem->GetRecvTickCount();
				CThreadLockHandle SocketLockHandle(pServerSocketItem->GetSignedLock());

				//������
				if (dwRecvTickCount>=dwNowTickCount) continue;

				//�������
				if (pServerSocketItem->IsReadySend()==true)
				{
					if ((dwNowTickCount-dwRecvTickCount)>dwBreakTickCount)
					{
						pServerSocketItem->CloseSocket(pServerSocketItem->GetRountID());
						continue;
					}
				}
				else
				{
					if ((dwNowTickCount-dwRecvTickCount)>TIME_BREAK_CONNECT)
					{
						pServerSocketItem->CloseSocket(pServerSocketItem->GetRountID());
						continue;
					}
				}

				//��������
				if (pServerSocketItem->IsReadySend()==true)
				{
					wRountID=pServerSocketItem->GetRountID();
					DetectSocket.dwSendTickCount=GetTickCount();
					pServerSocketItem->SendData(&DetectSocket,sizeof(DetectSocket),MDM_KN_COMMAND,SUB_KN_DETECT_SOCKET,wRountID);
				}
			}

			break;
		}
	default: { GT_ASSERT(FALSE); }
	}

	return;
}

//��ȡ���ж���
CServerSocketItem * CTCPSocketEngine::ActiveSocketItem()
{
	CThreadLockHandle ItemLockedHandle(&m_ItemLocked,true);

	//��ȡ���ж���
	CServerSocketItem * pServerSocketItem=NULL;
	if (m_FreeSocketItem.GetCount()>0)
	{
		INT_PTR nItemPostion=m_FreeSocketItem.GetCount()-1;
		pServerSocketItem=m_FreeSocketItem[nItemPostion];
		GT_ASSERT(pServerSocketItem!=NULL);
		m_FreeSocketItem.RemoveAt(nItemPostion);
		m_ActiveSocketItem.Add(pServerSocketItem);
	}

	//�����¶���
	if (pServerSocketItem==NULL)
	{
		WORD wStorageCount=(WORD)m_StorageSocketItem.GetCount();
		if (wStorageCount<m_wMaxSocketItem)
		{
			_BEGIN_TRY
			{
				pServerSocketItem=new CServerSocketItem(wStorageCount,this);
				if (pServerSocketItem==NULL) return NULL;
				m_StorageSocketItem.Add(pServerSocketItem);
				m_ActiveSocketItem.Add(pServerSocketItem);
			}
			CATCH_COMMON_EXCEPTION(return NULL;)
				CATCH_UNKNOWN_EXCEPTION(return NULL;)
				_END_CATCH
		}
	}

	return pServerSocketItem;
}

//��ȡ���Ӷ���
CServerSocketItem * CTCPSocketEngine::EnumSocketItem(WORD wIndex)
{
	CThreadLockHandle ItemLockedHandle(&m_ItemLocked,true);
	if (wIndex<m_StorageSocketItem.GetCount())
	{
		CServerSocketItem * pServerSocketItem=m_StorageSocketItem[wIndex];
		GT_ASSERT(pServerSocketItem!=NULL);
		return pServerSocketItem;
	}
	return NULL;
}

//�ͷ����Ӷ���
bool CTCPSocketEngine::FreeSocketItem(CServerSocketItem * pServerSocketItem)
{
	//Ч�����
	GT_ASSERT(pServerSocketItem!=NULL);

	//�ͷŶ���
	CThreadLockHandle ItemLockedHandle(&m_ItemLocked,true);
	INT_PTR nActiveCount=m_ActiveSocketItem.GetCount();
	for (int i=0;i<nActiveCount;i++)
	{
		if (pServerSocketItem==m_ActiveSocketItem[i])
		{
			m_ActiveSocketItem.RemoveAt(i);
			m_FreeSocketItem.Add(pServerSocketItem);
			return true;
		}
	}

	//�ͷ�ʧ��
	GT_ASSERT(FALSE);
	return false;
}

//�������
bool __cdecl CTCPSocketEngine::DetectSocket()
{
	return m_SendQueueService.AddToQueue(QUEUE_DETECT_SOCKET,NULL,0);
}


//���ͺ���
bool __cdecl CTCPSocketEngine::SendData(WORD wIndex, WORD wRountID, WORD wMainCmdID, WORD wSubCmdID)
{
	//Ч��״̬
	GT_ASSERT(m_bService==true);
	if (m_bService==false) return false;

	//��������
	tagSendDataRequest SendRequest;
	SendRequest.wMainCmdID=wMainCmdID;
	SendRequest.wSubCmdID=wSubCmdID;
	SendRequest.wIndex=wIndex;
	SendRequest.wRountID=wRountID;
	SendRequest.wDataSize=0;

	//���뷢�Ͷ���
	WORD wSendSize=sizeof(SendRequest)-sizeof(SendRequest.cbSendBuf);
	return m_SendQueueService.AddToQueue(QUEUE_SEND_REQUEST,&SendRequest,wSendSize);
}

//���ͺ���
bool __cdecl CTCPSocketEngine::SendData(WORD wIndex, WORD wRountID, WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize)
{
	//Ч��״̬
	GT_ASSERT(m_bService==true);
	if (m_bService==false) return false;

	//Ч������
	GT_ASSERT((wDataSize)<=SOCKET_PACKAGE);
	if ((wDataSize)>SOCKET_PACKAGE) return false;

	//��������
	tagSendDataRequest SendRequest;
	SendRequest.wMainCmdID=wMainCmdID;
	SendRequest.wSubCmdID=wSubCmdID;
	SendRequest.wIndex=wIndex;
	SendRequest.wRountID=wRountID;
	SendRequest.wDataSize=wDataSize;
	if (wDataSize>0)
	{
		GT_ASSERT(pData!=NULL);
		CopyMemory(SendRequest.cbSendBuf,pData,wDataSize);
	}

	//���뷢�Ͷ���
	WORD wSendSize=sizeof(SendRequest)-sizeof(SendRequest.cbSendBuf)+wDataSize;
	return m_SendQueueService.AddToQueue(QUEUE_SEND_REQUEST,&SendRequest,wSendSize);
}

//��������
bool __cdecl CTCPSocketEngine::SendDataBatch(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize)
{
	//Ч��״̬
	if (m_bService==false) return false;

	//Ч������
	GT_ASSERT((wDataSize)<=SOCKET_PACKAGE);
	if ((wDataSize)>SOCKET_PACKAGE) return false;

	//��������
	tagSendDataRequest SendRequest;
	SendRequest.wMainCmdID=wMainCmdID;
	SendRequest.wSubCmdID=wSubCmdID;
	SendRequest.wIndex=INDEX_ALL_SOCKET;
	SendRequest.wRountID=0;
	SendRequest.wDataSize=wDataSize;
	if (wDataSize>0)
	{
		GT_ASSERT(pData!=NULL);
		CopyMemory(SendRequest.cbSendBuf,pData,wDataSize);
	}

	//���뷢�Ͷ���
	WORD wSendSize=sizeof(SendRequest)-sizeof(SendRequest.cbSendBuf)+wDataSize;
	return m_SendQueueService.AddToQueue(QUEUE_SEND_REQUEST,&SendRequest,wSendSize);
}

//�ر�����
bool __cdecl CTCPSocketEngine::CloseSocket(WORD wIndex, WORD wRountID)
{
	CServerSocketItem * pServerSocketItem=EnumSocketItem(wIndex);
	if (pServerSocketItem==NULL) return false;
	CThreadLockHandle SocketLockHandle(pServerSocketItem->GetSignedLock());
	return pServerSocketItem->CloseSocket(wRountID);
}

//���ùر�
bool __cdecl CTCPSocketEngine::ShutDownSocket(WORD wIndex, WORD wRountID)
{
	tagSafeCloseSocket SafeCloseSocket;
	SafeCloseSocket.wIndex=wIndex;
	SafeCloseSocket.wRountID=wRountID;
	return m_SendQueueService.AddToQueue(QUEUE_SAFE_CLOSE,&SafeCloseSocket,sizeof(SafeCloseSocket));
}

//����Ⱥ��
bool __cdecl CTCPSocketEngine::AllowBatchSend(WORD wIndex, WORD wRountID, bool bAllow)
{
	tagAllowBatchSend AllowBatchSend;
	AllowBatchSend.wIndex=wIndex;
	AllowBatchSend.wRountID=wRountID;
	AllowBatchSend.cbAllow=bAllow;
	return m_SendQueueService.AddToQueue(QUEUE_ALLOW_BATCH,&AllowBatchSend,sizeof(AllowBatchSend));
}

//////////////////////////////////////////////////////////////////////////

//����������
extern "C" __declspec(dllexport) void * __cdecl CreateTCPSocketEngine(const GUID & Guid, DWORD dwInterfaceVer)
{
	//��������
	CTCPSocketEngine * pTCPSocketEngine=NULL;
	_BEGIN_TRY
	{
		pTCPSocketEngine=new CTCPSocketEngine();
		if (pTCPSocketEngine==NULL) throw TEXT("����ʧ��");
		void * pObject=pTCPSocketEngine->QueryInterface(Guid,dwInterfaceVer);
		if (pObject==NULL) throw TEXT("�ӿڲ�ѯʧ��");
		return pObject;
	}
	CATCH_COMMON_EXCEPTION(;)CATCH_UNKNOWN_EXCEPTION(;)_END_CATCH


	//�������
	SafeDelete(pTCPSocketEngine);
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
