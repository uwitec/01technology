#ifndef CMD_GAME_HEAD_FILE
#define CMD_GAME_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//��¼���ݰ�����
#pragma pack(push)
#pragma pack(1)
#define MDM_GR_LOGON				1									//�����¼

#define SUB_GR_LOGON_ACCOUNT		1									//�ʻ���¼

#define SUB_GR_LOGON_SUCCESS		100									//��½�ɹ�
#define SUB_GR_LOGON_ERROR			101									//��½ʧ��
#define SUB_GR_LOGON_FINISH			102									//��½���

//�����ʺŵ�¼
struct CMD_GR_LogonByAccount
{
	DWORD							dwPlazaVersion;						//�㳡�汾
	DWORD							dwProcessVersion;					//���̰汾
	TCHAR							szAccounts[NAME_LEN];				//��¼�ʺ�
	TCHAR							szPassWord[PASS_LEN];				//��¼����
};


//��½�ɹ���Ϣ
struct CMD_GR_LogonSuccess
{
	DWORD							dwUserID;							//�û� I D
};

//��½ʧ��
struct CMD_GR_LogonError
{
	WORD							wErrorCode;							//�������
};

//////////////////////////////////////////////////////////////////////////
//�û����ݰ�����

#define MDM_GR_USER					2									//�û���Ϣ

#define SUB_GR_USER_SIT_REQ			1									//��������
#define SUB_GR_USER_LOOKON_REQ		2									//�Թ�����
#define SUB_GR_USER_STANDUP_REQ		3									//��������
#define SUB_GR_USER_LEFT_GAME_REQ	4									//�뿪��Ϸ

#define SUB_GR_USER_COME			100									//�û�����
#define SUB_GR_USER_STATUS			101									//�û�״̬
#define SUB_GR_USER_SCORE			102									//�û�����
#define SUB_GR_SIT_FAILED			103									//����ʧ��

#define SUB_GR_USER_RULE			200									//�û�����
#define SUB_GR_USER_CHAT			201									//������Ϣ
#define SUB_GR_USER_WISPER			202									//˽����Ϣ

#define SUB_GR_USER_INVITE			300									//������Ϣ
#define SUB_GR_USER_INVITE_REQ		301									//��������

//�û�״̬
struct CMD_GR_UserStatus
{
	DWORD							dwUserID;							//���ݿ� ID
	WORD							wTableID;							//����λ��
	WORD							wChairID;							//����λ��
	WORD							wNetDelay;							//������ʱ
	BYTE							cbUserStatus;						//�û�״̬
};

//�û�����
struct CMD_GR_UserScore
{
	DWORD							dwUserID;							//�û� I D
	tagUserScore					UserScore;							//���ö����Ϣ
};

//��������
struct CMD_GR_UserSitReq
{
	WORD							wTableID;							//����λ��
	WORD							wChairID;							//����λ��
	WORD							wNetTimelag;						//������ʱ
	BYTE							cbPassLen;							//���볤��
	TCHAR							szTablePass[PASS_LEN];				//��������
};
#define AUTO_TABLEID				1024								//�������Զ�ѡ��λ��
#define AUTO_CHAIRID				1024								//�������Զ�ѡ��λ��

//�����û� 
struct CMD_GR_UserInviteReq
{
	WORD							wTableID;							//���Ӻ���
	DWORD							dwUserID;							//�û� I D
};

//����ʧ��
struct CMD_GR_SitFailed
{
	WORD							wErrorCode;//��������
};

//����ṹ 
struct CMD_GR_UserChat
{
	WORD							wChatLength;						//��Ϣ����
	DWORD							dwSendUserID;						//�����û�
	DWORD							dwTargetUserID;						//Ŀ���û�
	TCHAR							szChatMessage[MAX_CHAT_LEN];		//������Ϣ
};

//˽��ṹ 
struct CMD_GR_Wisper
{
	WORD							wChatLength;						//��Ϣ����
	DWORD							dwSendUserID;						//�����û�
	DWORD							dwTargetUserID;						//Ŀ���û�
	TCHAR							szChatMessage[MAX_CHAT_LEN];		//������Ϣ
};

//�û�����
struct CMD_GR_UserRule
{
	bool							bPassword;							//��������
	bool							bLimitScore;						//���Ʒ���
	bool							bLimitDelay;						//������ʱ
	bool							bCheckSameIP;						//Ч���ַ
	WORD							wWinRate;							//����ʤ��
	WORD							wFleeRate;							//��������
	WORD							wNetDelay;							//������ʱ
	LONG							lMaxScore;							//��߷��� 
	LONG							lLessScore;							//��ͷ���
	TCHAR							szPassword[PASS_LEN];				//��������
};

//�����û� 
struct CMD_GR_UserInvite
{
	WORD							wTableID;							//���Ӻ���
	DWORD							dwUserID;							//�û� I D
};

//////////////////////////////////////////////////////////////////////////
//������Ϣ���ݰ�

#define MDM_GR_INFO					3									//������Ϣ

