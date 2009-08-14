#ifndef GLOBAL_DEF_HEAD_FILE
#define GLOBAL_DEF_HEAD_FILE

#pragma once
#pragma pack(push)
#pragma pack(1)

/////////////////////////////////////////////////////////////////////////////////////////
//��������

const TCHAR	szProductName[]=TEXT("Casino");								//��Ϸ����
const TCHAR szSystemRegKey[]=TEXT("Casino");							//ע������


/////////////////////////////////////////////////////////////////////////////////////////
//��Ϸ��ʶ

//ͨ����Ϸ
#define ID_PLAZA						0									//����
//��ս
#define ID_B_HOE						1									//�����
#define ID_B_LAND						2									//������
#define ID_B_UNCOVERPIG					3									//����
#define ID_B_BACK_JACK					4									//�ڽܿ�
#define ID_B_UPGRADE					5									//������
#define ID_B_SPARROW					6							  		//�齫
#define ID_B_SHOWHAND					7									 //���
#define ID_B_THREELEAF					8									//թ��
#define ID_B_SICHUAN_LAND				9									//�Ĵ�������
//��Ƶ
#define ID_V_DRAGONTIGER				101									//����
#define ID_V_BACCARAT					102									//�ټ���
#define ID_V_SUPER6						103									//Super6
#define ID_V_SICBO						104									//����
#define ID_V_ROULETTE					105									//����
//����
#define ID_P_SLOT_COOLFRUIT 	 		201									//ˮ�����Ի�
#define ID_P_SLOT_POKER		 	 		202									//�˿����Ի�
#define ID_P_SLOT_MAHJONGBALL		 	203									//Ͳ�����Ի�
#define ID_P_SLOT_SOCCER		 		204									//�������Ի�
#define ID_P_SLOT_STAR97				205									//����97���Ի�

#define ID_P_VIDEOPOKER_7PK				221									//7PK�綯�˿�
#define ID_P_VIDEOPOKER_5PK				222									//5PK�綯�˿�
#define ID_P_VIDEOPOKER_5PK1			223									//�ٴ�����綯�˿�
#define ID_P_VIDEOPOKER_5PK2			224									//���Ƶ綯�˿�

#define ID_P_BIGGAMEMACHINE_FRUIT				241							//ˮ����ת��
#define ID_P_BIGGAMEMACHINE_LUCKYNUMBER			242							//���ִ�ת��
#define ID_P_BIGGAMEMACHINE_GUESSTRAIN			243							//��ʯ�г�
#define ID_P_BIGGAMEMACHINE_MONSTERLEGEND		244							//ʥ�޴�˵
#define ID_P_BIGGAMEMACHINE_HORSERACE 			245							//����

#define ID_P_TABLEGAME_BLACKJACK		261									//21��
#define ID_P_TABLEGAME_WAR				262									//����
#define ID_P_TABLEGAME_REDDOG			263									//�칷
#define ID_P_TABLEGAME_ROULETTE			264									//����
#define ID_P_TABLEGAME_DRAGONTIGER		265									//����
#define ID_P_TABLEGAME_BACCARAT			266									//�ټ���
#define ID_P_TABLEGAME_SICBO			267									//����

//��Ʊ
#define ID_L_MARKSIX 	 				301									//���ϲ�

