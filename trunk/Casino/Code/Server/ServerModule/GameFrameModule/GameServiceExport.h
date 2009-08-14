#ifndef GAME_SERVICE_EXPORT_HEAD_FILE
#define GAME_SERVICE_EXPORT_HEAD_FILE

//////////////////////////////////////////////////////////////////////////

//ƽ̨ͷ�ļ�
#include "..\..\Template\Template.h"
#include "..\..\CommonDefinition\GlobalDef.h"
#include "..\..\CommonDefinition\GlobalField.h"
#include "..\..\CommonDefinition\GlobalFrame.h"
#include "..\..\CommonDefinition\GlobalRight.h"
#include "..\..\CommonDefinition\GlobalService.h"
#include "..\..\CommonDefinition\GlobalAI.h"

//�������ļ�
#include "..\..\CommandDefinition\CMD_Game.h"
#include "..\..\CommandDefinition\CMD_Plaza.h"
#include "..\..\CommandDefinition\CMD_Center.h"

//���ͷ�ļ�
#include "..\..\Interface\IUnknownEx.h"
#include "..\..\ShareModule\SocketModule\SocketModule.h"
#include "..\..\ServerModule\KernelModule\KernelModule.h"

//////////////////////////////////////////////////////////////////////////
//�����궨��

//��������
#ifndef GAME_SERVICE_CLASS
	#ifdef  GAME_SERVICE_DLL
		#define GAME_SERVICE_CLASS _declspec(dllexport)
	#else
		#define GAME_SERVICE_CLASS _declspec(dllimport)
	#endif
#endif

//ģ�鶨��
#ifdef _DEBUG
	#define GAME_SERVICE_DLL_NAME	TEXT("GameFrameModuleD.dll")				//��� DLL ����
#else
	#define GAME_SERVICE_DLL_NAME	TEXT("GameFrameModule.dll")					//��� DLL ����
#endif

//�궨��
#define IDI_MAX_TIME_ID					30									//���޶�ʱ��

//����ԭ��
#define GER_NORMAL						0x00								//�������
#define GER_DISMISS						0x01								//��Ϸ��ɢ
#define GER_USER_LEFT					0x02								//�û��뿪

#define INVALID_CALCULATEITEMPOS		0xffffffff							//��Ч���㵥Ԫλ��

//////////////////////////////////////////////////////////////////////////
//ö�ٶ���

//��ʼģʽ
enum enStartMode
{
	enStartMode_FullReady,				//���˿�ʼ
	enStartMode_AllReady,				//����׼��
	enStartMode_Symmetry,				//�Գƿ�ʼ
	enStartMode_TimeControl,			//ʱ�����
};

//�������
enum enScoreKind
{
	enScoreKind_Win,					//ʤ
	enScoreKind_Lost,					//��
	enScoreKind_Draw,					//��
	enScoreKind_Flee,					//��
};

//////////////////////////////////////////////////////////////////////////
//�ṹ����

//���ö����Ϣ
struct tagScoreInfo
{
	DOUBLE								fScore;								//��Ϸ���ö��
	enScoreKind							ScoreKind;							//�������
};

//�û�����
struct tagUserRule
{
	bool								bPassword;							//��������
	bool								bLimitWin;							//����ʤ��
	bool								bLimitFlee;							//��������
	bool								bLimitScore;						//���ƶ��
	bool								bLimitDelay;						//������ʱ
	bool								bCheckSameIP;						//Ч���ַ
	WORD								wWinRate;							//ʤ������
	WORD								wFleeRate;							//��������
	WORD								wNetDelay;							//������ʱ
	LONG								lMaxScore;							//��߶�� 
	LONG								lLessScore;							//��Ͷ��
	TCHAR								szPassword[PASS_LEN];				//��������
};

//�û���Ϣ�ṹ
struct tagServerUserData
{
	//������Ϣ
	tagUserData							UserData;							//�û���Ϣ
	BYTE								cbUserType;							//�û�����
	DWORD								dwRealUserID;						//�û�ID

	DECIMAL								decOccupancyRate;					//ռ�ɱ�
	DECIMAL								decTaxOccupancyRate;			//��ˮռ�ɱ�
	DECIMAL								decRollbackRate;					//ϴ���
	DWORD								dwGameRight;						//��ϷȨ��

	//״̬��Ϣ
	tagUserStatus						UserStatus;
};

//���Ӳ����ṹ
struct tagTableFrameParameter
{
	IUnknownEx							* pIGameServiceFrame;				//������
	IUnknownEx							* pIGameServiceManager;				//�������
	IUnknownEx							* pIDataBaseEngine;					//���ݿ�
	IUnknownEx							* pIServerUserManager;				//�û�����
	const tagGameServiceAttrib			* pGameServiceAttrib;				//��������
	const tagGameServiceOption			* pGameServiceOption;				//���ò���
};

//��������ṹ
struct tagGameServiceParameter
{
	TCHAR								szModuleName[MODULE_LEN];			//ģ������
	DWORD								dwCenterAddr;						//���ĵ�ַ
	tagDataBaseInfo						GameUserDBInfo;						//��������
	tagDataBaseInfo						GameScoreDBInfo;					//��������
	tagGameServiceOption				GameServiceOption;					//��������
};

//////////////////////////////////////////////////////////////////////////

#define VER_IServerUserItem INTERFACE_VERSION(2,1)
static const GUID IID_IServerUserItem={0x8fbde5db,0x99c5,0x4908,0x91,0xed,0x74,0xea,0xe4,0xc0,0xa7,0x8d};

//�������û��ӿ�
interface IServerUserItem : public IUnknownEx
{
	//��Ϣ�ӿ�
public:
	//�Ƿ񼤻�
	virtual bool __cdecl IsActive()=NULL;
	//�û�����
	virtual WORD __cdecl GetUserIndex()=NULL;
	//��ȡ��ַ
	virtual DWORD __cdecl GetClientIP()=NULL;
	//��ȡ����
	virtual LPCTSTR __cdecl GetPassword()=NULL;
	//��ȡ����
	virtual const tagUserRule * __cdecl GetUserRule()=NULL;
	//��ȡ���ö��
	virtual  tagUserScore * __cdecl GetUserScore()=NULL;
	//��ȡ��Ϣ
	virtual tagServerUserData * __cdecl GetUserData()=NULL;
	//��ȡ���û�
	virtual IServerUserItem * __cdecl GetParent()=NULL;
	//���ø��û�
	virtual bool  __cdecl SetParent(IServerUserItem* pParent)=NULL;

	//���Խӿ�
public:
	//�û�����
	virtual LPCTSTR __cdecl GetAccounts()=NULL;
	//�û� I D
	virtual DWORD __cdecl GetUserID()=NULL;
	//���û� I D
	virtual DWORD __cdecl GetParentUserID()=NULL;
	//�û��ȼ�
	virtual BYTE __cdecl GetUserLevel()=NULL;
	//���Ӻ���
	virtual WORD __cdecl GetTableID()=NULL;
	//���Ӻ���
	virtual WORD __cdecl GetChairID()=NULL;
	//�û�״̬
	virtual BYTE __cdecl GetUserStatus()=NULL;
	//�ж��û���ϵ��ȷ
	virtual bool __cdecl IsUserTreeFrameOK()=NULL;
	//�жϻ�����
	virtual bool __cdecl IsAIUser()=NULL;

	//ͳ�ƽӿ�
public:
	//��Ϸʱ��
	virtual DWORD __cdecl GetPlayTimeCount()=NULL;
	//����ʱ��
	virtual DWORD __cdecl GetOnlineTimeCount()=NULL;
	//���ö����Ϣ
	virtual bool __cdecl GetUserScoreInfo(tagUserScore & ScoreResultInfo)=NULL;

	//���ýӿ�
public:
	//��������
	virtual void __cdecl AddUserRefCount()=NULL;
	//��������
	virtual void __cdecl ReleaseUserRefCount()=NULL;
	//����Ϊ��
	virtual bool __cdecl IsUserRefCountZero()=NULL;
	//���ܽӿ�
public:
	//���ù���
	virtual bool __cdecl SetUserRule(tagUserRule & UserRule)=NULL;
	//�޸����ö��
    virtual bool __cdecl WriteScore(tagScoreInfo & ScoreInfo, DWORD dwPlayTimeCount)=NULL;
	//����״̬
	virtual bool __cdecl SetUserStatus(BYTE cbUserStatus, WORD wTableID, WORD wChairID)=NULL;

