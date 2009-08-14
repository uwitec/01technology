#ifndef CMD_CENTER_HEAD_FILE
#define CMD_CENTER_HEAD_FILE
#include "../CommonDefinition\GlobalService.h"
//
//��Ϸ�б�������
//
#pragma pack(push)
#pragma pack(1)

#define MDM_CS_SERVER_LIST				1								//�б���Ϣ

#define SUB_CS_GET_SERVER_LIST			1								//�����б�

#define SUB_CS_LIST_INFO				100								//�б���Ϣ
#define SUB_CS_LIST_TYPE				101								//�����б�
#define SUB_CS_LIST_KIND				102								//�����б�
#define SUB_CS_LIST_STATION				103								//վ���б�
#define SUB_CS_LIST_SERVER				104								//�����б�
#define SUB_CS_LIST_FINISH				105								//�б����

//�б���Ϣ
struct CMD_CS_ListInfo
{
	DWORD								dwTypeCount;					//������Ŀ
	DWORD								dwKindCount;					//������Ŀ
	DWORD								dwServerCount;					//������Ŀ
	DWORD								dwStationCount;					//վ����Ŀ
};
//////////////////////////////////////////////////////////////////////////
//
//�������������
//

#define MDM_CS_SERVER_MANAGER			2								//�������

#define SUB_CS_REG_GAME_SERVER			1								//ע�᷿��
#define SUB_CS_UNREG_GAME_SERVER		2								//ע������
#define SUB_CS_SERVER_ONLINE_COUNT		3								//��������

//ע�᷿��
struct CMD_CS_RegGameServer
{
	tagGameServer						GameServer;						//������Ϣ
};

//ע������
struct CMD_CS_UnRegGameServer
{
	WORD								wKindID;						//���ƺ���
	WORD								wServerID;						//�������
};

//��������
struct CMD_CS_ServerOnLineCount
{
	WORD								wKindID;						//���ƺ���
	WORD								wServerID;						//�������
	DWORD								dwOnLineCount;					//��������
};

//////////////////////////////////////////////////////////////////////////

//����������������
//
#define MDM_CS_SERVER_REQUEST				3								//��Ϣ

#define SUB_CS_GET_LOGONSERVERINFO			1								//PlazaServer��Ϣ

#define SUB_CS_GET_LOGONSERVERINFO_SUCCESS	101								//��ȡPlazaServer��Ϣ�ɹ�
#define SUB_CS_GET_LOGONSERVERINFO_FAILD	102								//��ȡPlazaServer��Ϣʧ��

//�������ݿ���Ϣ
struct CMD_CS_GetLogonServerInfo
{
	TCHAR							szLogonServerPass[256];				//������������
};

//���ݿ���Ϣ
struct CMD_CS_LogonServerInfo
{
	WORD							wDataBasePort;						//���ݿ�˿�
	DWORD							dwDataBaseAddr;						//���ݿ��ַ
	TCHAR							szDataBasePipeName[32];				//���ݿ��û�
	TCHAR							szDataBaseUser[32];					//���ݿ��û�
	TCHAR							szDataBasePass[32];					//���ݿ�����
	TCHAR							szMainPage[32];						//��Ϸ��վ
};

#define SUB_CS_GET_LOADERSERVERINFO			2								//�b�d��������Ϣ

#define SUB_CS_GET_LOADERSERVERINFO_SUCCESS	201								//��ȡ��������Ϣ�ɹ�
#define SUB_CS_GET_LOADERSERVERINFO_FAILD	202								//��ȡ��������Ϣʧ��

//�������ݿ���Ϣ
struct CMD_CS_GetLoaderServerInfo
{
	WORD							wServerID;								//����ID
	TCHAR							szLoaderServerPass[256];				//������������
};


//��������ṹ
struct CMD_CS_LoaderServerInfo
{
	TCHAR								szModuleName[MODULE_LEN];			//ģ������
	DWORD								dwCenterAddr;						//���ĵ�ַ
	tagDataBaseInfo						GameUserDBInfo;						//��������
	tagDataBaseInfo						GameScoreDBInfo;					//��������
	tagGameServiceOption				GameServiceOption;					//��������
};

//////////////////////////////////////////////////////////////////////////
//������Ϣ������
#define MDM_CS_UPDATE_INFO				4									//������Ϣ

#define SUB_CS_UPDATE_SERVERUSERDATA	1									//�����û���Ϣ
struct CMD_CS_ServerUserData
{
	DWORD								dwUserID;							//�û�ID
	DECIMAL								decOccupancyRate;					//ռ�ɱ�
	BYTE								cbOccupancyRate_NoFlag;				//��ռ�ɱ��
	DECIMAL								decTaxOccupancyRate;			//��ˮռ�ɱ�
	DECIMAL								decRollbackRate;					//ϴ���
	DWORD								dwBetLimit;							//��Ϸ������
};
//////////////////////////////////////////////////////////////////////////
//������Ϣ
#define MDM_CS_MANAGEMENT				5									//������Ϣ

//�����û���Ϣ
#define SUB_CS_USER_MESSAGE				1									//�û���Ϣ
//��Ϣ���ݰ�
struct SUB_CS_UserMessage
{
	DWORD							dwRealUserID;						//�û��ʺ�
	BYTE							cbUserType;							//�û�����
	WORD							wMessageType;						//��Ϣ����
	WORD							wErrorCode;							//������Ϣ����
	WORD							wMessageLength;						//��Ϣ����
	TCHAR							szContent[1024];					//��Ϣ����
};
#pragma pack(pop)

#endif


//-----------------------------------------------
//					the end
//-----------------------------------------------
