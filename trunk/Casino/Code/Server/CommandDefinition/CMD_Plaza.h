#ifndef CMD_PLAZA_HEAD_FILE
#define CMD_PLAZA_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
#pragma pack(push)
#pragma pack(1)
//�㳡�汾
#define VER_PLAZA_LOW					1								//�㳡�汾
#define VER_PLAZA_HIGH					1								//�㳡�汾
#define VER_PLAZA_FRAME					MAKELONG(VER_PLAZA_LOW,VER_PLAZA_HIGH)


struct CMD_GP_Error
{
	WORD								wErrorCode;						//�������
};

//////////////////////////////////////////////////////////////////////////
//��¼������

#define MDM_GP_LOGON					1								//�㳡��¼

#define SUB_GP_LOGON_ACCOUNT			1								//�ʺŵ�¼

#define SUB_GP_LOGON_SUCCESS			100								//��½�ɹ�
#define SUB_GP_LOGON_ERROR				101								//��½ʧ��
#define SUB_GP_LOGON_GAMEINFO			102								//��Ϸ��Ϣ
#define SUB_GP_LOGON_NOTICE				103								//����
#define SUB_GP_LOGON_FINISH				104								//��½���

//�ʺŵ�¼
struct CMD_GP_LogonByAccount
{
	DWORD								dwPlazaVersion;					//�㳡�汾
	TCHAR								szAccounts[NAME_LEN];			//��¼�ʺ�
	TCHAR								szPassWord[PASS_LEN];			//��¼����
	TCHAR								szRealityPass[PASS_LEN];		//��ʵ����
	BYTE								cbClientType;					//�ͻ�������
};

//��½�ɹ�
struct CMD_GP_LogonSuccess
{
	tagUserData							UserData;							//�û�����
	BYTE								cbUserType;							//�û�����
	DWORD								dwRealUserID;						//�û�ID
};

//��½ʧ��
typedef CMD_GP_Error CMD_GP_LogonError;

//��Ϸ��Ϣ
struct CMD_GP_GameInfo
{
	TCHAR szMainWebPageAddress[128];									//��Ϸ��ҳ��ַ
	TCHAR szFieldLiveVideoAddress[64];									//�ֳ���ַ
	TCHAR szVideoTapeAddress[64];										//��Ƶ¼���ַ

};

//����
struct CMD_GP_Notice
{
	TCHAR szNotice[512];												//��������
};
//////////////////////////////////////////////////////////////////////////
//��Ϸ�б�������
#define MDM_GP_SERVER_LIST				2								//�б�����

#define SUB_GP_LIST_TYPE				100								//�����б�
#define SUB_GP_LIST_KIND				101								//�����б�
#define SUB_GP_LIST_STATION				102								//վ���б�
#define SUB_GP_LIST_SERVER				103								//�����б�
#define SUB_GP_LIST_FINISH				104								//�������
#define SUB_GP_LIST_CONFIG				105								//�б�����

#define SUB_GP_LIST_PROCESS				106								//�����б�

//�б�����
struct CMD_GP_ListConfig
{
	BYTE								bShowOnLineCount;				//��ʾ����
};

//////////////////////////////////////////////////////////////////////////
//ϵͳ������
#define MDM_GP_SYSTEM					3								//ϵͳ����

#define SUB_GP_VERSION					100								//�汾֪ͨ
#define SUB_SP_SYSTEM_MSG				101								//ϵͳ��Ϣ

//�汾֪ͨ
struct CMD_GP_Version
{
	BYTE								bNewVersion;					//���°汾
	BYTE								bAllowConnect;					//��������
};

//////////////////////////////////////////////////////////////////////////
//�༭�ʺ�����
#define MDM_GP_EDIT_USERINFO			4								//�༭�ʺ�
//-------------ע���ʺ�
#define SUB_GP_REGISTER_USER			1								//ע���ʺ�

#define SUB_GP_REGISTER_SUCCESS			101								//ע��ɹ�
#define SUB_GP_REGISTER_ERROR			102								//ע��ʧ��

//ע���ʺ�
struct CMD_GP_RegisterUser
{
	DWORD								dwOperationUserID;					//�����û�ID
	tagUserData							UserData;
	TCHAR								szPassWord[PASS_LEN];				//��¼����
};
//ע���ʺųɹ�
struct CMD_GP_RegisterSuccess
{
	tagUserData							UserData;
};
//ע���ʺ�ʧ��
typedef CMD_GP_Error		CMD_GP_RegisterError;