	//���㸨���ӿ�
public:
	virtual DWORD __cdecl GetCalculateItemPos(WORD wTableID)=NULL;
	virtual void  __cdecl SetCalculateItemPos(WORD wTableID, DWORD dwItemPos)=NULL;
};

//////////////////////////////////////////////////////////////////////////

#define VER_IServerUserManager INTERFACE_VERSION(2,1)
static const GUID IID_IServerUserManager={0xa27d1d5d,0x5999,0x488e,0xaa,0x4a,0x30,0x8a,0x1,0x11,0xf4,0x9e};

//�������û�����ӿ�
interface IServerUserManager : public IUnknownEx
{
	//����ӿ�
public:
	//�����û�
	virtual bool __cdecl ResetUserManager()=NULL;
	//�л��û�
	virtual bool __cdecl SwitchOnLineUserItem(IServerUserItem * pIServerUserItem, DWORD dwClientIP, WORD wUserIndex)=NULL;
	//�����û�
	virtual bool __cdecl ActiveOffLineUserItem(IServerUserItem * pIServerUserItem, DWORD dwClientIP, WORD wUserIndex)=NULL;
	//�����û�
	virtual IServerUserItem * __cdecl ActiveUserItem(tagServerUserData & ServerUserData, DWORD dwClientIP, WORD wUserIndex, TCHAR szPassword[PASS_LEN])=NULL;
	//�����û�
	virtual IServerUserItem * __cdecl ActiveLinkUserItem(tagServerUserData & ServerUserData)=NULL;
	//ɾ���û�
	virtual bool __cdecl DeleteUserItem(IServerUserItem * pIServerUserItem)=NULL;
	//ע�����
	virtual bool __cdecl RegOffLineUserItem(IServerUserItem * pIServerUserItem)=NULL;
	//ɾ������
	virtual bool __cdecl UnRegOffLineUserItem(IServerUserItem * pIServerUserItem)=NULL;
	//����û�����
	virtual bool __cdecl CheckGameUserRefCount(IServerUserItem * pIServerUserItem)=NULL;

	//���ҽӿ�
public:
	//ö���û�
	virtual IServerUserItem * __cdecl EnumOnLineUser(WORD wEnumIndex)=NULL;
	//ö���û�
	virtual IServerUserItem * __cdecl EnumOffLineUser(WORD wEnumIndex)=NULL;
	//�����û�
	virtual IServerUserItem * __cdecl SearchOnLineUser(DWORD dwUserID)=NULL;
	//�����û�
	virtual IServerUserItem * __cdecl SearchOffLineUser(DWORD dwUserID)=NULL;
	//�����û�
	virtual IServerUserItem * __cdecl SearchUser(DWORD dwUserID)=NULL;

	//��Ϣ�ӿ�
public:
	//��������
	virtual DWORD __cdecl GetOnLineCount()=NULL;
	//��������
	virtual DWORD __cdecl GetOffLineCount()=NULL;
};
//////////////////////////////////////////////////////////////////////////
#define VER_ICalculateItem INTERFACE_VERSION(1,1)
// {F1803198-BEA8-48a4-B410-2A7F8624CEF8}
static const GUID IID_ICalculateItem = { 0xf1803198, 0xbea8, 0x48a4, { 0xb4, 0x10, 0x2a, 0x7f, 0x86, 0x24, 0xce, 0xf8 } };

//�����ܵ�Ԫ�ӿ�
interface ICalculateItem : public IUnknownEx
{
	//�û� I D
	virtual DWORD __cdecl GetUserID()=NULL;
	//���û� I D
	virtual DWORD __cdecl GetParentUserID()=NULL;
	//�û��ȼ�
	virtual BYTE __cdecl GetUserLevel()=NULL;
	//��ȡ��Ϣ
	virtual tagServerUserData * __cdecl GetUserData()=NULL;
	//��ȡ�û�����ID
	virtual WORD __cdecl GetChairID()=NULL;
	//�û���ϵ��ȷ
	virtual bool  __cdecl IsUserTreeFrameOK()=NULL;

