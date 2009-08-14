#ifndef SERVER_KERNEL_HEAD_FILE
#define SERVER_KERNEL_HEAD_FILE

//////////////////////////////////////////////////////////////////////////

//ϵͳͷ�ļ�
#include <ICrsint.h>
#include <WinSock2.h>

//ƽ̨ͷ�ļ�
#include "..\..\Template\Template.h"
#include "..\..\CommonDefinition\GlobalDef.h"
#include "..\..\Interface\IUnknownEx.h"
#include "..\..\ShareModule\CommonModule\CommonModule.h"

//////////////////////////////////////////////////////////////////////////

//ADO �����
#import "MSADO15.DLL" rename_namespace("ADOCG") rename("EOF","EndOfFile")
using namespace ADOCG;

//COM ��������
typedef _com_error					CComError;							//COM ����

//////////////////////////////////////////////////////////////////////////
//�����궨��

//ģ�鶨��
#ifdef _DEBUG
	#define SERVER_KERNEL_DLL_NAME	TEXT("KernelModuleD.dll")			//��� DLL ����
#else
	#define SERVER_KERNEL_DLL_NAME	TEXT("KernelModule.dll")			//��� DLL ����
#endif

//�����궨��
#define EVENT_LEVEL_COUNT			4									//�¼��ȼ�
#define MAX_QUEUE_PACKET			10240								//������
#define INDEX_ALL_SOCKET			0xFFFF								//��������
#define TIMER_REPEAT_TIMER			DWORD(-1)							//�ظ�����

//////////////////////////////////////////////////////////////////////////
//ö�ٶ���

//����ȼ�
enum enTraceLevel
{
	Level_Normal					=0,									//��ͨ��Ϣ
	Level_Warning					=1,									//������Ϣ
	Level_Exception					=2,									//�쳣��Ϣ
	Level_Debug						=3,									//������Ϣ
};

//���ݿ�������
enum enADOErrorType
{
	ErrorType_Nothing				=0,									//û�д���
	ErrorType_Connect				=1,									//���Ӵ���
	ErrorType_Other					=2,									//��������
};

//////////////////////////////////////////////////////////////////////////
//�¼�����

//�¼���ʶ
#define EVENT_CONTROL				0x0001								//������Ϣ
#define EVENT_TIMER					0x0002								//��ʱ������
#define EVENT_DATABASE				0x0003								//���ݿ�����
#define EVENT_SOCKET_ACCEPT			0x0004								//����Ӧ��
#define EVENT_SOCKET_READ			0x0005								//�����ȡ
#define EVENT_SOCKET_CLOSE			0x0006								//����ر�

//��ʱ���¼�
struct NTY_TimerEvent
{
	WORD							wTimerID;							//��ʱ�� ID
	WPARAM							wBindParam;							//�󶨲���
};

//���ݿ������¼�
struct NTY_DataBaseEvent
{
	WORD							wIndex;								//��������
	WORD							wRoundID;							//�����ʶ
	WORD							wRequestID;							//�����ʶ
};

//����Ӧ���¼�
struct NTY_SocketAcceptEvent
{
	WORD							wIndex;								//��������
	WORD							wRoundID;							//���ӱ�ʶ
	DWORD							dwClientIP;							//���ӵ�ַ
};

//�����ȡ�¼�
struct NTY_SocketReadEvent
{
	WORD							wIndex;								//��������
	WORD							wRoundID;							//���ӱ�ʶ
	WORD							wDataSize;							//���ݴ�С
	CMD_Command						Command;							//������Ϣ
};

//����ر��¼�
struct NTY_SocketCloseEvent
{
	WORD							wIndex;								//��������
	WORD							wRoundID;							//���ӱ�ʶ
	DWORD							dwClientIP;							//���ӵ�ַ
	DWORD							dwConnectSecond;					//����ʱ��
};

//////////////////////////////////////////////////////////////////////////

#define VER_IADOError INTERFACE_VERSION(1,1)
static const GUID IID_IADOError={0x66463b5a,0x390c,0x42f9,0x85,0x19,0x13,0x31,0x39,0x36,0xfe,0x8f};

//���ݿ����ӿ�
interface IADOError : public IUnknownEx
{
	//��������
	virtual LPCTSTR __cdecl GetErrorDescribe()=NULL;
	//��������
	virtual enADOErrorType __cdecl GetErrorType()=NULL;
};

