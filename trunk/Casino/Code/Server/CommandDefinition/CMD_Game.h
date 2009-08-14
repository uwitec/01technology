#ifndef CMD_GAME_HEAD_FILE
#define CMD_GAME_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//登录数据包定义
#pragma pack(push)
#pragma pack(1)
#define MDM_GR_LOGON				1									//房间登录

#define SUB_GR_LOGON_ACCOUNT		1									//帐户登录

#define SUB_GR_LOGON_SUCCESS		100									//登陆成功
#define SUB_GR_LOGON_ERROR			101									//登陆失败
#define SUB_GR_LOGON_FINISH			102									//登陆完成

//房间帐号登录
struct CMD_GR_LogonByAccount
{
	DWORD							dwPlazaVersion;						//广场版本
	DWORD							dwProcessVersion;					//进程版本
	TCHAR							szAccounts[NAME_LEN];				//登录帐号
	TCHAR							szPassWord[PASS_LEN];				//登录密码
};


//登陆成功消息
struct CMD_GR_LogonSuccess
{
	DWORD							dwUserID;							//用户 I D
};

//登陆失败
struct CMD_GR_LogonError
{
	WORD							wErrorCode;							//错误代码
};

//////////////////////////////////////////////////////////////////////////
//用户数据包定义

#define MDM_GR_USER					2									//用户信息

#define SUB_GR_USER_SIT_REQ			1									//坐下请求
#define SUB_GR_USER_LOOKON_REQ		2									//旁观请求
#define SUB_GR_USER_STANDUP_REQ		3									//起立请求
#define SUB_GR_USER_LEFT_GAME_REQ	4									//离开游戏

#define SUB_GR_USER_COME			100									//用户进入
#define SUB_GR_USER_STATUS			101									//用户状态
#define SUB_GR_USER_SCORE			102									//用户分数
#define SUB_GR_SIT_FAILED			103									//坐下失败

#define SUB_GR_USER_RULE			200									//用户规则
#define SUB_GR_USER_CHAT			201									//聊天消息
#define SUB_GR_USER_WISPER			202									//私语消息

#define SUB_GR_USER_INVITE			300									//邀请消息
#define SUB_GR_USER_INVITE_REQ		301									//邀请请求

//用户状态
struct CMD_GR_UserStatus
{
	DWORD							dwUserID;							//数据库 ID
	WORD							wTableID;							//桌子位置
	WORD							wChairID;							//椅子位置
	WORD							wNetDelay;							//网络延时
	BYTE							cbUserStatus;						//用户状态
};

//用户分数
struct CMD_GR_UserScore
{
	DWORD							dwUserID;							//用户 I D
	tagUserScore					UserScore;							//信用额度信息
};

//请求坐下
struct CMD_GR_UserSitReq
{
	WORD							wTableID;							//桌子位置
	WORD							wChairID;							//椅子位置
	WORD							wNetTimelag;						//网络延时
	BYTE							cbPassLen;							//密码长度
	TCHAR							szTablePass[PASS_LEN];				//桌子密码
};
#define AUTO_TABLEID				1024								//服务器自动选择位置
#define AUTO_CHAIRID				1024								//服务器自动选择位置

//邀请用户 
struct CMD_GR_UserInviteReq
{
	WORD							wTableID;							//桌子号码
	DWORD							dwUserID;							//用户 I D
};

//坐下失败
struct CMD_GR_SitFailed
{
	WORD							wErrorCode;//错误描述
};

//聊天结构 
struct CMD_GR_UserChat
{
	WORD							wChatLength;						//信息长度
	DWORD							dwSendUserID;						//发送用户
	DWORD							dwTargetUserID;						//目标用户
	TCHAR							szChatMessage[MAX_CHAT_LEN];		//聊天信息
};

//私语结构 
struct CMD_GR_Wisper
{
	WORD							wChatLength;						//信息长度
	DWORD							dwSendUserID;						//发送用户
	DWORD							dwTargetUserID;						//目标用户
	TCHAR							szChatMessage[MAX_CHAT_LEN];		//聊天信息
};

//用户规则
struct CMD_GR_UserRule
{
	bool							bPassword;							//设置密码
	bool							bLimitScore;						//限制分数
	bool							bLimitDelay;						//限制延时
	bool							bCheckSameIP;						//效验地址
	WORD							wWinRate;							//限制胜率
	WORD							wFleeRate;							//限制逃跑
	WORD							wNetDelay;							//网络延时
	LONG							lMaxScore;							//最高分数 
	LONG							lLessScore;							//最低分数
	TCHAR							szPassword[PASS_LEN];				//桌子密码
};

//邀请用户 
struct CMD_GR_UserInvite
{
	WORD							wTableID;							//桌子号码
	DWORD							dwUserID;							//用户 I D
};