	//��ȡ��Ͷע
	virtual bool __cdecl GetTotalBetScore(DECIMAL* pTotalBetScore)=NULL;
	//��ȡ���ɲ�
	virtual bool __cdecl GetTotalWinScore(DECIMAL* pTotalWinScore)=NULL;
	//��ȡ�ܳ�ˮ
	virtual bool __cdecl GetTotalTaxScore(DECIMAL* pTotalTaxScore)=NULL;

	//��ȡ�ɲ�ռ��
	virtual bool __cdecl GetWinScoreOccupancy(DECIMAL* pHighScore,
		DECIMAL* pSelfScore,DECIMAL *pLessScore)=NULL;
	//��ȡ��ˮռ��
	virtual bool __cdecl GetTaxScoreOccupancy(DECIMAL* pHighScore,
		DECIMAL* pSelfScore,DECIMAL *pLessScore)=NULL;
	//��ȡϴ���뽻�ն��
	virtual bool __cdecl GetValidBetScoreAndPaidScore(DECIMAL* pTotalScore,
		DECIMAL* pHighScore, DECIMAL *pHighRollbackScore,
		DECIMAL* pLessScore, DECIMAL *pLessRollbackScore,
		DECIMAL* pHighPaidScore, DECIMAL* pLessPaidScore)=NULL;

	//��ȡͶע��ϸ��Ϣ
	virtual bool __cdecl GetDetailBetScore(BYTE *pDetailBetScore,
		WORD& wDetailBetScoreSize)=NULL;
};
//////////////////////////////////////////////////////////////////////////
//�����ܻص��ӿ�
#define VER_ICalculateSink INTERFACE_VERSION(1,1)
// {F8B5E4B3-A39A-4f59-8501-18D90A7B0A78}
static const GUID IID_ICalculateSink = { 0xf8b5e4b3, 0xa39a, 0x4f59, { 0x85, 0x1, 0x18, 0xd9, 0xa, 0x7b, 0xa, 0x78 } };

//�����ܻص��ӿ�
interface ICalculateSink : public IUnknownEx
{
	//�����ɲ�
	virtual bool __cdecl CalculateResult(void* pCalculateContext,
		WORD wChairID,
		ICalculateItem* pItem,
		DECIMAL* pPartedBetScore,
		DECIMAL* pPartedWinScore,
		DECIMAL* pPartedTaxScore,
		WORD	 wBetRegionCount)=NULL;

	//����ϴ��
	virtual bool __cdecl RectifyValidBetScore(void* pCalculateContext,
		ICalculateItem* pItem,
		DECIMAL* pPartedBetScore,
		WORD	 wBetRegionCount,
		DECIMAL* pValidBetScore)=NULL;
};
//////////////////////////////////////////////////////////////////////////
//Ͷע��Ϣ
struct tagBetScoreInfo
{
	IServerUserItem *	pIServerUserItem;	
	WORD				wBetRegionIndex;
	DWORD				dwBetScore;
};

//�����ܽӿ�
#define VER_ICalculateFrame INTERFACE_VERSION(1,1)
// {85FBFF21-B4FB-4b17-A5B2-3A7E262E0EC7}
static const GUID IID_ICalculateFrame = { 0x85fbff21, 0xb4fb, 0x4b17, { 0xa5, 0xb2, 0x3a, 0x7e, 0x26, 0x2e, 0xe, 0xc7 } };

//�����ܽӿ�
interface ICalculateFrame : public IUnknownEx
{
	//ö�ټ��㵥Ԫ
	virtual ICalculateItem* __cdecl EnumCalculateItem(WORD wEnumIndex)=NULL;
	//��ȡ���㵥Ԫ��Ŀ
	virtual DWORD	__cdecl GetCalculateItemCount()=NULL;

	//��ȡ��Ͷע
	virtual bool __cdecl GetFrameTotalBetScore(DECIMAL* pTotalBetScore)=NULL;
	//��ȡ���ɲ�
	virtual bool __cdecl GetFrameTotalWinScore(DECIMAL* pTotalWinScore)=NULL;