//////////////////////////////////////////////////////////////////////////

#define VER_IDataBase INTERFACE_VERSION(1,1)
static const GUID IID_IDataBase={0x9e962173,0x2a9f,0x4ebd,0x8e,0x98,0x40,0xe9,0x96,0x57,0x24,0xfb};

//���ݿ����ӽӿ�
interface IDataBase : public IUnknownEx
{
	//������
	virtual bool __cdecl OpenConnection()=NULL;
	//�رռ�¼
	virtual bool __cdecl CloseRecordset()=NULL;
	//�ر�����
	virtual bool __cdecl CloseConnection()=NULL;
	//��������
	virtual bool __cdecl TryConnectAgain(bool bFocusConnect, CComError * pComError)=NULL;
	//������Ϣ
	virtual bool __cdecl SetConnectionInfo(LPCTSTR szIP, LPCTSTR szPipeName,WORD wPort, LPCTSTR szData, LPCTSTR szName, LPCTSTR szPass)=NULL;
	//�Ƿ����Ӵ���
	virtual bool __cdecl IsConnectError()=NULL;
	//�Ƿ��
	virtual bool __cdecl IsRecordsetOpened()=NULL;
	//�����ƶ�
	virtual void __cdecl MoveToNext()=NULL;
	//�Ƶ���ͷ
	virtual void __cdecl MoveToFirst()=NULL;
	//�Ƿ����
	virtual bool __cdecl IsEndRecordset()=NULL;
	//��ȡ��Ŀ
	virtual long __cdecl GetRecordCount()=NULL;
	//��ȡ��С
	virtual long __cdecl GetActualSize(LPCTSTR pszParamName)=NULL;
	//�󶨶���
	virtual bool __cdecl BindToRecordset(CADORecordBinding * pBind)=NULL;
	//��һ��¼��
	virtual bool __cdecl NextRecordset()=NULL;
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, BYTE & bValue)=NULL;
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, WORD & wValue)=NULL;
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, INT & nValue)=NULL;
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, LONG & lValue)=NULL;
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, DWORD & ulValue)=NULL;
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, UINT & ulValue)=NULL;
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, DOUBLE & dbValue)=NULL;
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, __int64 & llValue)=NULL;
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, ULONGLONG & ullValue)=NULL;
	//��ȡ����
	virtual UINT __cdecl GetFieldValue(LPCTSTR lpFieldName, TCHAR szBuffer[], UINT uSize)=NULL;
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, COleDateTime & Time)=NULL;
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, bool & bValue)=NULL;
	//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, DECIMAL & decValue)=NULL;
	//���ô洢����
	virtual void __cdecl SetSPName(LPCTSTR pszSpName)=NULL;
	//�������
	virtual void __cdecl AddParamter(LPCTSTR pszName, ADOCG::ParameterDirectionEnum Direction, ADOCG::DataTypeEnum Type, long lSize, _variant_t & vtValue)=NULL;
	//�������
	virtual void __cdecl AddDecimalParamter(LPCTSTR pszName, ADOCG::ParameterDirectionEnum Direction, DOUBLE& dbValue, 
		unsigned char Precision = 24, unsigned char NumericScale = 4)=NULL;
	//�������
	virtual void __cdecl AddDecimalParamter(LPCTSTR pszName, ADOCG::ParameterDirectionEnum Direction, DECIMAL& decValue, 
		unsigned char Precision = 24, unsigned char NumericScale = 4)=NULL;
	//ɾ������
	virtual void __cdecl ClearAllParameters()=NULL;
	//��ò���
	virtual void __cdecl GetParameterValue(LPCTSTR pszParamName, _variant_t & vtValue)=NULL;
	//��ȡ������ֵ
	virtual long __cdecl GetReturnValue()=NULL;
	//ִ�����
	virtual bool __cdecl Execute(LPCTSTR pszCommand)=NULL;
	//ִ������
	virtual bool __cdecl ExecuteCommand(bool bRecordset)=NULL;
};

//////////////////////////////////////////////////////////////////////////

#define VER_IDataBaseSink INTERFACE_VERSION(1,1)
static const GUID IID_IDataBaseSink={0x1287cdc2,0xcbda,0x4bb6,0x95,0xeb,0xd9,0xcc,0x6a,0x2b,0xef,0xd3};

