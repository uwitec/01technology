#ifndef CMD_PLAZA_HEAD_FILE
#define CMD_PLAZA_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
#pragma pack(push)
#pragma pack(1)
//广场版本
#define VER_PLAZA_LOW					1								//广场版本
#define VER_PLAZA_HIGH					1								//广场版本
#define VER_PLAZA_FRAME					MAKELONG(VER_PLAZA_LOW,VER_PLAZA_HIGH)


struct CMD_GP_Error
{
	WORD								wErrorCode;						//错误代码
};

//////////////////////////////////////////////////////////////////////////
//登录命令码

#define MDM_GP_LOGON					1								//广场登录

#define SUB_GP_LOGON_ACCOUNT			1								//帐号登录

#define SUB_GP_LOGON_SUCCESS			100								//登陆成功
#define SUB_GP_LOGON_ERROR				101								//登陆失败
#define SUB_GP_LOGON_GAMEINFO			102								//游戏信息
#define SUB_GP_LOGON_NOTICE				103								//公告
#define SUB_GP_LOGON_FINISH				104								//登陆完成

//帐号登录
struct CMD_GP_LogonByAccount
{
	DWORD								dwPlazaVersion;					//广场版本
	TCHAR								szAccounts[NAME_LEN];			//登录帐号
	TCHAR								szPassWord[PASS_LEN];			//登录密码
	TCHAR								szRealityPass[PASS_LEN];		//真实密码
	BYTE								cbClientType;					//客户端类型
};

//登陆成功
struct CMD_GP_LogonSuccess
{
	tagUserData							UserData;							//用户数据
	BYTE								cbUserType;							//用户类型
	DWORD								dwRealUserID;						//用户ID
};

//登陆失败
typedef CMD_GP_Error CMD_GP_LogonError;

//游戏信息
struct CMD_GP_GameInfo
{
	TCHAR szMainWebPageAddress[128];									//游戏首页地址
	TCHAR szFieldLiveVideoAddress[64];									//现场地址
	TCHAR szVideoTapeAddress[64];										//视频录象地址

};

//公告
struct CMD_GP_Notice
{
	TCHAR szNotice[512];												//公告内容
};
//////////////////////////////////////////////////////////////////////////
//游戏列表命令码
#define MDM_GP_SERVER_LIST				2								//列表资料

#define SUB_GP_LIST_TYPE				100								//类型列表
#define SUB_GP_LIST_KIND				101								//种类列表
#define SUB_GP_LIST_STATION				102								//站点列表
#define SUB_GP_LIST_SERVER				103								//房间列表
#define SUB_GP_LIST_FINISH				104								//发送完成
#define SUB_GP_LIST_CONFIG				105								//列表配置

#define SUB_GP_LIST_PROCESS				106								//进程列表

//列表配置
struct CMD_GP_ListConfig
{
	BYTE								bShowOnLineCount;				//显示人数
};

//////////////////////////////////////////////////////////////////////////
//系统命令码
#define MDM_GP_SYSTEM					3								//系统命令

#define SUB_GP_VERSION					100								//版本通知
#define SUB_SP_SYSTEM_MSG				101								//系统消息

//版本通知
struct CMD_GP_Version
{
	BYTE								bNewVersion;					//更新版本
	BYTE								bAllowConnect;					//允许连接
};

//////////////////////////////////////////////////////////////////////////
//编辑帐号命令
#define MDM_GP_EDIT_USERINFO			4								//编辑帐号
//-------------注册帐号
#define SUB_GP_REGISTER_USER			1								//注册帐号

#define SUB_GP_REGISTER_SUCCESS			101								//注册成功
#define SUB_GP_REGISTER_ERROR			102								//注册失败

//注册帐号
struct CMD_GP_RegisterUser
{
	DWORD								dwOperationUserID;					//操作用户ID
	tagUserData							UserData;
	TCHAR								szPassWord[PASS_LEN];				//登录密码
};
//注册帐号成功
struct CMD_GP_RegisterSuccess
{
	tagUserData							UserData;
};
//注册帐号失败
typedef CMD_GP_Error		CMD_GP_RegisterError;