	//�û�����
	virtual bool __cdecl OnEventUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)=NULL;
	//�û�����
	virtual bool __cdecl OnEventUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)=NULL;

	//��λ����
	virtual bool __cdecl Reset()=NULL;
	//����������Ϣ
	virtual bool __cdecl StructCalculateItem()=NULL;

	//Ͷע
	virtual bool __cdecl BetScore(IServerUserItem * pIServerUserItem,
		tagBetScoreInfo *pBetScoreInfo, WORD wCount)=NULL;
	//���Ͷע
	virtual BYTE __cdecl CheckBetScore(IServerUserItem * pIServerUserItem,
		tagBetScoreInfo *pBetScoreInfo, WORD wCount)=NULL;


	//�������Ի�Ա��ʽ
	virtual bool __cdecl CalculateCTP(void* pCalculateContext,
		ICalculateSink* pCalculateSink)=NULL;

	//�����Ա�Ի�Ա��ʽ
	virtual bool __cdecl CalculatePTP(tagBetScoreInfo *pBetScoreInfo, WORD wCount)=NULL;

};
//////////////////////////////////////////////////////////////////////////

#define VER_ITableFrame INTERFACE_VERSION(5,1)
static const GUID IID_ITableFrame={0x1aee9bf4,0x8629,0x45dc,0xb0,0xfa,0x2c,0xa7,0x4f,0xbe,0x18,0xc1};

//��Ϸ���ӽӿ�
interface ITableFrame : public IUnknownEx
{
	//��Ϣ�ӿ�
public:
	//���Ӻ���
	virtual WORD __cdecl GetTableID()=NULL;
	//��Ϸ����
	virtual WORD __cdecl GetChairCount()=NULL;
	//��λ����Ŀ
	virtual WORD __cdecl GetNullChairCount()=NULL;
	//��λ�ú���
	virtual WORD __cdecl GetNullChairID()=NULL;
	//��ͨ�������
	virtual WORD __cdecl GetGameUserCount()=NULL;
	//��ͬIP��ַλ��ID
	virtual WORD __cdecl GetSameIPChairID(IServerUserItem * pIServerUserItem)=NULL;
	//��Ϸʱ��
	virtual ULONGLONG __cdecl GetPlayTimeCount()=NULL;

	//AI��Ϣ�ӿ�
public:
	//AI�������
	virtual WORD __cdecl GetAIGameUserCount()=NULL;
	//��ȡAI��ɱ����ͨ���������
	virtual WORD __cdecl GetAIKillGameUserChairID()=NULL;
	//�Ƿ�AI��ɱ��ͨ���
	virtual bool __cdecl IsAIKillGameUser()=NULL; 

	//�û��ӿ�
public:
	//ö���û�
	virtual IServerUserItem * __cdecl EnumLookonUserItem(WORD wIndex)=NULL;
	//��ȡ�û�
	virtual IServerUserItem * __cdecl GetServerUserItem(WORD wChairID)=NULL;

	//״̬�ӿ�
public:
	//��ȡ״̬
	virtual BYTE __cdecl GetGameStatus()=NULL;
	//����״̬
	virtual void __cdecl SetGameStatus(BYTE bGameStatus)=NULL;
	//��ȡ����
	virtual const tagGameServiceAttrib * __cdecl GetGameServiceAttrib()=NULL; 
	//��ȡ����
	virtual const tagGameServiceOption * __cdecl GetGameServiceOption()=NULL;

	//��ʱ���ӿ�
public:
	//���ö�ʱ��
	virtual bool __cdecl SetGameTimer(WORD wTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM wBindParam)=NULL;
	//ɾ����ʱ��
	virtual bool __cdecl KillGameTimer(WORD wTimerID)=NULL;

	//����ӿ�
public:
	//��������
	virtual bool __cdecl SendUserData(IServerUserItem * pIServerUserItem, WORD wSubCmdID)=NULL;
	//��������
	virtual bool __cdecl SendUserData(IServerUserItem * pIServerUserItem, WORD wSubCmdID, void * pData, WORD wDataSize)=NULL;
	//��������
	virtual bool __cdecl SendTableData(WORD wChairID, WORD wSubCmdID)=NULL;
	//��������
	virtual bool __cdecl SendTableData(WORD wChairID, WORD wSubCmdID, void * pData, WORD wDataSize)=NULL;
	//��������
	virtual bool __cdecl SendLookonData(WORD wChairID, WORD wSubCmdID)=NULL;
	//��������
	virtual bool __cdecl SendLookonData(WORD wChairID, WORD wSubCmdID, void * pData, WORD wDataSize)=NULL;

