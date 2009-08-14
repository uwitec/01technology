#ifndef COMMONALITY_HEAD_FILE
#define COMMONALITY_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//定义头文件

//命令码文件
#include "..\..\CommandDefinition\CMD_Plaza.h"
#include "..\..\CommandDefinition\CMD_Center.h"

//全局头文件
#include "..\..\CommonDefinition\GlobalField.h"
#include "..\..\CommonDefinition\GlobalService.h"
#include "..\..\CommonDefinition\GlobalOperation.h"
#include "..\..\CommonDefinition\GlobalGameRoundAndChart.h"
//////////////////////////////////////////////////////////////////////////
//操作者
struct tagOperationUserInfo
{
	DWORD			dwOperationUserID;					//操作用户ID
	BYTE			cbOperationUserType;				//操作用户类型
	TCHAR			szOperationAccount[NAME_LEN];		//操作帐号
	DWORD			dwIPAddress;						//操作地址
};
//////////////////////////////////////////////////////////////////////////
//数据库对象定义
static const DWORD dwDBRequstOffset= 5000;

#define	DBR_GP_LOGON_BY_ACCOUNT						10		//帐号登陆

//数据库输出标识
#define DBR_GP_LOGON_SUCCESS						dwDBRequstOffset + 10//登陆成功
#define DBR_GP_LOGON_ERROR							dwDBRequstOffset + 11//登陆失败
//帐号登陆
struct DBR_GP_LogonByAccount
{
	DWORD							dwClientIP;							//连接地址
	TCHAR							szAccounts[NAME_LEN];				//登录帐号
	TCHAR							szPassWord[PASS_LEN];				//登录密码
	TCHAR							szRealityPass[PASS_LEN];			//真实密码
	BYTE							cbClientType;						//客户端类型
};


//登陆成功
typedef CMD_GP_LogonSuccess DBR_GP_LogonSuccess;

//登陆失败
typedef CMD_GP_Error DBR_GP_LogonError;

//////////////////////////////////////////////////////////////////////////

#define DBR_GP_REGISTER_USER						20					//注册帐号
//帐号注册
struct DBR_GP_RegisterUser
{
	tagOperationUserInfo			Operator;							//操作者

	DWORD							dwOperationUserID;					//操作用户ID
	DWORD							dwClientIP;							//连接地址
	tagUserData						UserData;
	TCHAR							szPassWord[PASS_LEN];				//登录密码
};

//////////////////////////////////////////////////////////////////////////

#define DBR_GP_GET_LOWUSERLIST						30					//获取下线帐号
#define DBR_GP_GET_SUBUSERLIST						31					//获取子帐号
//数据库输出标识
#define DBR_GP_GET_LOWUSERLISTCOMPLETE				dwDBRequstOffset + 30//发送下线帐号完毕

typedef CMD_GP_GetLessUserList DBR_GP_GetLessUserList;
typedef CMD_GP_GetLessUserList DBR_GP_GetSubUserList;

//////////////////////////////////////////////////////////////////////////

#define DBR_GP_SET_SETUSERSCORE						40					//额度存取

struct DBR_GP_SetUserScore 
{
	tagOperationUserInfo			Operator;							//操作者
	CMD_GP_SetUserScore				RequestData;						//请求数据
};

//////////////////////////////////////////////////////////////////////////

#define DBR_GP_SET_USERCONGEALSTATE					50					//状态设定
struct DBR_GP_SetUserCongealState 
{
	tagOperationUserInfo			Operator;							//操作者
	CMD_GP_SetUserCongealState		RequestData;						//请求数据
};

//////////////////////////////////////////////////////////////////////////

#define DBR_GP_SET_BASICUSERINFO					60					//基本信息设定
struct DBR_GP_SetBasicUserInfo 
{
	tagOperationUserInfo			Operator;							//操作者
	CMD_GP_BasicUserInfo		RequestData;							//请求数据
};

//////////////////////////////////////////////////////////////////////////

#define DBR_GP_SET_DETAILUSERINFO					70					//详细信息设定
struct DBR_GP_SetDetailUserInfo 
{
	tagOperationUserInfo			Operator;							//操作者
	CMD_GP_DetailUserInfo		RequestData;							//请求数据
};

//////////////////////////////////////////////////////////////////////////

#define DBR_GP_SET_USERRIGHT						80					//权限设定
struct DBR_GP_SetUserRight 
{
	tagOperationUserInfo			Operator;							//操作者
	CMD_GP_SetUserRight		RequestData;								//请求数据
};