#define SUB_GR_SERVER_INFO			100									//��������
#define SUB_GR_CONFIG_FINISH		101									//�������

//��Ϸ������Ϣ
struct CMD_GR_ServerInfo
{
	WORD							wKindID;							//���� I D
	WORD							wGameGenre;							//��Ϸ����
	WORD							wTableCount;						//������Ŀ
	WORD							wChairCount;						//������Ŀ
};


//////////////////////////////////////////////////////////////////////////
//����״̬���ݰ�

#define MDM_GR_STATUS				4									//״̬��Ϣ

#define SUB_GR_TABLE_INFO			100									//������Ϣ
#define SUB_GR_TABLE_STATUS			101									//����״̬

//����״̬�ṹ
struct tagTableStatus
{
	BYTE							bTableLock;							//����״̬
	BYTE							bPlayStatus;						//��Ϸ״̬
};

//����״̬����
struct CMD_GR_TableInfo
{
	WORD							wTableCount;						//������Ŀ
	tagTableStatus					TableStatus[512];					//״̬����
};

//����״̬��Ϣ
struct CMD_GR_TableStatus
{
	WORD							wTableID;							//���Ӻ���
	BYTE							bPlayStatus;						//��Ϸ״̬
};

//////////////////////////////////////////////////////////////////////////
//�������ݰ�

#define MDM_GR_MANAGER				5									//��������

#define SUB_GR_SEND_WARNING			1									//���;���
#define SUB_GR_SEND_MESSAGE			2									//������Ϣ
#define SUB_GR_LOOK_USER_IP			3									//�鿴��ַ
#define SUB_GR_KILL_USER			4									//�߳��û�
#define SUB_GR_OPTION_SERVER		5									//��������

#define SUB_GR_LOOK_USER_IP_RESULT	30									//���ز鿴��ַ


//���;���
struct CMD_GR_SendWarning
{
	WORD							wChatLength;						//��Ϣ����
	DWORD							dwTargetUserID;						//Ŀ���û�
	TCHAR							szWarningMessage[MAX_CHAT_LEN];		//������Ϣ
};

//ϵͳ��Ϣ
struct CMD_GR_SendMessage
{
	BYTE							cbGame;								//��Ϸ��Ϣ
	BYTE							cbRoom;								//��Ϸ��Ϣ
	WORD							wChatLength;						//��Ϣ����
	TCHAR							szSystemMessage[MAX_CHAT_LEN];		//ϵͳ��Ϣ
};

//�鿴��ַ
struct CMD_GR_LookUserIP
{
	DWORD							dwTargetUserID;						//Ŀ���û�
};
//�鿴��ַ���
struct CMD_GR_LookUserIPResult
{
	DWORD							dwTargetUserID;						//Ŀ���û�
	TCHAR							szAccount[NAME_LEN];				//�ʺ�
	TCHAR							szName[NAME_LEN];					//����
	DWORD							dwUserIP;							//IP
	TCHAR							szIPDescribe[256];					//IP����
};

//�߳��û�
struct CMD_GR_KillUser
{
	DWORD							dwTargetUserID;						//Ŀ���û�
};



//���ñ�־
#define OSF_ROOM_CHAT				1									//��������
#define OSF_GAME_CHAT				2									//��Ϸ����
#define OSF_ROOM_WISPER				3									//����˽��
#define OSF_ENTER_GAME				4									//������Ϸ
#define OSF_ENTER_ROOM				5									//���뷿��
#define OSF_SHALL_CLOSE				6									//�����ر�

//��������
struct CMD_GR_OptionServer
{
	BYTE							cbOptionFlags;						//���ñ�־
	BYTE							cbOptionValue;						//���ñ�־
};

//////////////////////////////////////////////////////////////////////////
//ϵͳ���ݰ�

#define MDM_GR_SYSTEM				10									//ϵͳ��Ϣ

#define SUB_GR_MESSAGE				100									//ϵͳ��Ϣ

//��Ϣ����
#define SMT_INFO					0x0001								//��Ϣ��Ϣ
#define SMT_EJECT					0x0002								//������Ϣ
#define SMT_GLOBAL					0x0004								//ȫ����Ϣ
#define SMT_CLOSE_ROOM				0x1000								//�رշ���
#define SMT_INTERMIT_LINE			0x4000								//�ж�����

//��Ϣ���ݰ�
struct CMD_GR_Message
{
	WORD							wMessageType;						//��Ϣ����
	WORD							wErrorCode;							//������Ϣ����
	WORD							wMessageLength;						//��Ϣ����
	TCHAR							szContent[1024];					//��Ϣ����
};

//////////////////////////////////////////////////////////////////////////
//�������ݰ�

#define MDM_GR_SERVER_INFO			11									//������Ϣ

#define SUB_GR_ONLINE_COUNT_INFO	100									//������Ϣ

//������Ϣ
struct tagGameServerOnLineCountInfo
{
	WORD							wServerID;							//���ͱ�ʶ
	DWORD							dwOnLineCount;						//��������
};

//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)

#endif