//���ݿ⹳�ӽӿ�
interface IDataBaseSink : public IUnknownEx
{
	//���ݿ�ģ������
	virtual bool __cdecl StartService(IUnknownEx * pIUnknownEx)=NULL;
	//���ݿ�ģ��ر�
	virtual bool __cdecl StopService(IUnknownEx * pIUnknownEx)=NULL;
	//���ݲ�������
	virtual bool __cdecl OnDataBaseRequest(BYTE cbThreadIndex,const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize)=NULL;
};

//////////////////////////////////////////////////////////////////////////

#define VER_IDataBaseEngine INTERFACE_VERSION(1,1)
static const GUID IID_IDataBaseEngine={0x9b00b2a6,0x620e,0x4791,0x97,0xad,0x28,0x32,0x16,0xf1,0x8c,0xdf};

//���ݿ�����ӿ�
interface IDataBaseEngine : public IUnknownEx
{
	//��������
	virtual bool __cdecl StartService(BYTE cbThreadCount)=NULL;
	//ֹͣ����
	virtual bool __cdecl StopService()=NULL;
	//ע�ṳ��
	virtual bool __cdecl SetDataBaseSink(IUnknownEx * pIUnknownEx)=NULL;
	//��ȡ�ӿ�
	virtual void * __cdecl GetQueueService(const IID & Guid, DWORD dwQueryVer)=NULL;
};

//////////////////////////////////////////////////////////////////////////

#define VER_IQueueServiceSink INTERFACE_VERSION(1,1)
static const GUID IID_IQueueServiceSink={0x88b5bf59,0x3a98,0x43b1,0xac,0x85,0xf1,0x17,0x5b,0x29,0x69,0x40};

//���ݶ����๳�ӽӿ�
interface IQueueServiceSink : public IUnknownEx
{
	//֪ͨ�ص�����
	virtual void __cdecl OnQueueServiceSink(BYTE cbThreadIndex,
		WORD wIdentifier, void * pBuffer, WORD wDataSize, DWORD dwInsertTime)=NULL;
};

//////////////////////////////////////////////////////////////////////////

#define VER_IQueueService INTERFACE_VERSION(1,1)
static const GUID IID_IQueueService={0xcc5310b5,0x3709,0x40aa,0x85,0x24,0xd6,0xc5,0x87,0xb0,0x32,0x22};

//���ݶ��нӿ�
interface IQueueService : public IUnknownEx
{
	//��������
	virtual bool __cdecl AddToQueue(WORD wIdentifier, void * const pBuffer, WORD wDataSize)=NULL;
};

//////////////////////////////////////////////////////////////////////////

#define VER_IQueueServiceEngine INTERFACE_VERSION(1,1)
static const GUID IID_IQueueServiceEngine={0xba2c602e,0x7f5a,0x40ad,0xb7,0xa5,0x7d,0x1c,0x2a,0x8f,0xee,0x78};

//����������ӿ�
interface IQueueServiceEngine : public IUnknownEx
{
	//��ʼ����
	virtual bool __cdecl StartService(BYTE cbThreadCount)=NULL;
	//ֹͣ����
	virtual bool __cdecl StopService()=NULL;
	//���ýӿ�
	virtual bool __cdecl SetQueueServiceSink(IUnknownEx * pIUnknownEx)=NULL;
	//������Ϣ
	virtual bool __cdecl GetBurthenInfo(tagBurthenInfo & BurthenInfo)=NULL;
};

//////////////////////////////////////////////////////////////////////////

#define VER_IEventService INTERFACE_VERSION(1,1)
static const GUID IID_IEventService={0x729d0499,0xad36,0x49d5,0x8d,0x46,0xfd,0xd,0x78,0x82,0xa6,0x37};

//�¼���������ӿ�
interface IEventService : public IUnknownEx
{
	//���þ��
	virtual bool __cdecl SetRichEditHwnd(HWND hRichEdit)=NULL;
	//���ü���
	virtual void __cdecl ConfigEventService(enTraceLevel TraceLevel, bool bShow)=NULL;
	//�¼�֪ͨ
	virtual void __cdecl ShowEventNotify(LPCTSTR pszString, enTraceLevel TraceLevel)=NULL;
};
//////////////////////////////////////////////////////////////////////////

