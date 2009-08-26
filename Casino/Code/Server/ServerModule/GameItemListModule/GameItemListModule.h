#ifndef LIST_SERVICE_HEAD_FILE
#define LIST_SERVICE_HEAD_FILE

//////////////////////////////////////////////////////////////////////////

//ƽ̨ͷ�ļ�
#include "..\..\Template\Template.h"
#include "..\..\CommonDefinition\GlobalDef.h"
#include "..\..\CommonDefinition\GlobalService.h"

//�������ļ�
#include "..\..\CommandDefinition\CMD_Game.h"
#include "..\..\CommandDefinition\CMD_Plaza.h"
#include "..\..\CommandDefinition\CMD_Center.h"

//���ͷ�ļ�
#include "..\..\Interface\IUnknownEx.h"
#include "..\..\ServerModule\KernelModule\KernelModule.h"

//////////////////////////////////////////////////////////////////////////
//�����궨��

//��������
#ifndef LIST_SERVICE_CLASS
	#ifdef  LIST_SERVICE_DLL
		#define LIST_SERVICE_CLASS _declspec(dllexport)
	#else
		#define LIST_SERVICE_CLASS _declspec(dllimport)
	#endif
#endif

//ģ�鶨��
#ifdef _DEBUG
	#define LIST_SERVICE_DLL_NAME	TEXT("GameItemListModuleD.dll")				//��� DLL ����
#else
	#define LIST_SERVICE_DLL_NAME	TEXT("GameItemListModule.dll")					//��� DLL ����
#endif

//////////////////////////////////////////////////////////////////////////
//�ṹ����

//��Ϸ����
struct tagGameTypeItem
{
	tagGameType						GameType;							//������Ϣ
};

//��Ϸ����
struct tagGameKindItem
{
	tagGameKind						GameKind;							//������Ϣ
};

//��Ϸվ��
struct tagGameStationItem
{
	tagGameStation					GameStation;						//վ����Ϣ
};

//��Ϸ����
struct tagGameServerItem
{
	DWORD							dwUpdateTime;						//����ʱ��
	WORD							wIndex;								//��������
	WORD							wRoundID;							//���ӱ�ʶ
	tagGameServer					GameServer;							//������Ϣ
};

//////////////////////////////////////////////////////////////////////////
//�ӿڶ���

#define VER_IServerListCenter INTERFACE_VERSION(2,1)
static const GUID IID_IServerListCenter={0x80533956,0x98e0,0x43e4,0xa6,0xe,0x11,0xff,0xa,0xb9,0x3c,0x2f};

//�����б��ӿ�
interface IServerListCenter : public IUnknownEx
{
	//���ýӿ�
public:
	//�����¼�
	virtual bool __cdecl SetEventService(IUnknownEx * pIUnknownEx)=NULL;

	//����ӿ�
public:
	//��������
	virtual bool __cdecl InsertGameType(tagGameType * pGameType)=NULL;
	//��������
	virtual bool __cdecl InsertGameKind(tagGameKind * pGameKind)=NULL;
	//���뷿��
	virtual bool __cdecl InsertGameServer(tagGameServer * pGameServer,
										WORD wIndex = 0xFFFF,WORD wRoundID = 0xFFFF)=NULL;
	//����վ��
	virtual bool __cdecl InsertGameStation(tagGameStation * pGameStation)=NULL;

	//ɾ���ӿ�
public:
	//ɾ������
	virtual bool __cdecl DeleteGameType(WORD wTypeID)=NULL;
	//ɾ������
	virtual bool __cdecl DeleteGameKind(WORD wKindID)=NULL;
	//ɾ������
	virtual bool __cdecl DeleteGameServer(WORD wKindID, WORD wServerID)=NULL;
	//ɾ��վ��
	virtual bool __cdecl DeleteGameStation(WORD wKindID, WORD wStationID)=NULL;

	//ö�ٽӿ�
public:
	//ö������
	virtual tagGameTypeItem * const __cdecl EmunGameTypeItem(POSITION & Pos)=NULL;
	//ö������
	virtual tagGameKindItem * const __cdecl EmunGameKindItem(POSITION & Pos)=NULL;
	//ö�ٷ���
	virtual tagGameServerItem * const __cdecl EmunGameServerItem(POSITION & Pos)=NULL;
	//ö��վ��
	virtual tagGameStationItem * const __cdecl EmunGameStationItem(POSITION & Pos)=NULL;

	//���ҽӿ�
public:
	//��������
	virtual tagGameTypeItem * const __cdecl SearchGameType(WORD wTypeID)=NULL;
	//��������
	virtual tagGameKindItem * const __cdecl SearchGameKind(WORD wKindID)=NULL;
	//���ҷ���
	virtual tagGameServerItem * const __cdecl SearchGameServer(WORD wKindID, WORD wServerID)=NULL;
	//����վ��
	virtual tagGameStationItem * const __cdecl SearchGameStation(WORD wKindID, WORD wStationID)=NULL;

	//��Ŀ�ӿ�
public:
	//������Ŀ
	virtual DWORD __cdecl GetGameTypeCount()=NULL;
	//������Ŀ
	virtual DWORD __cdecl GetGameKindCount()=NULL;
	//������Ŀ
	virtual DWORD __cdecl GetGameServerCount()=NULL;
	//վ����Ŀ
	virtual DWORD __cdecl GetGameStationCount()=NULL;

	//ά���ӿ�
public:
	//�����б�
	virtual void __cdecl ResetServerList()=NULL;
	//�����б�
	virtual bool __cdecl LoadServerList(tagDataBaseInfo & DataBaseInfo)=NULL;
	//����ȫ���б�
	virtual bool __cdecl LoadAllServerList(tagDataBaseInfo & DataBaseInfo)=NULL;
	//��������
	virtual bool __cdecl UpdateServerOnLineCount(WORD wKindID, WORD wServerID, DWORD dwOnLineCount)=NULL;
};

//////////////////////////////////////////////////////////////////////////

//ListService���������
class CServerListCenterHelper : public CTempldateHelper<IServerListCenter>
{
	//��������
public:
	//���캯��
	CServerListCenterHelper(void) : CTempldateHelper<IServerListCenter>(IID_IServerListCenter,
		VER_IServerListCenter,LIST_SERVICE_DLL_NAME,TEXT("CreateServerListCenter")) { }
};

//////////////////////////////////////////////////////////////////////////

#endif