//-----------状态设定
#define SUB_GP_SET_USERCONGEALSTATE		2									//状态设定停用启用

#define SUB_GP_SET_USERCONGEALSTATE_SUCCESS		201							//状态设定成功
#define SUB_GP_SET_USERCONGEALSTATE_ERROR		202							//状态设定失败

//帐号停用启用状态
struct CMD_GP_SetUserCongealState
{
	DWORD								dwOperationUserID;					//操作用户ID
	DWORD								dwUserID;							//被操作用户ID
	BYTE								cbLevel;							//被操作用户等级
	BYTE								cbState;							//0-启用 1-停用	
};
//状态设定成功
struct CMD_GP_SetUserCongealStateSuccess 
{
	DWORD								dwUserID;							//被操作用户ID
	BYTE								cbState;							//0-启用 1-停用	
	DWORD								dwStateMark;						//掩码
};
typedef CMD_GP_Error	CMD_GP_SetUserCongealStateError;

//---------额度存取
#define SUB_GP_SET_SETUSERSCORE				3
#define SUB_GP_SET_SETUSERSCORE_SUCCESS		301							//额度存取成功
#define SUB_GP_SET_SETUSERSCORE_ERROR		302							//额度存取失败

//额度存取
struct CMD_GP_SetUserScore
{
	DWORD								dwOperationUserID;					//操作用户ID
	DWORD								dwParentUserID;						//被存取用户上线ID
	DWORD								dwUserID;							//被存取用户ID
	DOUBLE								fScore;								//额度
};
//额度存取成功
struct CMD_GP_SetUserScoreSuccess
{
	DWORD								dwParentUserID;						//被存取用户上线ID
	DWORD								dwUserID;							//被存取用户ID
	DOUBLE								fScore;								//额度
	DOUBLE								fScoreOffset;						//额度差
};
typedef CMD_GP_Error	CMD_GP_SetUserScoreError;

//---------基本资料修改
#define SUB_GP_SET_BASICUSERINFO				4
#define SUB_GP_SET_BASICUSERINFO_SUCCESS		401							//基本资料修改成功
#define SUB_GP_SET_BASICUSERINFO_ERROR		402							//基本资料修改失败

//基本资料
struct CMD_GP_BasicUserInfo
{
	DWORD								dwOperationUserID;					//操作用户ID
	DWORD								dwUserID;							//被操作用户ID
	TCHAR								szName[NAME_LEN];					//名称
	TCHAR								szPassWord[PASS_LEN];				//登录密码
	BYTE								cbFaceID;							//头像ID
};
//修改基本资料成功
struct CMD_GP_SetBasicUserInfoSuccess
{
	DWORD								dwUserID;							//被操作用户ID
	TCHAR								szName[NAME_LEN];					//名称
	BYTE								cbFaceID;							//头像ID
};
typedef CMD_GP_Error			CMD_GP_SetBasicUserInfoError;


//---------详细资料修改
#define SUB_GP_SET_DETAILUSERINFO			5
#define SUB_GP_SET_DETAILUSERINFO_SUCCESS	501							//详细资料修改成功
#define SUB_GP_SET_DETAILUSERINFO_ERROR		502							//详细资料修改失败