	//��Ϣ�ӿ�
public:
	//���ͷ�����Ϣ
	virtual bool __cdecl SendRoomMessage(IServerUserItem * pIServerUserItem, WORD wErrCode,LPCTSTR lpszMessage, WORD wMessageType)=NULL;
	//������Ϸ��Ϣ
	virtual bool __cdecl SendGameMessage(IServerUserItem * pIServerUserItem, WORD wErrCode, LPCTSTR lpszMessage, WORD wMessageType)=NULL;

	//��Ϸ�ӿ�
public:
	//��ɢ��Ϸ
	virtual bool __cdecl DismissGame()=NULL;
	//������Ϸ
	virtual bool __cdecl ConcludeGame()=NULL;
	//����
	virtual bool __cdecl WriteGameEnd(BYTE cbReason,
		BYTE *pEndData,
		WORD wEndDataSize,
		void* pCalculateContext,
		ICalculateSink* pCalculateSink)=NULL;
	//���ͳ���
	virtual bool __cdecl SendGameScene(IServerUserItem * pIServerUserItem, void * pData, WORD wDataSize)=NULL;
	//����ӿ�
public:
	//��ȡ������
	virtual ICalculateFrame* __cdecl GetICalculateFrame()=NULL;
};

//////////////////////////////////////////////////////////////////////////

#define VER_ITableFrameManager INTERFACE_VERSION(2,1)
static const GUID IID_ITableFrameManager={0xc0994158,0xf4b4,0x462f,0xa8,0x84,0x85,0x13,0xe2,0xac,0xfe,0x31};

//��ܹ���ӿ�
interface ITableFrameManager : public ITableFrame
{
	//���ýӿ�
public:
	//��ʼ��
	virtual bool __cdecl InitTableFrame(WORD wTableID, 
		tagTableFrameParameter * pTableFrameParameter)=NULL;

	//��Ϣ�ӿ�
public:
	//����״̬
	virtual bool __cdecl IsTableLocked()=NULL;
	//��Ϸ״̬
	virtual bool __cdecl IsGameStarted()=NULL;
	//��Ϸ״̬
	virtual bool __cdecl IsUserPlaying(IServerUserItem * pIServerUserItem)=NULL;

	//ִ�нӿ�
public:
	//���¶���
	virtual bool __cdecl PerformSitDownAction(WORD wChairID, IServerUserItem * pIServerUserItem)=NULL;
	//�Թ۶���
	virtual bool __cdecl PerformLookonAction(WORD wChairID, IServerUserItem * pIServerUserItem)=NULL;
	//�뿪����
	virtual bool __cdecl PerformStandUpAction(IServerUserItem * pIServerUserItem)=NULL;
	//�˹�������Ϸ����
	virtual bool __cdecl PerformAIGameAction()=NULL;

	//�û��¼�
public:
	//�����¼�
	virtual bool __cdecl OnUserOffLine(WORD wChairID)=NULL;
	//�ؽ��¼�
	virtual bool __cdecl OnUserReConnect(WORD wChairID)=NULL;
	//�������
	virtual bool __cdecl OnUserReqOffLine(WORD wChairID)=NULL;

	//ϵͳ�¼�
public:
	//��ʱ���¼�
	virtual bool __cdecl OnEventTimer(WORD wTimerID, WPARAM wBindParam)=NULL;
	//��Ϸ�¼�
	virtual bool __cdecl OnEventSocketGame(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)=NULL;
	//����¼�
	virtual bool __cdecl OnEventSocketFrame(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)=NULL;
};

//////////////////////////////////////////////////////////////////////////

#define VER_ITableFrameSink INTERFACE_VERSION(3,1)
static const GUID IID_ITableFrameSink={0x53d985c5,0x623a,0x41df,0x89,0x76,0x27,0xb1,0x8d,0xc1,0x97,0x37};