//////////////////////////////////////////////////////////////////////////
//配置信息数据包

#define MDM_GR_INFO					3									//配置信息

#define SUB_GR_SERVER_INFO			100									//房间配置
#define SUB_GR_CONFIG_FINISH		101									//配置完成

//游戏房间信息
struct CMD_GR_ServerInfo
{
	WORD							wKindID;							//类型 I D
	WORD							wGameGenre;							//游戏类型
	WORD							wTableCount;						//桌子数目
	WORD							wChairCount;						//椅子数目
};


//////////////////////////////////////////////////////////////////////////
//房间状态数据包

#define MDM_GR_STATUS				4									//状态信息

#define SUB_GR_TABLE_INFO			100									//桌子信息
#define SUB_GR_TABLE_STATUS			101									//桌子状态

//桌子状态结构
struct tagTableStatus
{
	BYTE							bTableLock;							//锁定状态
	BYTE							bPlayStatus;						//游戏状态
};

//桌子状态数组
struct CMD_GR_TableInfo
{
	WORD							wTableCount;						//桌子数目
	tagTableStatus					TableStatus[512];					//状态数组
};

//桌子状态信息
struct CMD_GR_TableStatus
{
	WORD							wTableID;							//桌子号码
	BYTE							bPlayStatus;						//游戏状态
};

//////////////////////////////////////////////////////////////////////////
//管理数据包

#define MDM_GR_MANAGER				5									//管理命令

#define SUB_GR_SEND_WARNING			1									//发送警告
#define SUB_GR_SEND_MESSAGE			2									//发送消息
#define SUB_GR_LOOK_USER_IP			3									//查看地址
#define SUB_GR_KILL_USER			4									//踢出用户
#define SUB_GR_OPTION_SERVER		5									//房间设置

#define SUB_GR_LOOK_USER_IP_RESULT	30									//返回查看地址


//发送警告
struct CMD_GR_SendWarning
{
	WORD							wChatLength;						//信息长度
	DWORD							dwTargetUserID;						//目标用户
	TCHAR							szWarningMessage[MAX_CHAT_LEN];		//警告消息
};

//系统消息
struct CMD_GR_SendMessage
{
	BYTE							cbGame;								//游戏消息
	BYTE							cbRoom;								//游戏消息
	WORD							wChatLength;						//信息长度
	TCHAR							szSystemMessage[MAX_CHAT_LEN];		//系统消息
};

//查看地址
struct CMD_GR_LookUserIP
{
	DWORD							dwTargetUserID;						//目标用户
};
//查看地址结果
struct CMD_GR_LookUserIPResult
{
	DWORD							dwTargetUserID;						//目标用户
	TCHAR							szAccount[NAME_LEN];				//帐号
	TCHAR							szName[NAME_LEN];					//名称
	DWORD							dwUserIP;							//IP
	TCHAR							szIPDescribe[256];					//IP描叙
};

//踢出用户
struct CMD_GR_KillUser
{
	DWORD							dwTargetUserID;						//目标用户
};



//设置标志
#define OSF_ROOM_CHAT				1									//大厅聊天
#define OSF_GAME_CHAT				2									//游戏聊天
#define OSF_ROOM_WISPER				3									//大厅私聊
#define OSF_ENTER_GAME				4									//进入游戏
#define OSF_ENTER_ROOM				5									//进入房间
#define OSF_SHALL_CLOSE				6									//即将关闭

//房间设置
struct CMD_GR_OptionServer
{
	BYTE							cbOptionFlags;						//设置标志
	BYTE							cbOptionValue;						//设置标志
};

//////////////////////////////////////////////////////////////////////////
//系统数据包

#define MDM_GR_SYSTEM				10									//系统信息

#define SUB_GR_MESSAGE				100									//系统消息

//消息类型
#define SMT_INFO					0x0001								//信息消息
#define SMT_EJECT					0x0002								//弹出消息
#define SMT_GLOBAL					0x0004								//全局消息
#define SMT_CLOSE_ROOM				0x1000								//关闭房间
#define SMT_INTERMIT_LINE			0x4000								//中断连接

//消息数据包
struct CMD_GR_Message
{
	WORD							wMessageType;						//消息类型
	WORD							wErrorCode;							//错误消息代码
	WORD							wMessageLength;						//消息长度
	TCHAR							szContent[1024];					//消息内容
};

//////////////////////////////////////////////////////////////////////////
//房间数据包

#define MDM_GR_SERVER_INFO			11									//房间信息

#define SUB_GR_ONLINE_COUNT_INFO	100									//在线信息

//人数信息
struct tagGameServerOnLineCountInfo
{
	WORD							wServerID;							//类型标识
	DWORD							dwOnLineCount;						//在线人数
};

//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)

#endif