//-----------״̬�趨
#define SUB_GP_SET_USERCONGEALSTATE		2									//״̬�趨ͣ������

#define SUB_GP_SET_USERCONGEALSTATE_SUCCESS		201							//״̬�趨�ɹ�
#define SUB_GP_SET_USERCONGEALSTATE_ERROR		202							//״̬�趨ʧ��

//�ʺ�ͣ������״̬
struct CMD_GP_SetUserCongealState
{
	DWORD								dwOperationUserID;					//�����û�ID
	DWORD								dwUserID;							//�������û�ID
	BYTE								cbLevel;							//�������û��ȼ�
	BYTE								cbState;							//0-���� 1-ͣ��	
};
//״̬�趨�ɹ�
struct CMD_GP_SetUserCongealStateSuccess 
{
	DWORD								dwUserID;							//�������û�ID
	BYTE								cbState;							//0-���� 1-ͣ��	
	DWORD								dwStateMark;						//����
};
typedef CMD_GP_Error	CMD_GP_SetUserCongealStateError;

//---------��ȴ�ȡ
#define SUB_GP_SET_SETUSERSCORE				3
#define SUB_GP_SET_SETUSERSCORE_SUCCESS		301							//��ȴ�ȡ�ɹ�
#define SUB_GP_SET_SETUSERSCORE_ERROR		302							//��ȴ�ȡʧ��

//��ȴ�ȡ
struct CMD_GP_SetUserScore
{
	DWORD								dwOperationUserID;					//�����û�ID
	DWORD								dwParentUserID;						//����ȡ�û�����ID
	DWORD								dwUserID;							//����ȡ�û�ID
	DOUBLE								fScore;								//���
};
//��ȴ�ȡ�ɹ�
struct CMD_GP_SetUserScoreSuccess
{
	DWORD								dwParentUserID;						//����ȡ�û�����ID
	DWORD								dwUserID;							//����ȡ�û�ID
	DOUBLE								fScore;								//���
	DOUBLE								fScoreOffset;						//��Ȳ�
};
typedef CMD_GP_Error	CMD_GP_SetUserScoreError;

//---------���������޸�
#define SUB_GP_SET_BASICUSERINFO				4
#define SUB_GP_SET_BASICUSERINFO_SUCCESS		401							//���������޸ĳɹ�
#define SUB_GP_SET_BASICUSERINFO_ERROR		402							//���������޸�ʧ��

//��������
struct CMD_GP_BasicUserInfo
{
	DWORD								dwOperationUserID;					//�����û�ID
	DWORD								dwUserID;							//�������û�ID
	TCHAR								szName[NAME_LEN];					//����
	TCHAR								szPassWord[PASS_LEN];				//��¼����
	BYTE								cbFaceID;							//ͷ��ID
};
//�޸Ļ������ϳɹ�
struct CMD_GP_SetBasicUserInfoSuccess
{
	DWORD								dwUserID;							//�������û�ID
	TCHAR								szName[NAME_LEN];					//����
	BYTE								cbFaceID;							//ͷ��ID
};
typedef CMD_GP_Error			CMD_GP_SetBasicUserInfoError;


//---------��ϸ�����޸�
#define SUB_GP_SET_DETAILUSERINFO			5
#define SUB_GP_SET_DETAILUSERINFO_SUCCESS	501							//��ϸ�����޸ĳɹ�
#define SUB_GP_SET_DETAILUSERINFO_ERROR		502							//��ϸ�����޸�ʧ��

//��ϸ����
struct CMD_GP_DetailUserInfo
{
	DWORD								dwOperationUserID;					//�����û�ID
	DWORD								dwUserID;							//�������û�ID
	DOUBLE								fOccupancyRate;						//ռ�ɱ�
	BYTE								cbOccupancyRate_NoFlag;				//��ռ�ɱȱ�־
	DOUBLE								fLowOccupancyRate_Max;				//�������ռ�ɱ�
	BYTE								cbLowOccupancyRate_Max_NoFlag;		//������߲�ռ�ɱȱ�־
	DOUBLE								fLowOccupancyRate_Min;				//�������ռ�ɱ�
	BYTE								cbLowOccupancyRate_Min_NoFlag;		//������Ͳ�ռ�ɱȱ�־
	DOUBLE								fTaxOccupancyRate;				//��ˮռ�ɱ�
	DOUBLE								fRollbackRate;						//ϴ���
	DWORD								dwBetLimit;							//������
};
//�޸���ϸ���ϳɹ�
struct CMD_GP_SetDetailUserInfoSuccess
{
	DWORD								dwUserID;							//�������û�ID
	DOUBLE								fOccupancyRate;						//ռ�ɱ�
	BYTE								cbOccupancyRate_NoFlag;				//��ռ�ɱȱ�־
	DOUBLE								fLowOccupancyRate_Max;				//�������ռ�ɱ�
	BYTE								cbLowOccupancyRate_Max_NoFlag;		//������߲�ռ�ɱȱ�־
	DOUBLE								fLowOccupancyRate_Min;				//�������ռ�ɱ�
	BYTE								cbLowOccupancyRate_Min_NoFlag;		//������Ͳ�ռ�ɱȱ�־
	DOUBLE								fTaxOccupancyRate;				//��ˮռ�ɱ�
	DOUBLE								fRollbackRate;						//ϴ���
	DWORD								dwBetLimit;							//������
};
typedef CMD_GP_Error					CMD_GP_SetDetailUserInfoError;

//-----------Ȩ���趨
#define SUB_GP_SET_USERRIGHT				6								//Ȩ���趨

#define SUB_GP_SET_USERRIGHT_SUCCESS		601								//Ȩ���趨�ɹ�
#define SUB_GP_SET_USERRIGHT_ERROR			602								//Ȩ���趨ʧ��

//�ʺ�Ȩ��
struct CMD_GP_SetUserRight
{
	DWORD								dwOperationUserID;					//�����û�ID
	DWORD								dwUserID;							//�������û�ID
	DWORD								dwUserRight;						//��ϷȨ��
	DWORD								dwMasterRight;						//����Ȩ��
};
//�ʺ�Ȩ��
struct CMD_GP_SetUserRightSuccess
{
	DWORD								dwUserID;							//�������û�ID
	DWORD								dwUserRight;						//��ϷȨ��
	DWORD								dwMasterRight;						//����Ȩ��
};
typedef CMD_GP_Error	CMD_GP_SetUserRightError;

//---------������������޸�
#define SUB_GP_SET_MYSELFBASICUSERINFO				7
#define SUB_GP_SET_MYSELFBASICUSERINFO_SUCCESS		701							//���������޸ĳɹ�
#define SUB_GP_SET_MYSELFBASICUSERINFO_ERROR		702						//���������޸�ʧ��

//��������
struct CMD_GP_MySelfBasicUserInfo
{
	DWORD								dwOperationUserID;					//�����û�ID
	DWORD								dwUserID;							//�������û�ID
	TCHAR								szName[NAME_LEN];					//����
	TCHAR								szPassWord[PASS_LEN];				//��¼����
	TCHAR								szNewPassWord[PASS_LEN];			//��¼����
	TCHAR								szNewRealityPassWord[PASS_LEN];		//��¼����
	BYTE								cbFaceID;							//ͷ��ID
};
//�޸Ļ������ϳɹ�
struct CMD_GP_SetMySelfBasicUserInfoSuccess
{
	DWORD								dwUserID;							//�������û�ID
	TCHAR								szName[NAME_LEN];					//����
	TCHAR								szNewRealityPassWord[PASS_LEN];		//��¼����
	BYTE								cbFaceID;							//ͷ��ID
};
typedef CMD_GP_Error			CMD_GP_SetMySelfBasicUserInfoError;

//////////////////////////////////////////////////////////////////////////
//�༭���ʺ�����
#define MDM_GP_EDIT_SUBUSERINFO					5								//�༭���ʺ�
//-------------ע�����ʺ�
#define SUB_GP_REGISTER_SUBUSER					1								//ע�����ʺ�

#define SUB_GP_REGISTER_SUBUSER_SUCCESS			101								//ע��ɹ�
#define SUB_GP_REGISTER_SUBUSER_ERROR			102								//ע��ʧ��

//ע�����ʺ�
struct CMD_GP_RegisterSubUser
{
	DWORD								dwOperationUserID;					//�����û�ID
	DWORD								dwParentUserID;						//�����ʺ�ID
	TCHAR								szAccount[NAME_LEN];				//�ʺ�
	TCHAR								szName[NAME_LEN];					//����
	TCHAR								szPassWord[PASS_LEN];				//��¼����
};
//ע���ʺųɹ�
struct CMD_GP_RegisterSubUserSuccess
{
	tagSubUserData						UserData;
};
//ע���ʺ�ʧ��
typedef CMD_GP_Error		CMD_GP_RegisterSubUserError;


//-----------״̬�趨
#define SUB_GP_SET_SUBUSERCONGEALSTATE				2							//״̬�趨ͣ������

#define SUB_GP_SET_SUBUSERCONGEALSTATE_SUCCESS		201							//״̬�趨�ɹ�
#define SUB_GP_SET_SUBUSERCONGEALSTATE_ERROR		202							//״̬�趨ʧ��

//�ʺ�ͣ������״̬
struct CMD_GP_SetSubUserCongealState
{
	DWORD								dwOperationUserID;					//�����û�ID
	DWORD								dwUserID;							//�������û�ID
	BYTE								cbState;							//0-���� 1-ͣ��	
};
//״̬�趨�ɹ�
struct CMD_GP_SetSubUserCongealStateSuccess 
{
	DWORD								dwUserID;							//�������û�ID
	DWORD								dwParentUserID;						//�������û�����ID
	BYTE								cbState;							//0-���� 1-ͣ��	
	DWORD								dwStateMark;						//����
};
typedef CMD_GP_Error	CMD_GP_SetSubUserCongealStateError;

//---------���������޸�
#define SUB_GP_SET_BASICSUBUSERINFO					3
#define SUB_GP_SET_BASICSUBUSERINFO_SUCCESS			301							//���������޸ĳɹ�
#define SUB_GP_SET_BASICSUBUSERINFO_ERROR			302							//���������޸�ʧ��

//��������
struct CMD_GP_BasicSubUserInfo
{
	DWORD								dwOperationUserID;					//�����û�ID
	DWORD								dwUserID;							//�������û�ID
	TCHAR								szName[NAME_LEN];					//����
	TCHAR								szPassWord[PASS_LEN];				//��¼����
};
//�޸Ļ������ϳɹ�
struct CMD_GP_SetBasicSubUserInfoSuccess
{
	DWORD								dwUserID;							//�������û�ID
	DWORD								dwParentUserID;						//�������û�����ID
	TCHAR								szName[NAME_LEN];					//����
};
typedef CMD_GP_Error			CMD_GP_SetBasicSubUserInfoError;


//////////////////////////////////////////////////////////////////////////
//�ʺŻ�ȡ
#define MDM_GP_GET_USER					6							//��ȡ�ʺ�
//-------------��ȡ�ʺ�
#define SUB_GP_GET_LOWUSERLIST			1							//��ȡ�����ʺ�
						
#define SUB_GP_LIST_LOWUSER				101							//�ʺ�
#define SUB_GP_LIST_LOWUSERERROR		102							//��ȡ�ʺ�ʧ��
#define SUB_GP_LIST_SUBUSER				103							//���ʺ�

//�ʺŻ�ȡ
struct CMD_GP_GetLessUserList
{
	DWORD								dwParentUserID;			//�����ʺ�ID
};
//�ʺ��б�
struct CMD_GP_LessUserList
{
	DWORD								dwParentUserID;			//�����ʺ�ID
	WORD								wUserCount;				//�ʺ�����
	//tagUserData						UserData[20];
};
//���ʺ��б�
struct CMD_GP_SubUserList
{
	DWORD								dwParentUserID;			//�����ʺ�ID
	WORD								wSubUserCount;			//���ʺ�����
	//tagSubUserData					UserData[20];
};
typedef CMD_GP_Error		CMD_GP_GetLessUserListError;

/////////////////////////////////////////////////////////////////////////////////
//��¼��ȡ
#define MDM_GP_GET_RECORD				7						//��ȡ��¼
//-----------------��ȡ������¼
#define SUB_GP_GET_OPERATION			1						//��ȡ������¼

#define SUB_GP_LIST_OPERATIONCONFIG		101						//������¼�б�����
#define SUB_GP_LIST_OPERATION			102						//������¼
#define SUB_GP_LIST_OPERATIONFINISH		103						//�������
#define SUB_GP_LIST_OPERATIONERROR		104						//����ʧ��

struct CMD_GP_GetOperation 
{
	DWORD								dwOperationUserID;		//�����û�ID
	WORD								wPageIndex;				//ҳ����
	WORD								wPageSize;				//ҳ��С
	TCHAR								szAccount[NAME_LEN];	//�ʺ�
	BYTE								cbUserType;				//�û�����
	DWORD								dwOPType;				//��������
	DOUBLE								fBeginTime;				//������ʼʱ��
	DOUBLE								fEndTime;				//��������ʱ��
};
struct CMD_GP_OperationListConfig
{
	WORD								wPageIndex;				//ҳ����
	WORD								wPageSize;				//ҳ��С
	WORD								wResultCount;			//���ؼ�¼��Ŀ
	DWORD								dwTotalResultCount;		//��¼����
};
struct CMD_GP_OperationList
{
	//tagOperationData					OperationData[20];
};
typedef CMD_GP_Error		CMD_GP_OperationListError;
//------------------��ȡ�����¼�鿴��ʽ-1
#define SUB_GP_GET_CHARTVIEWONE				2						//��ȡ�����¼�鿴-1

#define SUB_GP_LIST_CHARTVIEWONECONFIG		201						//�����¼�б�����
#define SUB_GP_LIST_CHARTVIEWONE			202						//�����¼
#define SUB_GP_LIST_CHARTVIEWONEFINISH		203						//�������
#define SUB_GP_LIST_CHARTVIEWONEERROR		204						//����ʧ��

struct CMD_GP_GetChartViewOne 
{
	DWORD								dwOperationUserID;		//�����û�ID
	BYTE								cbParentFlag;			//��ѯ���߱��
	TCHAR								szAccount[NAME_LEN];	//�ʺ�
	DOUBLE								fBeginTime;				//��ʼʱ��
	DOUBLE								fEndTime;				//����ʱ��
	WORD								wGameType;				//��Ϸ����
	WORD								wGameKind;				//��Ϸ����
	BYTE								cbSortType;				//��������
	BYTE								cbGameRoundType;		//������Ч��
};
struct CMD_GP_ChartViewOneListConfig
{
	TCHAR								szAccount[NAME_LEN];	//�ʺ�
	BYTE								cbLevel;				//�û�����
	DWORD								dwTotalResultCount;		//��¼����
};
struct CMD_GP_ChartViewOneList
{
	//tagChartViewOne					ChartViewOneData[20];
};
typedef CMD_GP_Error		CMD_GP_ChartViewOneListError;
//------------------��ȡ�����¼�鿴-2
#define SUB_GP_GET_CHARTVIEWTWO			3							//��ȡ�����¼�鿴-2

#define SUB_GP_LIST_CHARTVIEWTWOCONFIG		301						//�����¼�б�����
#define SUB_GP_LIST_CHARTVIEWTWO			302						//������¼
#define SUB_GP_LIST_CHARTVIEWTWOFINISH		303						//�������
#define SUB_GP_LIST_CHARTVIEWTWOERROR		304						//����ʧ��

struct CMD_GP_GetChartViewTwo 
{
	DWORD								dwOperationUserID;		//�����û�ID
	WORD								wPageIndex;				//ҳ����
	WORD								wPageSize;				//ҳ��С
	TCHAR								szAccount[NAME_LEN];	//�ʺ�
	DOUBLE								fBeginTime;				//��ʼʱ��
	DOUBLE								fEndTime;				//����ʱ��
	BYTE								cbLevel;				//�û�����
	WORD								wGameType;				//��Ϸ����
	WORD								wGameKind;				//��Ϸ����
	BYTE								cbSortType;				//��������
	BYTE								cbGameRoundType;		//������Ч��
};
struct CMD_GP_ChartViewTwoListConfig
{
	WORD								wPageIndex;				//ҳ����
	WORD								wPageSize;				//ҳ��С
	WORD								wResultCount;			//���ؼ�¼��Ŀ
	DWORD								dwTotalResultCount;		//��¼����
};
struct CMD_GP_ChartViewTwoList
{
	//tagChartViewTwoData					ChartViewTwoData[20];
};
typedef CMD_GP_Error		CMD_GP_ChartViewTwoListError;
//------------------��ȡ��Ϸ��¼�鿴
#define SUB_GP_GET_BETHISTORY			4						//��ȡ��Ϸ��¼�鿴
#define SUB_GP_LIST_BETHISTORYCONFIG	401						//�����¼�б�����
#define SUB_GP_LIST_BETHISTORY			402						//������¼
#define SUB_GP_LIST_BETHISTORYFINISH	403						//�������
#define SUB_GP_LIST_BETHISTORYERROR		404						//����ʧ��

struct CMD_GP_GetBetHistory 
{
	DWORD								dwOperationUserID;		//�����û�ID
	WORD								wPageIndex;				//ҳ����
	WORD								wPageSize;				//ҳ��С
	TCHAR								szAccount[NAME_LEN];	//�ʺ�
	DOUBLE								fBeginTime;				//��ʼʱ��
	DOUBLE								fEndTime;				//����ʱ��
	WORD								wGameType;				//��Ϸ����
	WORD								wGameKind;				//��Ϸ����
	BYTE								cbSortType;				//��������
	BYTE								cbGameRoundType;		//������Ч��
};
struct CMD_GP_BetHistoryListConfig
{
	WORD								wPageIndex;				//ҳ����
	WORD								wPageSize;				//ҳ��С
	WORD								wResultCount;			//���ؼ�¼��Ŀ
	DWORD								dwTotalResultCount;		//��¼����
};
struct CMD_GP_BetHistoryList
{
	//tagBetHistoryData					BetHistoryData[20];
};
typedef CMD_GP_Error		CMD_GP_BetHistoryListError;

//------------------��ȡ��Ϸ�ּ�¼��ϸͶע��Ϣ
#define SUB_GP_GET_DETAILBETSCORE			5						//��ȡ��Ϸ�ּ�¼��ϸͶע��Ϣ

#define SUB_GP_GET_DETAILBETSCORESUCCESS	501						//��ȡ��Ϸ�ּ�¼��ϸͶע��Ϣ�ɹ�
#define SUB_GP_GET_DETAILBETSCOREERROR		502						//��ȡ��Ϸ�ּ�¼��ϸͶע��Ϣ�ɹ�

struct CMD_GP_GetDetailBetScore 
{
	DWORD								dwOperationUserID;			//�����û�ID
	DOUBLE								fUCID;						//�����¼ID
};
struct CMD_GP_GetDetailBetScoreSuccess
{
	DOUBLE								fUCID;						//�����¼ID
	WORD								wDetailBetScoreSize;		//Ͷע��ϸ��С
	BYTE								cbDetailBetScore[1536];		//Ͷע��ϸ
};
typedef CMD_GP_Error		CMD_GP_GetDetailBetScoreError;
//------------------��ȡ�����¼�鿴-3
#define SUB_GP_GET_CHARTVIEWTHREE			6						//��ȡ�����¼�鿴-3

#define SUB_GP_CHARTVIEWTHREESUCCESS		601						//�����¼�б�����
#define SUB_GP_CHARTVIEWTHREEERROR			602						//����ʧ��

struct CMD_GP_GetChartViewThree 
{
	DWORD								dwOperationUserID;			//�����û�ID
	DWORD								dwUserID;					//�ʺ�ID
	DOUBLE								fBeginTime;					//��ʼʱ��
	DOUBLE								fEndTime;					//����ʱ��
	WORD								wGameType;					//��Ϸ����
	WORD								wGameKind;					//��Ϸ����
	BYTE								cbGameRoundType;			//������Ч��
};

struct CMD_GP_ChartViewThreeSucess
{
	//Ͷע
	DWORD	dwGameRoundCount;		//����
	DOUBLE	fTotalBetScore;			//Ͷע�ܶ�
	DOUBLE	fTotalWinScore;			//���ɲ�
	DOUBLE	fValidScore_Less_Rollback;	//ϴ��-����ϴ��

};
typedef CMD_GP_Error		CMD_GP_ChartViewThreeError;

/////////////////////////////////////////////////////////////////////////////////
#define MDM_GP_SERVER_INFO			8									//������Ϣ

#define SUB_GP_ONLINE_COUNT_INFO	1									//������Ϣ

//������Ϣ
struct tagOnLineCountInfo
{
	WORD							wTypeID;							//�����ʶ
	WORD							wKindID;							//���ͱ�ʶ
	DWORD							dwOnLineCount;						//��������
};
/////////////////////////////////////////////////////////////////////////////////
#pragma pack(pop)


#endif