//详细资料
struct CMD_GP_DetailUserInfo
{
	DWORD								dwOperationUserID;					//操作用户ID
	DWORD								dwUserID;							//被操作用户ID
	DOUBLE								fOccupancyRate;						//占成比
	BYTE								cbOccupancyRate_NoFlag;				//不占成比标志
	DOUBLE								fLowOccupancyRate_Max;				//下线最高占成比
	BYTE								cbLowOccupancyRate_Max_NoFlag;		//下线最高不占成比标志
	DOUBLE								fLowOccupancyRate_Min;				//下线最低占成比
	BYTE								cbLowOccupancyRate_Min_NoFlag;		//下线最低不占成比标志
	DOUBLE								fTaxOccupancyRate;				//抽水占成比
	DOUBLE								fRollbackRate;						//洗码比
	DWORD								dwBetLimit;							//上下限
};
//修改详细资料成功
struct CMD_GP_SetDetailUserInfoSuccess
{
	DWORD								dwUserID;							//被操作用户ID
	DOUBLE								fOccupancyRate;						//占成比
	BYTE								cbOccupancyRate_NoFlag;				//不占成比标志
	DOUBLE								fLowOccupancyRate_Max;				//下线最高占成比
	BYTE								cbLowOccupancyRate_Max_NoFlag;		//下线最高不占成比标志
	DOUBLE								fLowOccupancyRate_Min;				//下线最低占成比
	BYTE								cbLowOccupancyRate_Min_NoFlag;		//下线最低不占成比标志
	DOUBLE								fTaxOccupancyRate;				//抽水占成比
	DOUBLE								fRollbackRate;						//洗码比
	DWORD								dwBetLimit;							//上下限
};
typedef CMD_GP_Error					CMD_GP_SetDetailUserInfoError;

//-----------权限设定
#define SUB_GP_SET_USERRIGHT				6								//权限设定

#define SUB_GP_SET_USERRIGHT_SUCCESS		601								//权限设定成功
#define SUB_GP_SET_USERRIGHT_ERROR			602								//权限设定失败

//帐号权限
struct CMD_GP_SetUserRight
{
	DWORD								dwOperationUserID;					//操作用户ID
	DWORD								dwUserID;							//被操作用户ID
	DWORD								dwUserRight;						//游戏权限
	DWORD								dwMasterRight;						//管理权限
};
//帐号权限
struct CMD_GP_SetUserRightSuccess
{
	DWORD								dwUserID;							//被操作用户ID
	DWORD								dwUserRight;						//游戏权限
	DWORD								dwMasterRight;						//管理权限
};
typedef CMD_GP_Error	CMD_GP_SetUserRightError;

//---------自身基本资料修改
#define SUB_GP_SET_MYSELFBASICUSERINFO				7
#define SUB_GP_SET_MYSELFBASICUSERINFO_SUCCESS		701							//基本资料修改成功
#define SUB_GP_SET_MYSELFBASICUSERINFO_ERROR		702						//基本资料修改失败

//基本资料
struct CMD_GP_MySelfBasicUserInfo
{
	DWORD								dwOperationUserID;					//操作用户ID
	DWORD								dwUserID;							//被操作用户ID
	TCHAR								szName[NAME_LEN];					//名称
	TCHAR								szPassWord[PASS_LEN];				//登录密码
	TCHAR								szNewPassWord[PASS_LEN];			//登录密码
	TCHAR								szNewRealityPassWord[PASS_LEN];		//登录密码
	BYTE								cbFaceID;							//头像ID
};
//修改基本资料成功
struct CMD_GP_SetMySelfBasicUserInfoSuccess
{
	DWORD								dwUserID;							//被操作用户ID
	TCHAR								szName[NAME_LEN];					//名称
	TCHAR								szNewRealityPassWord[PASS_LEN];		//登录密码
	BYTE								cbFaceID;							//头像ID
};
typedef CMD_GP_Error			CMD_GP_SetMySelfBasicUserInfoError;

//////////////////////////////////////////////////////////////////////////
//编辑子帐号命令
#define MDM_GP_EDIT_SUBUSERINFO					5								//编辑子帐号
//-------------注册子帐号
#define SUB_GP_REGISTER_SUBUSER					1								//注册子帐号

#define SUB_GP_REGISTER_SUBUSER_SUCCESS			101								//注册成功
#define SUB_GP_REGISTER_SUBUSER_ERROR			102								//注册失败