struct GameProcessItem
{
	WORD	wProcessType;
	TCHAR*	szTypeName;
	TCHAR*	szModuleName;
};
//��Ϸ��
const static GameProcessItem g_GameProcessItem[] = 
{
	{ID_B_HOE,			TEXT("��ս-�����"),		TEXT("B_Hoe.dll")},
	{ID_B_LAND,			TEXT("��ս-������"),		TEXT("B_Land.dll")},
	{ID_B_UNCOVERPIG,	TEXT("��ս-����"),			TEXT("B_UncoverPig.dll")},
	{ID_B_BACK_JACK,	TEXT("��ս-�ڽܿ�"),		TEXT("B_BlackJack.dll")},
	{ID_B_UPGRADE,		TEXT("��ս-����"),			TEXT("B_UpGrade.dll")},
	{ID_B_SPARROW,		TEXT("��ս-�齫"),			TEXT("B_Sparrow.dll")},
	{ID_B_SHOWHAND,		TEXT("��ս-���"),			TEXT("B_ShowHand.dll")},
	{ID_B_THREELEAF,	TEXT("��ս-թ��"),		TEXT("B_GoldFlower.dll")},
	{ID_B_SICHUAN_LAND,	TEXT("��ս-�Ĵ�������"),	TEXT("B_SiChuan_Land.dll")},

	{ID_V_DRAGONTIGER,	TEXT("��Ƶ-����"),			TEXT("V_DragonTiger.dll")},
	{ID_V_BACCARAT,		TEXT("��Ƶ-�ټ���"),		TEXT("V_Baccarat.dll")},
	{ID_V_SUPER6,		TEXT("��Ƶ-Super6"),		TEXT("V_Super6.dll")},
	{ID_V_SICBO,		TEXT("��Ƶ-����"),			TEXT("V_Sicbo.dll")},
	{ID_V_ROULETTE,		TEXT("��Ƶ-����"),			TEXT("V_Roulette.dll")},

	{ID_P_SLOT_COOLFRUIT,	TEXT("����-ˮ�����Ի�"),		TEXT("P_Slot_CoolFruit.dll")},
	{ID_P_SLOT_POKER,		TEXT("����-�˿����Ի�"),		TEXT("P_Slot_Poker.dll")},
	{ID_P_SLOT_MAHJONGBALL, TEXT("����-Ͳ�����Ի�"),		TEXT("P_Slot_MahjongBall.dll")},
	{ID_P_SLOT_SOCCER,		TEXT("����-�������Ի�"),		TEXT("P_Slot_Soccer.dll")},
	{ID_P_SLOT_STAR97,		TEXT("����-����97���Ի�"),		TEXT("P_Slot_Star97.dll")},
	{ID_P_VIDEOPOKER_7PK,	TEXT("����-7PK�綯�˿�"),		TEXT("P_VideoPoker_7PK.dll")},
	{ID_P_VIDEOPOKER_5PK,	TEXT("����-5PK�綯�˿�"),		TEXT("P_VideoPoker_5PK.dll")},
	{ID_P_VIDEOPOKER_5PK1,	TEXT("����-�ٴ�����綯�˿�"),	TEXT("P_VideoPoker_5PK1.dll")},
	{ID_P_VIDEOPOKER_5PK2,	TEXT("����-���Ƶ綯�˿�"),		TEXT("P_VideoPoker_5PK2.dll")},
	{ID_P_BIGGAMEMACHINE_FRUIT,			TEXT("����-ˮ����ת��"),	TEXT("P_BigGameMachine_Fruit.dll")},
	{ID_P_BIGGAMEMACHINE_LUCKYNUMBER,	TEXT("����-���ִ�ת��"),	TEXT("P_BigGameMachine_LuckyNumber.dll")},
	{ID_P_BIGGAMEMACHINE_GUESSTRAIN,	TEXT("����-��ʯ�г�"),		TEXT("P_BigGameMachine_GuessTrain.dll")},
	{ID_P_BIGGAMEMACHINE_MONSTERLEGEND, TEXT("����-ʥ�޴�˵"),		TEXT("P_BigGameMachine_MonsterLegend.dll")},
	{ID_P_BIGGAMEMACHINE_HORSERACE,		TEXT("����-����"),			TEXT("P_BigGameMachine_HorseRace.dll")},
	{ID_P_TABLEGAME_BLACKJACK,	TEXT("����-�ڽܿ�"),		TEXT("P_TableGame_BlackJack.dll")},
	{ID_P_TABLEGAME_WAR,		TEXT("����-����"),			TEXT("P_TableGame_War.dll")},
	{ID_P_TABLEGAME_REDDOG,		TEXT("����-�칷"),			TEXT("P_TableGame_RedDog.dll")},
	{ID_P_TABLEGAME_ROULETTE,	TEXT("����-����"),			TEXT("P_TableGame_Roulette.dll")},
	{ID_P_TABLEGAME_DRAGONTIGER,TEXT("����-����"),			TEXT("P_TableGame_DragonTiger.dll")},
	{ID_P_TABLEGAME_BACCARAT,	TEXT("����-�ټ���"),		TEXT("P_TableGame_Baccarat.dll")},
	{ID_P_TABLEGAME_SICBO,		TEXT("����-����"),			TEXT("P_TableGame_Sicbo.dll")},

	{ID_L_MARKSIX,		  TEXT("��Ʊ-���ϲ�"),				TEXT("L_MarkSix.dll")},
};
//��Ϸ��������
const DWORD dwGameTypeMark_Battle = 0x1;
const DWORD dwGameTypeMark_Video  = 0x2;
const DWORD	dwGameTypeMark_Pokie  = 0x4;
const DWORD dwGameTypeMark_Lottery= 0x8;
//�ж϶�ս��Ϸ
inline bool IsBattleGameProcessType(WORD wProcessType)
{
	if(wProcessType >= ID_B_HOE && wProcessType <= ID_B_SICHUAN_LAND)
		return true;
	else
		return false;
}
//�ж���Ƶ��Ϸ
inline bool IsVideoGameProcessType(WORD wProcessType)
{
	if(wProcessType >= ID_V_DRAGONTIGER && wProcessType <= ID_V_ROULETTE)
		return true;
	else
		return false;
}
//�жϵ�����Ϸ
inline bool IsPokieGameProcessType(WORD wProcessType)
{
	if(wProcessType >= ID_P_SLOT_COOLFRUIT && wProcessType <= ID_P_TABLEGAME_SICBO)
		return true;
	else
		return false;
}
//�жϲ�Ʊ��Ϸ
inline bool IsLotteryGameProcessType(WORD wProcessType)
{
	if(wProcessType >= ID_L_MARKSIX && wProcessType <= ID_L_MARKSIX)
		return true;
	else
		return false;
}
//������Ϸ��������
inline const TCHAR* GetGameTypeName(WORD wProcessType)
{
	for(int i=0; i < sizeof(g_GameProcessItem) / sizeof(g_GameProcessItem[0]); i ++)
	{
		if(g_GameProcessItem[i].wProcessType == wProcessType)
			return g_GameProcessItem[i].szTypeName;
	}
	return "";
}
//������Ϸ��������
inline const TCHAR* GetGameTypeNameByModuleName(TCHAR* szModuleName)
{
	for(int i=0; i < sizeof(g_GameProcessItem) / sizeof(g_GameProcessItem[0]); i ++)
	{
		if(_tcscmp(g_GameProcessItem[i].szModuleName,szModuleName) == 0)
			return g_GameProcessItem[i].szTypeName;
	}
	return "";
}
/////////////////////////////////////////////////////////////////////////////////////////
//�궨��

#define MAX_CHAIR						8										//�����Ϸ��
#define MAX_CHAT_LEN					512										//���쳤��
#define INVALID_TABLE					((WORD)(-1))							//��Ч���Ӻ�
#define INVALID_CHAIR					((WORD)(-1))							//��Ч���Ӻ�
#define MAIN_DLG_CLASSNAME				TEXT("CasinoGamePlaza")					//����ע����

//�˿ڶ���
#define PORT_LOGON_SERVER				9000									//PlazaServer
#define PORT_CENTER_SERVER				9010									//CenterServer

//�������ݶ���
#define SOCKET_VER						68										//���ݰ��汾
#define SOCKET_PACKAGE					2048									//��������
#define SOCKET_BUFFER					(sizeof(CMD_Head)+SOCKET_PACKAGE+2*sizeof(DWORD))

/////////////////////////////////////////////////////////////////////////////////////////

//���ݰ��ṹ��Ϣ
struct CMD_Info
{
	WORD								wDataSize;								//���ݴ�С
	BYTE								cbCheckCode;							//Ч���ֶ�
	BYTE								cbMessageVer;							//�汾��ʶ
};

//���ݰ�������Ϣ
struct CMD_Command
{
	WORD								wMainCmdID;								//��������
	WORD								wSubCmdID;								//��������
};

//���ݰ����ݰ�ͷ
struct CMD_Head
{
	CMD_Info							CmdInfo;								//�����ṹ
	CMD_Command							CommandInfo;							//������Ϣ
};

//�������ݰ�����
struct CMD_Buffer
{
	CMD_Head							Head;									//���ݰ�ͷ
	BYTE								cbBuffer[SOCKET_PACKAGE];				//���ݻ���
};

//�ں�������
#define MDM_KN_COMMAND					0										//�ں�����
#define SUB_KN_DETECT_SOCKET			1										//�������

//���ṹ��Ϣ
struct CMD_KN_DetectSocket
{
	DWORD								dwSendTickCount;						//����ʱ��
	DWORD								dwRecvTickCount;						//����ʱ��
};


//////////////////////////////////////////////////////////////////////////

//���Ⱥ궨��
#define TYPE_LEN						32										//���೤��
#define KIND_LEN						32										//���ͳ���
#define STATION_LEN						32										//վ�㳤��
#define SERVER_LEN						32										//���䳤��
#define MODULE_LEN						128										//���̳���

//��Ϸ����
#define GAME_GENRE_SCORE				0x0001									//��ֵ����
#define GAME_GENRE_GOLD					0x0002									//�������
#define GAME_GENRE_MATCH				0x0004									//��������
#define GAME_GENRE_EDUCATE				0x0008									//ѵ������

//��Ϸ���ͽṹ
struct tagGameType
{
	WORD								wSortID;								//�������
	WORD								wTypeID;								//�������
	BYTE								nbImageID;								//�D��̖�a
	TCHAR								szTypeName[TYPE_LEN];					//��������
};

//��Ϸ���ƽṹ
struct tagGameKind
{
	WORD								wSortID;								//�������
	WORD								wTypeID;								//���ͺ���
	WORD								wKindID;								//���ƺ���
	DWORD								dwOnLineCount;							//������Ŀ
	DWORD								dwMaxVersion;							//���°汾
	WORD								wProcessType;							//��������
	WORD								wTableCount;							//������Ŀ
	DWORD								dwCellScore;							//��λ���
	DWORD								dwHighScore;							//��߶��
	DWORD								dwLessScore;							//��Ͷ��
	DOUBLE								fTaxRate;								//��ˮ��
	TCHAR								szKindName[KIND_LEN];					//��Ϸ����
	//AI����
	WORD								wAIUserCount;							//��������Ŀ
	BYTE								cbAILevel;								//���������ܵȼ�
};

//��Ϸ���̽ṹ
struct tagGameProcess
{
	WORD								wSortID;								//�������
	WORD								wTypeID;								//���ͺ���
	WORD								wKindID;								//���ƺ���
	WORD								wServerPort;							//����˿�
	DWORD								dwServerAddr;							//�����ַ
	DWORD								dwMaxVersion;							//���°汾
	DWORD								dwOnLineCount;							//������Ŀ
	TCHAR								szKindName[KIND_LEN];					//��Ϸ����
};

//��Ϸվ��ṹ
struct tagGameStation
{
	WORD								wSortID;								//�������
	WORD								wKindID;								//���ƺ���
	WORD								wJoinID;								//�ҽӺ���
	WORD								wStationID;								//վ�����
	TCHAR								szStationName[STATION_LEN];				//վ������
};

//��Ϸ�����б�ṹ
struct tagGameServer
{
	WORD								wSortID;								//�������
	WORD								wKindID;								//��Ϸ����
	WORD								wTypeID;								//��ͺ���
	WORD								wServerID;								//���պ���
	WORD								wStationID;								//վ�����
	WORD								wServerPort;							//����˿�
	DWORD								dwServerAddr;							//�����ַ
	DWORD								dwOnLineCount;							//��������
	WORD								wMaxConnect;							//�������
	TCHAR								szServerName[SERVER_LEN];				//��������
};
//��Ϸ���б�ṹ
struct tagGameTable 
{
	WORD								wTableID;								//������
	WORD								wKindID;								//��Ϸ����
	WORD								wTypeID;								//��ͺ���
	WORD								wTableNumber;							//������
	BYTE								nbTalbeParam[128];						//������
};
//��Ƶ��Ϸ�������ṹ
struct tagVideoGameTableParam 
{
	DWORD								dwHighScore;
	DWORD								dwCellScore;
	DOUBLE								fMaxUserRevenueSharing;
	WORD								wStateKernelServerPort;
	DWORD								dwStateKernelServerAddr;
};
//////////////////////////////////////////////////////////////////////////

//�û�״̬����
#define US_NULL							0x00									//û��״̬
#define US_FREE							0x01									//վ��״̬
#define US_SIT							0x02									//����״̬
#define US_READY						0x03									//ͬ��״̬
#define US_LOOKON						0x04									//�Թ�״̬
#define US_PLAY							0x05									//��Ϸ״̬
#define US_OFFLINE						0x06									//����״̬

//���Ⱥ궨��
#define NAME_LEN						32										//���ֳ���
#define PASS_LEN						33										//���볤��

//�û��ȼ�
#define USERLEVEL_GAMEUSER				5										//��Ա
//�û�����
#define USERTYPE_GAME					0										//��ͨ�ʺ�
#define USERTYPE_SUB					1										//���ʺ�
#define USERTYPE_DEALER					2										//Dealer�ʺ�
//�ͻ�������
#define CLIENTTYPE_GAME					0										//��Ϸ��
#define CLIENTTYPE_MAN					1										//�����
#define CLIENTTYPE_SERVICE				2										//�ͻ�����
#define CLIENTTYPE_CONTROL				3										//���ƶ�

//�û����ö����Ϣ
struct tagUserScore
{
	DOUBLE								fScore;
};

//�û�״̬��Ϣ
struct tagUserStatus
{
	WORD								wTableID;								//���Ӻ���
	WORD								wChairID;								//����λ��
	WORD								wNetDelay;								//������ʱ
	BYTE								cbUserStatus;							//�û�״̬
};

//�û�������Ϣ�ṹ
struct tagUserInfoHead
{
	//�û�����
	WORD								wFaceID;								//ͷ������
	DWORD								dwUserID;								//�û� I D
	TCHAR								szName[NAME_LEN]; 
	//�û�����ʱ��Ϣ
	tagUserScore						UserScore;								//���ö��
	tagUserStatus						UserStatus;								//�û�״̬
};

//�û���Ϣ�ṹ
struct tagUserData
{
	  DWORD								dwUserID;								//�û�ID
	  DWORD								dwParentUserID;							//�û���ID-��һ��
	  DWORD								dwHighUserID0;							//�û���ID-��˾
      DWORD								dwHighUserID1;							//�û���ID-��ɶ�
	  DWORD								dwHighUserID2;							//�û���ID-�ɶ�
	  DWORD								dwHighUserID3;							//�û���ID-�ܴ���
	  DWORD								dwHighUserID4;							//�û���ID-����
	  TCHAR								szAccount[NAME_LEN];					//�û��ʺ�
	  BYTE								cbLevel;								//�û�����
      BYTE								cbState;								//�û�״̬
	  TCHAR								szName[NAME_LEN];						//�û�����
      BYTE								cbFaceID;								//�û�ͷ��
	  BYTE								cbStateCongealFlag;						//�û�״̬ 
      DWORD								dwUserRight;							//�û���ϷȨ��
	  DWORD								dwMasterRight;							//�û�����Ȩ��
      DOUBLE							fMidScore;								//�û���ϵ���
	  DOUBLE							fLowScore;								//��Ա���
	  DOUBLE							fOccupancyRate;							//��Ӯռ�ɱ�
      BYTE								cbOccupancyRate_NoFlag;					//��Ӯ��ռ�ɱ��
	  DOUBLE							fLowOccupancyRate_Max;					//��Ӯ�������ռ�ɱ�
      BYTE								cbLowOccupancyRate_Max_NoFlag;			//��Ӯ������߲�ռ�ɱȱ��
      DOUBLE							fLowOccupancyRate_Min;					//��Ӯ�������ռ�ɱ�
      BYTE								cbLowOccupancyRate_Min_NoFlag;			//��Ӯ�������ռ�ɱ�
	  DOUBLE							fTaxOccupancyRate;					//��ˮռ�ɱ�	
	  DOUBLE							fRollbackRate;							//ϴ���
      DWORD								dwBetLimit;								//��Ϸ������
	  DOUBLE							fRegisterTime;							//ע��ʱ��
	  WORD								wLessUserCount;							//��������

	 tagUserScore						UserScore;								//���ö��
};
//���û���Ϣ�ṹ
struct tagSubUserData
{
	DWORD								dwUserID;								//�û�ID
	DWORD								dwParentUserID;							//�û���ID
	DWORD								dwHighUserID0;							//�û���ID
	DWORD								dwHighUserID1;							//�û���ID
	DWORD								dwHighUserID2;							//�û���ID
	DWORD								dwHighUserID3;							//�û���ID
	DWORD								dwHighUserID4;							//�û���ID
	TCHAR								szAccount[NAME_LEN];					//�û��ʺ�
	BYTE								cbState;								//�û�״̬
	TCHAR								szName[NAME_LEN];						//�û�����
	BYTE								cbStateCongealFlag;						//�û�״̬
	DWORD								dwUserRight;							//�û���ϷȨ��
	DWORD								dwMasterRight;							//�û�����Ȩ��
	DOUBLE								fRegisterTime;							//ע��ʱ��
};
//////////////////////////////////////////////////////////////////////////

//�������кŽṹ
struct tagClientSerial
{
	DWORD								dwComputerID[3];						//��������
};

//���û���ṹ
struct tagOptionBuffer
{
	BYTE								cbBufferLen;							//���ݳ���
	BYTE								cbOptionBuf[32];						//���û���
};

/////////////////////////////////////////////////////////////////////////////////////////
#pragma pack(pop)

#include "GlobalError.h"
#include "GlobalTrace.h"

#endif