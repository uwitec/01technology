#pragma	  once
#pragma pack(push)
#pragma pack(1)

//������¼����

//����
static const DWORD	OPT_All					= 0x00000000;	//ȫ��
////////��½
static const DWORD	OPT_Logon					= 0x00000001;	
struct  tagOperationData_Logon
{
};

//�����
///////���������ʺ�
static const DWORD	OPT_NewAccount			= 0x00000002;	
struct  tagOperationData_NewAccount
{
	BYTE		cbLevel;						//�ȼ�
	DOUBLE		fOccupancyRate;					//ռ�ɱ�
	BYTE		cbOccupancyRate_NoFlag;			//��ռ�ɱȱ�־
	DOUBLE		fTaxOccupancyRate;			//��ˮռ�ɱ�
	DOUBLE		fRollbackRate;					//ϴ���
	DWORD		dwBetLimit;						//������
};

///////�������ʺ�
static const DWORD	OPT_NewSubAccount			= 0x00000004;	
struct  tagOperationData_NewSubAccount
{
	TCHAR		szName[NAME_LEN];				//����
	TCHAR		szParentAccount[NAME_LEN];		//���ʺ�
};

///////�趨���뼰����
static const DWORD  OPT_SetBasicInfo			= 0x00000008;	
struct  tagOperationData_SetBasicInfo
{
	TCHAR		szName[NAME_LEN];				//����
};

///////��ȡ���
static const DWORD  OPT_SetScore				= 0x00000010;	
struct  tagOperationData_SetScore
{
	DOUBLE		fScoreResult;					//���
	DOUBLE		fSetScoreOffset;				//��Ȳ�
};

///////�趨״̬
static const DWORD  OPT_SetCongealState		= 0x00000020;	
struct  tagOperationData_SetCongealState
{
	BYTE		cbState;						//0-���� 1-ͣ��	
	DWORD		dwStateMark;					//����
};

///////�趨ռ�ɱ�
static const DWORD	OPT_SetOccupancyRate		= 0x00000040;
struct  tagOperationData_SetOccupancyRate
{
	DOUBLE		fOccupancyRate;						//ռ�ɱ�
	BYTE		cbOccupancyRate_NoFlag;				//��ռ�ɱȱ�־
	DOUBLE		fLowOccupancyRate_Max;				//�������ռ�ɱ�
	BYTE		cbLowOccupancyRate_Max_NoFlag;		//������߲�ռ�ɱȱ�־
	DOUBLE		fLowOccupancyRate_Min;				//�������ռ�ɱ�
	BYTE		cbLowOccupancyRate_Min_NoFlag;		//������Ͳ�ռ�ɱȱ�־
	DOUBLE		fTaxOccupancyRate;				//��ˮռ�ɱ�

	DOUBLE		fOldOccupancyRate;						//ռ�ɱ�
	BYTE		cbOldOccupancyRate_NoFlag;				//��ռ�ɱȱ�־
	DOUBLE		fOldLowOccupancyRate_Max;				//�������ռ�ɱ�
	BYTE		cbOldLowOccupancyRate_Max_NoFlag;		//������߲�ռ�ɱȱ�־
	DOUBLE		fOldLowOccupancyRate_Min;				//�������ռ�ɱ�
	BYTE		cbOldLowOccupancyRate_Min_NoFlag;		//������Ͳ�ռ�ɱȱ�־
	DOUBLE		fOldTaxOccupancyRate;				//��ˮռ�ɱ�
};

//////�趨ϴ���
static const DWORD  OPT_SetRollbackRate		= 0x00000080;
struct  tagOperationData_SetRollbackRate
{
	DOUBLE		fRollbackRate;					//ϴ���
	DOUBLE		fOldRollbackRate;				//ԭϴ���
};

///////�趨Ȩ��
static const DWORD	OPT_SetUserRight			= 0x00000100;	
struct  tagOperationData_SetUserRight
{
	DWORD		dwUserRight;					//��ϷȨ��
	DWORD		dwMasterRight;					//����Ȩ��
	DWORD		dwOldUserRight;					//����ϷȨ��
	DWORD		dwOldMasterRight;				//�ɹ���Ȩ��
};
static const DWORD	OPT_KickUser				= 0x00000200;	//�����
struct  tagOperationData_KickUser
{
	TCHAR		szTypeName[TYPE_LEN];			//��������
	TCHAR		szKindName[KIND_LEN];			//��Ϸ����
	TCHAR		szServerName[SERVER_LEN];		//��������
	WORD		wTableID;						//��ID
};
//--------------------------------------------------------------------------------------------
//�ͻ�����
static const DWORD	OPT_Notice				= 0x00010000;	//��������
static const DWORD	OPT_CancelBetHistory		= 0x00020000;	//ȡ����Ϸ��¼
static const DWORD	OPT_RestoreBetHistory		= 0x00040000;	//�ָ���Ϸ��¼
static const DWORD	OPT_SetBetLimit			= 0x00080000;	//�趨��Ϸ������
static const DWORD	OPT_SetTax				= 0x00100000;	//�趨��ˮ��
static const DWORD	OPT_SetRevenueSharing		= 0x00200000;	//�趨�޺�
static const DWORD	OPT_DismissGame				= 0x00400000;	//��ɢ��Ϸ

//������¼����ͷ
struct tagOperationHeader
{
	DWORD			dwUserID;							//�������û�ID
	BYTE			cbUserType;							//�������û�����
	TCHAR			szAccount[NAME_LEN];				//�������ʺ�
	DWORD			dwOperationUserID;					//�����û�ID
	BYTE			cbOperationUserType;				//�����û�����
	TCHAR			szOperationAccount[NAME_LEN];		//�����ʺ�
	DWORD			dwOPType;							//��������
	DOUBLE			fTime;								//����ʱ��
	DWORD			dwIPAddress;						//������ַ
};
//������¼��������
union tagOperationContent
{
	tagOperationData_Logon				data_logon;
	tagOperationData_NewAccount			data_NewAccount;
	tagOperationData_NewSubAccount		data_NewSubAccount;
	tagOperationData_SetBasicInfo		data_SetBasicInfo;
	tagOperationData_SetScore			data_SetScore;
	tagOperationData_SetCongealState	data_SetCongealState;
	tagOperationData_SetOccupancyRate	data_SetOccupancyRate;
	tagOperationData_SetRollbackRate	data_SetRollbackRate;
	tagOperationData_SetUserRight		data_SetUserRight;
	tagOperationData_KickUser			data_KickUser;

	TCHAR								data_Nothing[128];//ȷ������С
};

//������¼����
struct tagOperationData
{
	tagOperationHeader		Header;
	tagOperationContent		Content;
};

#pragma pack(pop)