//注册子帐号
struct CMD_GP_RegisterSubUser
{
	DWORD								dwOperationUserID;					//操作用户ID
	DWORD								dwParentUserID;						//父亲帐号ID
	TCHAR								szAccount[NAME_LEN];				//帐号
	TCHAR								szName[NAME_LEN];					//名称
	TCHAR								szPassWord[PASS_LEN];				//登录密码
};
//注册帐号成功
struct CMD_GP_RegisterSubUserSuccess
{
	tagSubUserData						UserData;
};
//注册帐号失败
typedef CMD_GP_Error		CMD_GP_RegisterSubUserError;


//-----------状态设定
#define SUB_GP_SET_SUBUSERCONGEALSTATE				2							//状态设定停用启用

#define SUB_GP_SET_SUBUSERCONGEALSTATE_SUCCESS		201							//状态设定成功
#define SUB_GP_SET_SUBUSERCONGEALSTATE_ERROR		202							//状态设定失败

//帐号停用启用状态
struct CMD_GP_SetSubUserCongealState
{
	DWORD								dwOperationUserID;					//操作用户ID
	DWORD								dwUserID;							//被操作用户ID
	BYTE								cbState;							//0-启用 1-停用	
};
//状态设定成功
struct CMD_GP_SetSubUserCongealStateSuccess 
{
	DWORD								dwUserID;							//被操作用户ID
	DWORD								dwParentUserID;						//被操作用户父亲ID
	BYTE								cbState;							//0-启用 1-停用	
	DWORD								dwStateMark;						//掩码
};
typedef CMD_GP_Error	CMD_GP_SetSubUserCongealStateError;

//---------基本资料修改
#define SUB_GP_SET_BASICSUBUSERINFO					3
#define SUB_GP_SET_BASICSUBUSERINFO_SUCCESS			301							//基本资料修改成功
#define SUB_GP_SET_BASICSUBUSERINFO_ERROR			302							//基本资料修改失败

//基本资料
struct CMD_GP_BasicSubUserInfo
{
	DWORD								dwOperationUserID;					//操作用户ID
	DWORD								dwUserID;							//被操作用户ID
	TCHAR								szName[NAME_LEN];					//名称
	TCHAR								szPassWord[PASS_LEN];				//登录密码
};
//修改基本资料成功
struct CMD_GP_SetBasicSubUserInfoSuccess
{
	DWORD								dwUserID;							//被操作用户ID
	DWORD								dwParentUserID;						//被操作用户父亲ID
	TCHAR								szName[NAME_LEN];					//名称
};
typedef CMD_GP_Error			CMD_GP_SetBasicSubUserInfoError;


//////////////////////////////////////////////////////////////////////////
//帐号获取
#define MDM_GP_GET_USER					6							//获取帐号
//-------------获取帐号
#define SUB_GP_GET_LOWUSERLIST			1							//获取下线帐号
						
#define SUB_GP_LIST_LOWUSER				101							//帐号
#define SUB_GP_LIST_LOWUSERERROR		102							//获取帐号失败
#define SUB_GP_LIST_SUBUSER				103							//子帐号

//帐号获取
struct CMD_GP_GetLessUserList
{
	DWORD								dwParentUserID;			//上线帐号ID
};
//帐号列表
struct CMD_GP_LessUserList
{
	DWORD								dwParentUserID;			//上线帐号ID
	WORD								wUserCount;				//帐号人数
	//tagUserData						UserData[20];
};
//子帐号列表
struct CMD_GP_SubUserList
{
	DWORD								dwParentUserID;			//上线帐号ID
	WORD								wSubUserCount;			//子帐号人数
	//tagSubUserData					UserData[20];
};
typedef CMD_GP_Error		CMD_GP_GetLessUserListError;

/////////////////////////////////////////////////////////////////////////////////
//记录获取
#define MDM_GP_GET_RECORD				7						//获取记录
//-----------------获取操作记录
#define SUB_GP_GET_OPERATION			1						//获取操作记录

#define SUB_GP_LIST_OPERATIONCONFIG		101						//操作记录列表配置
#define SUB_GP_LIST_OPERATION			102						//操作记录
#define SUB_GP_LIST_OPERATIONFINISH		103						//发送完毕
#define SUB_GP_LIST_OPERATIONERROR		104						//请求失败