#define VER_IUpdateEventSink INTERFACE_VERSION(1,1)
static const GUID IID_IUpdateEventSink={0x729d0491,0xad36,0x49d5,0x8d,0x46,0xfd,0xd,0x78,0x82,0xa6,0x37};

//�����¼����ӽӿ�
interface IUpdateEventSink : public IUnknownEx
{
	//��������֪ͨ
	virtual void __cdecl ShowUserCountNotify(WORD wLogonUserCount,WORD wGameUserCount)=NULL;
};
//////////////////////////////////////////////////////////////////////////

#define VER_IEnumUserSink INTERFACE_VERSION(1,1)
static const GUID IID_IEnumUserSink={0x729d0492,0xad36,0x49d5,0x8d,0x46,0xfd,0xd,0x78,0x82,0xa6,0x37};

//ö���û����ӽӿ�
interface IEnumUserSink : public IUnknownEx
{
	//�û�����
	virtual void __cdecl OnUserData(const DWORD&	dwRealUserID,
						const BYTE&   cbUserType,
						const tagUserData& UserData,
						const DWORD&	dwClientIP)=NULL;
};
//////////////////////////////////////////////////////////////////////////

#define VER_ITimerEngine INTERFACE_VERSION(1,1)
static const GUID IID_ITimerEngine={0x4a9b0421,0x1224,0x4e5e,0xa5,0x4f,0xb1,0x61,0x6e,0x62,0xdb,0x98};

//��ʱ������ӿ�
interface ITimerEngine : public IUnknownEx
{
	//���ö�ʱ��
	virtual bool __cdecl SetTimer(WORD wTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM wParam)=NULL; 
	//ɾ����ʱ��
	virtual bool __cdecl KillTimer(WORD wTimerID)=NULL;
	//ɾ����ʱ��
	virtual bool __cdecl KillAllTimer()=NULL;
};

//////////////////////////////////////////////////////////////////////////

#define VER_ITimerEngineManager INTERFACE_VERSION(1,1)
static const GUID IID_ITimerEngineManager={0xcdbfa840,0xdc45,0x41f7,0xb6,0x3c,0xad,0x83,0x75,0x98,0xc6,0x3e};

//��ʱ������ӿ�
interface ITimerEngineManager : public IUnknownEx
{
	//��ʼ����
	virtual bool __cdecl StartService()=NULL;
	//ֹͣ����
	virtual bool __cdecl StopService()=NULL;
	//���ýӿ�
	virtual bool __cdecl SetTimerEngineSink(IUnknownEx * pIUnknownEx)=NULL;
};

//////////////////////////////////////////////////////////////////////////

#define VER_ITCPSocketEngine INTERFACE_VERSION(1,1)
static const GUID IID_ITCPSocketEngine={0x9979ea68,0x5ee7,0x4c15,0xa4,0xdf,0x9f,0xe3,0x85,0x26,0x13,0xbb};

//TCP SOCKET ����ӿ�
interface ITCPSocketEngine : public IUnknownEx
{
	//�������
	virtual bool __cdecl DetectSocket()=NULL;
	//���ͺ���
	virtual bool __cdecl SendData(WORD wIndex, WORD wRoundID, WORD wMainCmdID, WORD wSubCmdID)=NULL;
	//���ͺ���
	virtual bool __cdecl SendData(WORD wIndex, WORD wRoundID, WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize)=NULL;
	//��������
	virtual bool __cdecl SendDataBatch(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize)=NULL;
	//�ر�����
	virtual bool __cdecl CloseSocket(WORD wIndex, WORD wRoundID)=NULL;
	//�ر�����
	virtual bool __cdecl ShutDownSocket(WORD wIndex, WORD wRoundID)=NULL;
	//����Ⱥ��
	virtual bool __cdecl AllowBatchSend(WORD wIndex, WORD wRoundID, bool bAllow)=NULL;
};

//////////////////////////////////////////////////////////////////////////

#define VER_ITCPSocketEngineManager INTERFACE_VERSION(1,1)
static const GUID IID_ITCPSocketEngineManager={0xf91b246f,0x3c97,0x4445,0xb3,0xeb,0xb9,0x42,0x3b,0x68,0x1b,0xed};

