#pragma	  once
#pragma pack(push)
#pragma pack(1)

//操作记录类型

//公有
static const DWORD	OPT_All					= 0x00000000;	//全部
////////登陆
static const DWORD	OPT_Logon					= 0x00000001;	
struct  tagOperationData_Logon
{
};

//管理端
///////新增下线帐号
static const DWORD	OPT_NewAccount			= 0x00000002;	
struct  tagOperationData_NewAccount
{
	BYTE		cbLevel;						//等级
	DOUBLE		fOccupancyRate;					//占成比
	BYTE		cbOccupancyRate_NoFlag;			//不占成比标志
	DOUBLE		fTaxOccupancyRate;			//抽水占成比
	DOUBLE		fRollbackRate;					//洗码比
	DWORD		dwBetLimit;						//上下限
};

///////新增子帐号
static const DWORD	OPT_NewSubAccount			= 0x00000004;	
struct  tagOperationData_NewSubAccount
{
	TCHAR		szName[NAME_LEN];				//名称
	TCHAR		szParentAccount[NAME_LEN];		//父帐号
};

///////设定密码及名称
static const DWORD  OPT_SetBasicInfo			= 0x00000008;	
struct  tagOperationData_SetBasicInfo
{
	TCHAR		szName[NAME_LEN];				//名称
};

///////存取额度
static const DWORD  OPT_SetScore				= 0x00000010;	
struct  tagOperationData_SetScore
{
	DOUBLE		fScoreResult;					//额度
	DOUBLE		fSetScoreOffset;				//额度差
};

///////设定状态
static const DWORD  OPT_SetCongealState		= 0x00000020;	
struct  tagOperationData_SetCongealState
{
	BYTE		cbState;						//0-启用 1-停用	
	DWORD		dwStateMark;					//掩码
};

///////设定占成比
static const DWORD	OPT_SetOccupancyRate		= 0x00000040;
struct  tagOperationData_SetOccupancyRate
{
	DOUBLE		fOccupancyRate;						//占成比
	BYTE		cbOccupancyRate_NoFlag;				//不占成比标志
	DOUBLE		fLowOccupancyRate_Max;				//下线最高占成比
	BYTE		cbLowOccupancyRate_Max_NoFlag;		//下线最高不占成比标志
	DOUBLE		fLowOccupancyRate_Min;				//下线最低占成比
	BYTE		cbLowOccupancyRate_Min_NoFlag;		//下线最低不占成比标志
	DOUBLE		fTaxOccupancyRate;				//抽水占成比

	DOUBLE		fOldOccupancyRate;						//占成比
	BYTE		cbOldOccupancyRate_NoFlag;				//不占成比标志
	DOUBLE		fOldLowOccupancyRate_Max;				//下线最高占成比
	BYTE		cbOldLowOccupancyRate_Max_NoFlag;		//下线最高不占成比标志
	DOUBLE		fOldLowOccupancyRate_Min;				//下线最低占成比
	BYTE		cbOldLowOccupancyRate_Min_NoFlag;		//下线最低不占成比标志
	DOUBLE		fOldTaxOccupancyRate;				//抽水占成比
};

//////设定洗码比
static const DWORD  OPT_SetRollbackRate		= 0x00000080;
struct  tagOperationData_SetRollbackRate
{
	DOUBLE		fRollbackRate;					//洗码比
	DOUBLE		fOldRollbackRate;				//原洗码比
};

///////设定权限
static const DWORD	OPT_SetUserRight			= 0x00000100;	
struct  tagOperationData_SetUserRight
{
	DWORD		dwUserRight;					//游戏权限
	DWORD		dwMasterRight;					//管理权限
	DWORD		dwOldUserRight;					//旧游戏权限
	DWORD		dwOldMasterRight;				//旧管理权限
};
static const DWORD	OPT_KickUser				= 0x00000200;	//踢玩家
struct  tagOperationData_KickUser
{
	TCHAR		szTypeName[TYPE_LEN];			//种类名字
	TCHAR		szKindName[KIND_LEN];			//游戏名字
	TCHAR		szServerName[SERVER_LEN];		//房间名称
	WORD		wTableID;						//桌ID
};
//--------------------------------------------------------------------------------------------
//客户中心
static const DWORD	OPT_Notice				= 0x00010000;	//发布公告
static const DWORD	OPT_CancelBetHistory		= 0x00020000;	//取消游戏记录
static const DWORD	OPT_RestoreBetHistory		= 0x00040000;	//恢复游戏记录
static const DWORD	OPT_SetBetLimit			= 0x00080000;	//设定游戏上下限
static const DWORD	OPT_SetTax				= 0x00100000;	//设定抽水率
static const DWORD	OPT_SetRevenueSharing		= 0x00200000;	//设定限红
static const DWORD	OPT_DismissGame				= 0x00400000;	//解散游戏

//操作记录数据头
struct tagOperationHeader
{
	DWORD			dwUserID;							//被操作用户ID
	BYTE			cbUserType;							//被操作用户类型
	TCHAR			szAccount[NAME_LEN];				//被操作帐号
	DWORD			dwOperationUserID;					//操作用户ID
	BYTE			cbOperationUserType;				//操作用户类型
	TCHAR			szOperationAccount[NAME_LEN];		//操作帐号
	DWORD			dwOPType;							//操作类型
	DOUBLE			fTime;								//操作时间
	DWORD			dwIPAddress;						//操作地址
};
//操作记录数据描述
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

	TCHAR								data_Nothing[128];//确保包大小
};

//操作记录数据
struct tagOperationData
{
	tagOperationHeader		Header;
	tagOperationContent		Content;
};

#pragma pack(pop)