struct CMD_GP_GetOperation 
{
	DWORD								dwOperationUserID;		//操作用户ID
	WORD								wPageIndex;				//页索引
	WORD								wPageSize;				//页大小
	TCHAR								szAccount[NAME_LEN];	//帐号
	BYTE								cbUserType;				//用户类型
	DWORD								dwOPType;				//操作类型
	DOUBLE								fBeginTime;				//操作开始时间
	DOUBLE								fEndTime;				//操作结束时间
};
struct CMD_GP_OperationListConfig
{
	WORD								wPageIndex;				//页索引
	WORD								wPageSize;				//页大小
	WORD								wResultCount;			//返回记录数目
	DWORD								dwTotalResultCount;		//记录总数
};
struct CMD_GP_OperationList
{
	//tagOperationData					OperationData[20];
};
typedef CMD_GP_Error		CMD_GP_OperationListError;
//------------------获取报表记录查看方式-1
#define SUB_GP_GET_CHARTVIEWONE				2						//获取报表记录查看-1

#define SUB_GP_LIST_CHARTVIEWONECONFIG		201						//报表记录列表配置
#define SUB_GP_LIST_CHARTVIEWONE			202						//报表记录
#define SUB_GP_LIST_CHARTVIEWONEFINISH		203						//发送完毕
#define SUB_GP_LIST_CHARTVIEWONEERROR		204						//请求失败

struct CMD_GP_GetChartViewOne 
{
	DWORD								dwOperationUserID;		//操作用户ID
	BYTE								cbParentFlag;			//查询上线标记
	TCHAR								szAccount[NAME_LEN];	//帐号
	DOUBLE								fBeginTime;				//开始时间
	DOUBLE								fEndTime;				//结束时间
	WORD								wGameType;				//游戏类型
	WORD								wGameKind;				//游戏种类
	BYTE								cbSortType;				//排序类型
	BYTE								cbGameRoundType;		//结算有效性
};
struct CMD_GP_ChartViewOneListConfig
{
	TCHAR								szAccount[NAME_LEN];	//帐号
	BYTE								cbLevel;				//用户级别
	DWORD								dwTotalResultCount;		//记录总数
};
struct CMD_GP_ChartViewOneList
{
	//tagChartViewOne					ChartViewOneData[20];
};
typedef CMD_GP_Error		CMD_GP_ChartViewOneListError;
//------------------获取报表记录查看-2
#define SUB_GP_GET_CHARTVIEWTWO			3							//获取报表记录查看-2

#define SUB_GP_LIST_CHARTVIEWTWOCONFIG		301						//报表记录列表配置
#define SUB_GP_LIST_CHARTVIEWTWO			302						//操作记录
#define SUB_GP_LIST_CHARTVIEWTWOFINISH		303						//发送完毕
#define SUB_GP_LIST_CHARTVIEWTWOERROR		304						//请求失败

struct CMD_GP_GetChartViewTwo 
{
	DWORD								dwOperationUserID;		//操作用户ID
	WORD								wPageIndex;				//页索引
	WORD								wPageSize;				//页大小
	TCHAR								szAccount[NAME_LEN];	//帐号
	DOUBLE								fBeginTime;				//开始时间
	DOUBLE								fEndTime;				//结束时间
	BYTE								cbLevel;				//用户级别
	WORD								wGameType;				//游戏类型
	WORD								wGameKind;				//游戏种类
	BYTE								cbSortType;				//排序类型
	BYTE								cbGameRoundType;		//结算有效性
};
struct CMD_GP_ChartViewTwoListConfig
{
	WORD								wPageIndex;				//页索引
	WORD								wPageSize;				//页大小
	WORD								wResultCount;			//返回记录数目
	DWORD								dwTotalResultCount;		//记录总数
};
struct CMD_GP_ChartViewTwoList
{
	//tagChartViewTwoData					ChartViewTwoData[20];
};
typedef CMD_GP_Error		CMD_GP_ChartViewTwoListError;
//------------------获取游戏记录查看
#define SUB_GP_GET_BETHISTORY			4						//获取游戏记录查看
#define SUB_GP_LIST_BETHISTORYCONFIG	401						//报表记录列表配置
#define SUB_GP_LIST_BETHISTORY			402						//操作记录
#define SUB_GP_LIST_BETHISTORYFINISH	403						//发送完毕
#define SUB_GP_LIST_BETHISTORYERROR		404						//请求失败