//TCP SOCKET �������ӿ�
interface ITCPSocketEngineManager : public IUnknownEx
{
	//���ýӿ�
	virtual bool __cdecl SetSocketEngineSink(IUnknownEx * pIUnknownEx)=NULL;
	//���ö˿�
	virtual bool __cdecl SetServicePort(WORD wListenPort)=NULL;
	//������Ŀ
	virtual bool __cdecl SetMaxSocketItem(WORD wMaxSocketItem)=NULL;
	//��������
	virtual bool __cdecl StartService()=NULL;
	//ֹͣ����
	virtual bool __cdecl StopService()=NULL;
};

//////////////////////////////////////////////////////////////////////////

#define VER_IAttemperEngineSink INTERFACE_VERSION(1,1)
static const GUID IID_IAttemperEngineSink={0x82d2c5,0x4e6f,0x4031,0xae,0xd4,0x6f,0xee,0x5a,0x17,0x1b,0x8a};

//����ģ�鹳�ӽӿ�
interface IAttemperEngineSink : public IUnknownEx
{
	//����ӿ�
public:
	//����ģ������
	virtual bool __cdecl StartService(IUnknownEx * pIUnknownEx)=NULL;
	//����ģ��ر�
	virtual bool __cdecl StopService(IUnknownEx * pIUnknownEx)=NULL;
	//�¼�����ӿ�
	virtual bool __cdecl OnAttemperEvent(BYTE cbThreadIndex,WORD wIdentifier, void * pBuffer, WORD wDataSize, DWORD dwInsertTime)=NULL;

	//�¼��ӿ�
public:
	//��ʱ���¼�
	virtual bool __cdecl OnEventTimer(BYTE cbThreadIndex,WORD wTimerID, WPARAM wBindParam)=NULL;
	//���ݿ��¼�
	virtual bool __cdecl OnEventDataBase(BYTE cbThreadIndex,void * pDataBuffer, WORD wDataSize, NTY_DataBaseEvent * pDataBaseEvent)=NULL;
	//����Ӧ���¼�
	virtual bool __cdecl OnEventSocketAccept(BYTE cbThreadIndex,NTY_SocketAcceptEvent * pSocketAcceptEvent)=NULL;
	//�����ȡ�¼�
	virtual bool __cdecl OnEventSocketRead(BYTE cbThreadIndex,CMD_Command Command, void * pDataBuffer, WORD wDataSize, NTY_SocketReadEvent * pSocketReadEvent)=NULL;
	//����ر��¼�
	virtual bool __cdecl OnEventSocketClose(BYTE cbThreadIndex,NTY_SocketCloseEvent * pSocketCloseEvent)=NULL;
};

//////////////////////////////////////////////////////////////////////////

#define VER_IAttemperEngine INTERFACE_VERSION(1,1)
static const GUID IID_IAttemperEngine={0x967b3e64,0x69af,0x434a,0xb8,0x6c,0x67,0x9b,0x72,0xbb,0x2e,0xe};

//��������ӿ�
interface IAttemperEngine : public IUnknownEx
{
	//����ӿ�
public:
	//��������
	virtual bool __cdecl StartService(BYTE cbThreadCount)=NULL;
	//ֹͣ����
	virtual bool __cdecl StopService()=NULL;
	//��������
	virtual bool __cdecl SetSocketEngine(IUnknownEx * pIUnknownEx)=NULL;
	//���ù���
	virtual bool __cdecl SetAttemperEngineSink(IUnknownEx * pIUnknownEx)=NULL;
	//��ȡ�ӿ�
	virtual void * __cdecl GetQueueService(const IID & Guid, DWORD dwQueryVer)=NULL;
};

//////////////////////////////////////////////////////////////////////////

#define VER_IServiceEngine INTERFACE_VERSION(3,1)
static const GUID IID_IServiceEngine={0x74284fc0,0xc587,0x4c21,0x9b,0x7c,0x16,0x76,0x27,0x1a,0x7b,0xd5};

//��������ӿ�
interface IServiceEngine : public IUnknownEx
{
	//����ӿ�
public:
	//��������
	virtual bool __cdecl StartService(BYTE cbDBThreadCount = 12,
		BYTE cbAttemperThreadCount = 1)=NULL;
	//ֹͣ����
	virtual bool __cdecl StopService()=NULL;