//////////////////////////////////////////////////////////////////////////

#define DBR_GP_SET_MYSELFBASICUSERINFO				90					//自身基本信息设定
struct DBR_GP_SetMySelfBasicUserInfo 
{
	tagOperationUserInfo			Operator;							//操作者
	CMD_GP_MySelfBasicUserInfo		RequestData;						//请求数据
};



//////////////////////////////////////////////////////////////////////////

#define DBR_GP_SET_SUBUSERCONGEALSTATE				100					//子帐号状态设定
struct DBR_GP_SetSubUserCongealState 
{
	tagOperationUserInfo			Operator;							//操作者
	CMD_GP_SetSubUserCongealState		RequestData;					//请求数据
};

//////////////////////////////////////////////////////////////////////////

#define DBR_GP_REGISTER_SUBUSER						110					//注册子帐号
struct DBR_GP_RegisterSubUserInfo 
{
	tagOperationUserInfo			Operator;							//操作者
	CMD_GP_RegisterSubUser		RequestData;							//请求数据
};

//////////////////////////////////////////////////////////////////////////

#define DBR_GP_SET_BASICSUBUSERINFO					120					//子帐号基本信息设定
struct DBR_GP_SetBasicSubUserInfo 
{
	tagOperationUserInfo			Operator;							//操作者
	CMD_GP_BasicSubUserInfo		RequestData;							//请求数据
};

//////////////////////////////////////////////////////////////////////////
#define DBR_GP_GET_OPERATION						130					//获取操作记录
struct DBR_GP_GetOperation 
{
	tagOperationUserInfo			Operator;							//操作者
	CMD_GP_GetOperation				RequestData;						//请求数据
};

//////////////////////////////////////////////////////////////////////////
#define DBR_GP_GET_CHARTVIEWONE						140					//获取游戏报表记录方式-1
struct DBR_GP_GetChartViewOne
{
	tagOperationUserInfo			Operator;							//操作者
	CMD_GP_GetChartViewOne			RequestData;						//请求数据
};
//////////////////////////////////////////////////////////////////////////
#define DBR_GP_GET_CHARTVIEWTWO						150					//获取游戏报表记录方式-2
struct DBR_GP_GetChartViewTwo
{
	tagOperationUserInfo			Operator;							//操作者
	CMD_GP_GetChartViewTwo			RequestData;						//请求数据
};
//////////////////////////////////////////////////////////////////////////
#define DBR_GP_GET_BETHISTORY						160					//获取游戏局记录
struct DBR_GP_GetBetHistory
{
	tagOperationUserInfo			Operator;							//操作者
	CMD_GP_GetBetHistory			RequestData;						//请求数据
};
//////////////////////////////////////////////////////////////////////////
#define DBR_GP_GET_DETAILBETSCORE					170					//获取游戏局记录详细投注信息
struct DBR_GP_GetDetailBetScore
{
	tagOperationUserInfo			Operator;							//操作者
	CMD_GP_GetDetailBetScore		RequestData;						//请求数据
};
//////////////////////////////////////////////////////////////////////////
#define DBR_GP_GET_CHARTVIEWTHREE					180					//获取游戏报表记录方式-3
struct DBR_GP_GetChartViewThree
{
	tagOperationUserInfo			Operator;							//操作者
	CMD_GP_GetChartViewThree		RequestData;						//请求数据
};
///////////////////////////////////////////////////////////////////////////
#define DBR_UPDATE_GAMEMAININFO						1000				//更新游戏主信息
#define DBR_UPDATE_GAMEMAININFO_SUCCESS				1001				//更新游戏主信息
struct DBR_Update_GameMainInfo
{
	CMD_GP_GameInfo					GameInfo;							//游戏信息
	CMD_GP_Notice					ManagementNotice;					//管理公告
	CMD_GP_Notice					GameNotice;							//游戏公告

};

//////////////////////////////////////////////////////////////////////////
#define	DBR_GP_LOGOUT_USER							1010				//注销帐号登陆
struct DBR_GP_LogoutUser
{
	DWORD							dwRealUserID;						//用户帐号
	BYTE							cbUserType;							//用户类型
};

//////////////////////////////////////////////////////////////////////////
//其他命令
#define	MR_EVENT_SEND_USER_MESSAGE					2000				//发送用户消息
struct 	MR_EVENT_SendUserMessage
{
	DWORD							dwRealUserID;						//用户帐号
	BYTE							cbUserType;							//用户类型
	BOOL							bKick;								//登出标记
	TCHAR							szMessage[1024];					//用户消息
};

#endif