//��Ϸ���ӻص��ӿ�
interface ITableFrameSink : public IUnknownEx
{
	//����ӿ�
public:
	//��ʼ��
	virtual bool __cdecl InitTableFrameSink(IUnknownEx * pIUnknownEx)=NULL;
	//��λ����
	virtual void __cdecl RepositTableFrameSink()=NULL;

	//��Ϣ�ӿ�
public:
	//��ʼģʽ
	virtual enStartMode __cdecl GetGameStartMode()=NULL;
	//��Ϸ״̬
	virtual bool __cdecl IsUserPlaying(WORD wChairID)=NULL;

	//��Ϸ�¼�
public:
	//��Ϸ��ʼ
	virtual bool __cdecl OnEventGameStart()=NULL;
	//��Ϸ����
	virtual bool __cdecl OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)=NULL;
	//���ͳ���
	virtual bool __cdecl SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret)=NULL;
	//�˹�������Ϸ����
	virtual bool __cdecl OnPerformAIGameAction()=NULL;

	//�¼��ӿ�
public:
	//��ʱ���¼�
	virtual bool __cdecl OnTimerMessage(WORD wTimerID, WPARAM wBindParam)=NULL;
	//��Ϸ��Ϣ
	virtual bool __cdecl OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)=NULL;
	//�����Ϣ
	virtual bool __cdecl OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)=NULL;
};

//////////////////////////////////////////////////////////////////////////

#define VER_ITableFrameEvent INTERFACE_VERSION(1,1)
static const GUID IID_ITableFrameEvent={0xb16717de,0xc243,0x4686,0xbb,0x8a,0xd0,0x37,0xb8,0xc2,0xc,0xf6};

//����¼��ӿ�
interface ITableFrameEvent : public IUnknownEx
{
	//�����¼�
public:
	//����ͬ��
	virtual bool __cdecl OnEventUserReqReady(WORD wChairID, IServerUserItem * pIServerUserItem)=NULL;
	//�������
	virtual bool __cdecl OnEventUserReqOffLine(WORD wChairID, IServerUserItem * pIServerUserItem, WORD wOffLineCount)=NULL;
	//��������
	virtual bool __cdecl OnEventUserReqReConnect(WORD wChairID, IServerUserItem * pIServerUserItem)=NULL;
	//��������
	virtual bool __cdecl OnEventUserReqSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bReqLookon)=NULL;
	//��������
	virtual bool __cdecl OnEventUserReqStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bReqLookon)=NULL;

	//�����¼�
public:
	//�û�ͬ��
	virtual bool __cdecl OnEventUserReady(WORD wChairID, IServerUserItem * pIServerUserItem)=NULL;
	//�û�����
	virtual bool __cdecl OnEventUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem)=NULL;
	//�û�����
	virtual bool __cdecl OnEventUserReConnect(WORD wChairID, IServerUserItem * pIServerUserItem)=NULL;
	//�û�����
	virtual bool __cdecl OnEventUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)=NULL;
	//�û�����
	virtual bool __cdecl OnEventUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)=NULL;
};

//////////////////////////////////////////////////////////////////////////

#define VER_IGameServiceFrame INTERFACE_VERSION(1,1)
static const GUID IID_IGameServiceFrame={0xafccfa53,0x6431,0x4b2a,0x98,0x29,0xf5,0xcf,0x15,0xdf,0xd5,0x3b};

//GameService���
interface IGameServiceFrame : public IUnknownEx
{
	//״̬�ӿ�
public:
	//�رղ�ѯ
	virtual bool __cdecl IsShallClose()=NULL;
	//˽�Ĳ�ѯ
	virtual bool __cdecl IsAllowWisper()=NULL;
	//�����ѯ
	virtual bool __cdecl IsAllowRoomChat()=NULL;
	//�����ѯ
	virtual bool __cdecl IsAllowGameChat()=NULL;
	//�����ѯ
	virtual bool __cdecl IsAllowEnterRoom()=NULL;
	//�����ѯ
	virtual bool __cdecl IsAllowEnterGame()=NULL;

