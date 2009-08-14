#ifndef GLOBAL_SERVICE_HEAD_FILE
#define GLOBAL_SERVICE_HEAD_FILE

#pragma once
#pragma pack(push)
#pragma pack(1)
//////////////////////////////////////////////////////////////////////////

//���ݿ�����
const TCHAR szGameUserDB[]=TEXT("GServerInfo");						//�û����ݿ�
const TCHAR	szTreasureDB[]=TEXT("GServerInfo");						//��Ϸ���ݿ�
const TCHAR szServerInfoDB[]=TEXT("GServerInfo");					//�������ݿ�

//���ݿ����
const TCHAR szTableScore[]=TEXT("GameScore");							//��Ϸ��ȱ�
const TCHAR szTableAccounts[]=TEXT("GameUserInfo");						//�û��ʻ���

//////////////////////////////////////////////////////////////////////////

//���ݿ���Ϣ
struct tagDataBaseInfo
{
	WORD							wDataBasePort;						//���ݿ�˿�
	DWORD							dwDataBaseAddr;						//���ݿ��ַ
	TCHAR							szDataBasePipeName[32];				//���ݿ�ͨ��
	TCHAR							szDataBaseUser[32];					//���ݿ��û�
	TCHAR							szDataBasePass[32];					//���ݿ�����
	TCHAR							szDataBaseName[32];					//���ݿ�����
};

//////////////////////////////////////////////////////////////////////////

//GameService����
struct tagGameServiceAttrib
{
	WORD							wKindID;							//���ƺ���
	WORD							wChairCount;						//������Ŀ
	WORD							wSupporType;						//֧������
	DWORD							dwServerVersion;					//�汾��Ϣ
	DWORD							dwMaxClientVersion;					//�汾��Ϣ
	DWORD							dwLessClientVersion;				//�汾��Ϣ
	TCHAR							szDescription[128];					//ģ������
	TCHAR							szKindName[KIND_LEN];				//��������
	TCHAR							szServerModuleName[MODULE_LEN];		//ģ������
	WORD							wProcessType;						//��������
	WORD							wBetRegionCount;					//Ͷע������Ŀ
	BOOL							bCheckCellScoreBeforeStartGame;		//����ȿ�ʼ��Ϸǰ
	BOOL							bCheckSameIP;						//�����ͬIP
};

//GameService����
struct tagGameServiceOption
{
	//��������
	WORD							wTypeID;							//���ͱ�ʶ
	WORD							wKindID;							//���ͱ�ʶ
	WORD							wServerID;							//�������
	WORD							wStationID;							//վ�����
	WORD							wTableCount;						//������Ŀ
	WORD							wServerType;						//��������
	WORD							wServerPort;						//����˿�
	DWORD							dwServerAddr;						//�����ַ
	TCHAR							szGameRoomName[SERVER_LEN];			//��������

	//�������
	DWORD							dwCellScore;						//��λ���
	DWORD							dwHighScore;						//��߶��
	DWORD							dwLessScore;						//��Ͷ��

	//AI����
	WORD							wAIUserCount;						//��������Ŀ
	BYTE							cbAILevel;							//���������ܵȼ�

	//��չ����
	WORD							wMaxConnect;						//�������
	DWORD							dwUserRight;						//����Ȩ��
	tagOptionBuffer					OptionBuffer;						//���û���

	//��ˮ
	DECIMAL							decTaxRate;							//CT++
};

//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)
#endif