struct CMD_GP_GetBetHistory 
{
	DWORD								dwOperationUserID;		//操作用户ID
	WORD								wPageIndex;				//页索引
	WORD								wPageSize;				//页大小
	TCHAR								szAccount[NAME_LEN];	//帐号
	DOUBLE								fBeginTime;				//开始时间
	DOUBLE								fEndTime;				//结束时间
	WORD								wGameType;				//游戏类型
	WORD								wGameKind;				//游戏种类
	BYTE								cbSortType;				//排序类型
	BYTE								cbGameRoundType;		//结算有效性
};
struct CMD_GP_BetHistoryListConfig
{
	WORD								wPageIndex;				//页索引
	WORD								wPageSize;				//页大小
	WORD								wResultCount;			//返回记录数目
	DWORD								dwTotalResultCount;		//记录总数
};
struct CMD_GP_BetHistoryList
{
	//tagBetHistoryData					BetHistoryData[20];
};
typedef CMD_GP_Error		CMD_GP_BetHistoryListError;

//------------------获取游戏局记录详细投注信息
#define SUB_GP_GET_DETAILBETSCORE			5						//获取游戏局记录详细投注信息

#define SUB_GP_GET_DETAILBETSCORESUCCESS	501						//获取游戏局记录详细投注信息成功
#define SUB_GP_GET_DETAILBETSCOREERROR		502						//获取游戏局记录详细投注信息成功

struct CMD_GP_GetDetailBetScore 
{
	DWORD								dwOperationUserID;			//操作用户ID
	DOUBLE								fUCID;						//报表记录ID
};
struct CMD_GP_GetDetailBetScoreSuccess
{
	DOUBLE								fUCID;						//报表记录ID
	WORD								wDetailBetScoreSize;		//投注详细大小
	BYTE								cbDetailBetScore[1536];		//投注详细
};
typedef CMD_GP_Error		CMD_GP_GetDetailBetScoreError;
//------------------获取报表记录查看-3
#define SUB_GP_GET_CHARTVIEWTHREE			6						//获取报表记录查看-3

#define SUB_GP_CHARTVIEWTHREESUCCESS		601						//报表记录列表配置
#define SUB_GP_CHARTVIEWTHREEERROR			602						//请求失败

struct CMD_GP_GetChartViewThree 
{
	DWORD								dwOperationUserID;			//操作用户ID
	DWORD								dwUserID;					//帐号ID
	DOUBLE								fBeginTime;					//开始时间
	DOUBLE								fEndTime;					//结束时间
	WORD								wGameType;					//游戏类型
	WORD								wGameKind;					//游戏种类
	BYTE								cbGameRoundType;			//结算有效性
};

struct CMD_GP_ChartViewThreeSucess
{
	//投注
	DWORD	dwGameRoundCount;		//局数
	DOUBLE	fTotalBetScore;			//投注总额
	DOUBLE	fTotalWinScore;			//总派彩
	DOUBLE	fValidScore_Less_Rollback;	//洗码-下线洗码

};
typedef CMD_GP_Error		CMD_GP_ChartViewThreeError;

/////////////////////////////////////////////////////////////////////////////////
#define MDM_GP_SERVER_INFO			8									//房间信息

#define SUB_GP_ONLINE_COUNT_INFO	1									//在线信息

//人数信息
struct tagOnLineCountInfo
{
	WORD							wTypeID;							//种类标识
	WORD							wKindID;							//类型标识
	DWORD							dwOnLineCount;						//在线人数
};
/////////////////////////////////////////////////////////////////////////////////
#pragma pack(pop)


#endif