	//���ýӿ�
public:
	//�����¼�
	virtual bool __cdecl SetEventService(IUnknownEx * pIUnknownEx)=NULL;
	//���ù���
	virtual bool __cdecl SetDataBaseSink(IUnknownEx * pIUnknownEx)=NULL;
	//���ù���
	virtual bool __cdecl SetAttemperEngineSink(IUnknownEx * pIUnknownEx)=NULL;
	//ע�ṳ��
	virtual bool __cdecl RegisterAsynchronismEngineSink(IUnknownEx * pIUnknownEx)=NULL;
	//������������ӿ�
	virtual bool __cdecl InitServiceEngine(WORD wListenPort, WORD wMaxSocketItem)=NULL;

	//�����ѯ
public:
	//��ȡ��ʱ���ӿ�
	virtual void * __cdecl GetTimerEngine(const IID & Guid, DWORD dwQueryVer)=NULL;
	//��ȡ���ݿ�����ӿ�
	virtual void * __cdecl GetDataBaseEngine(const IID & Guid, DWORD dwQueryVer)=NULL;
	//��ȡ��������ӿ�
	virtual void * __cdecl GetAttemperEngine(const IID & Guid, DWORD dwQueryVer)=NULL;
	//��ȡ��������ӿ�
	virtual void * __cdecl GetTCPSocketEngine(const IID & Guid, DWORD dwQueryVer)=NULL;
	//��ȡ�첽����ӿ�
	virtual void * __cdecl GetAsynchronismEngine(const IID & Guid, DWORD dwQueryVer)=NULL;
	//��ȡ�ӿ�
	virtual void * __cdecl GetDataBaseQueueService(const IID & Guid, DWORD dwQueryVer)=NULL;
	//��ȡ�ӿ�
	virtual void * __cdecl GetAttemperQueueService(const IID & Guid, DWORD dwQueryVer)=NULL;

	//���ܽӿ�
public:
	//����״̬
	virtual bool __cdecl IsService()=NULL;
	//�ⲿ����
	virtual bool __cdecl ControlService(void * pBuffer, WORD wDataSize)=NULL;
};

//////////////////////////////////////////////////////////////////////////

#define VER_IAsynchronismEngine INTERFACE_VERSION(1,1)
static const GUID IID_IAsynchronismEngine={0x3728c1a5,0x37a9,0x4ace,0x8e,0x31,0xd9,0xb1,0xce,0x63,0xd9,0x88};

//�첽����ӿ�
interface IAsynchronismEngine : public IUnknownEx
{
	//����ӿ�
public:
	//��������
	virtual bool __cdecl StartService()=NULL;
	//ֹͣ����
	virtual bool __cdecl StopService()=NULL;
	//��������
	virtual bool __cdecl InsertRequest(WORD wRequestID, void * const pBuffer, WORD wDataSize, IUnknownEx * pIUnknownEx)=NULL;

	//���ܽӿ�
public:
	//ע�ṳ��
	virtual bool __cdecl RegisterAsynchronismEngineSink(IUnknownEx * pIUnknownEx)=NULL;
	//ȡ��ע��
	virtual bool __cdecl UnRegisterAsynchronismEngineSink(IUnknownEx * pIUnknownEx)=NULL;
};

//////////////////////////////////////////////////////////////////////////

#define VER_IAsynchronismEngineSink INTERFACE_VERSION(1,1)
static const GUID IID_IAsynchronismEngineSink={0x6898f671,0x5c4b,0x4555,0x8c,0x82,0x49,0xc4,0xf3,0x2a,0xeb,0x4d};

//�첽���湳�ӽӿ�
interface IAsynchronismEngineSink : public IUnknownEx
{
	//�����¼�
	virtual bool __cdecl OnAsynchronismEngineStart()=NULL;
	//ֹͣ�¼�
	virtual bool __cdecl OnAsynchronismEngineStop()=NULL;
	//�첽����
	virtual bool __cdecl OnAsynchronismRequest(WORD wRequestID, void * pBuffer, WORD wDataSize)=NULL;
};

//////////////////////////////////////////////////////////////////////////

//��������ļ�
#include "QueueServiceEvent.h"
#include "KernelModuleHelper.h"

//////////////////////////////////////////////////////////////////////////

#endif