	//״̬֪ͨ
public:
	//����״̬
	virtual bool __cdecl SendTableStatus(WORD wTableID)=NULL;
	//���Ͷ��
	virtual bool __cdecl SendUserScore(IServerUserItem * pIServerUserItem)=NULL;
	//����״̬
	virtual bool __cdecl SendUserStatus(IServerUserItem * pIServerUserItem)=NULL;

	//����ӿ�
public:
	//��������
	virtual bool __cdecl SendData(IServerUserItem * pIServerUserItem, WORD wMainCmdID, WORD wSubCmdID)=NULL;
	//��������
	virtual bool __cdecl SendData(IServerUserItem * pIServerUserItem, WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize)=NULL;

	//��ʱ���ӿ�
public:
	//���ö�ʱ��
	virtual bool __cdecl SetTableTimer(WORD wTableID, WORD wTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM wBindParam)=NULL; 
	//ɾ����ʱ��
	virtual bool __cdecl KillTableTimer(WORD wTableID, WORD wTimerID)=NULL;

	//����ӿ�
public:
	//ɾ���û�
	virtual bool __cdecl DeleteUserItem(IServerUserItem * pIServerUserItem)=NULL;
	//����û�����
	virtual bool __cdecl CheckGameUserRefCount(IServerUserItem * pIServerUserItem)=NULL;
	//�����Ϣ
	virtual void __cdecl ExportInformation(LPCTSTR pszString, enTraceLevel TraceLevel)=NULL;
};
//////////////////////////////////////////////////////////////////////////

#define VER_IGameAIManager INTERFACE_VERSION(2,1)
static const GUID IID_IGameAIManager={0x1a6b9efa,0x3c56,0x4ee1,0x96,0xce,0x90,0xe1,0x20,0x48,0xe5,0xe7};

//��Ϸ�˹����ܹ���ӿ�
interface IGameAIManager : public IUnknownEx
{
	//�������漴����
	virtual bool __cdecl ExecAI_User_RandOperation()=NULL;
	//��Ϸ���漴����
	virtual bool __cdecl ExecAI_Table_RandOperation()=NULL;

};
//////////////////////////////////////////////////////////////////////////

#define VER_IGameServiceManager INTERFACE_VERSION(2,1)
static const GUID IID_IGameServiceManager={0xda6b9efa,0x3c56,0x4ee1,0x96,0xce,0x90,0xe1,0x20,0x48,0xe5,0xe7};

//��Ϸ����������ӿ�
interface IGameServiceManager : public IUnknownEx
{
	//��������
	virtual void * __cdecl CreateTableFrameSink(const IID & Guid, DWORD dwQueryVer)=NULL;
	//��ȡ����
	virtual void __cdecl GetGameServiceAttrib(tagGameServiceAttrib & GameServiceAttrib)=NULL;
	//�޸Ĳ���
	virtual bool __cdecl RectifyServiceOption(tagGameServiceOption * pGameServiceOption)=NULL;
};

//////////////////////////////////////////////////////////////////////////

#define VER_IGameService INTERFACE_VERSION(2,1)
static const GUID IID_IGameService={0x1239db23,0x4d2e,0x4f35,0xbb,0x4e,0x8c,0x4c,0x5d,0x39,0xe1,0xa0};

//GameService�ӿ�
interface IGameService : public IUnknownEx
{
	//����״̬
	virtual bool __cdecl IsService()=NULL;
	//�����¼�
	virtual bool __cdecl SetEventService(IUnknownEx * pIEventService)=NULL;
	//��ʼ����
	virtual bool __cdecl StartService(tagGameServiceParameter * pGameServiceParameter)=NULL;
	//ֹͣ����
	virtual bool __cdecl StopService()=NULL;
};

//////////////////////////////////////////////////////////////////////////

//GameService���������
class CGameServiceHelper : public CTempldateHelper<IGameService>
{
	//��������
public:
	//���캯��
	CGameServiceHelper(void) : CTempldateHelper<IGameService>(IID_IGameService,
		VER_IGameService,GAME_SERVICE_DLL_NAME,TEXT("CreateGameService")) { }
};

//////////////////////////////////////////////////////////////////////////

#endif